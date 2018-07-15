#include <iostream>
#include <cstring>
#include <cstdlib>
#include <mpi.h>
#include "../lib/CImg-2.3.3/CImg.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

using namespace std;
using namespace cimg_library;

static const int repeatNum = 1000;

static const int maskWidth = 5;
static const int maskHeight = 5;
static const unsigned char erodeMask[maskWidth][maskHeight] = {
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
},
dilateMask[maskWidth][maskHeight] = {
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
};

struct Work {
    int rowStart;
    int rowEnd;
    int colStart;
    int colEnd;
};

void mainProcess(int argc, char *argv[]) {
    CImg<unsigned char> srcImage("../data/Lenna.png");

    // Convert RGB image to grayscale image
    CImg<unsigned char> grayScaleImage(
            srcImage.width(), srcImage.height(), 1, 1);
    unsigned char r, g, b;
    unsigned char gr1 = 0;
    for (int i = 0; i < srcImage.width(); ++i) {
        for (int j = 0; j < srcImage.height(); ++j) {
            //Return a pointer to a located pixel value.
            r = srcImage(i, j, 0, 0); // First channel RED
            g = srcImage(i, j, 0, 1); // Second channel GREEN
            b = srcImage(i, j, 0, 2); // Third channel BLUE

            //PAL and NTSC
            //Y = 0.299*R + 0.587*G + 0.114*B
            gr1 = round(0.299 * ((double)r) + 0.587 * ((double)g) + 0.114 * ((double)b));

            // assign the luminosity value
            grayScaleImage(i, j, 0, 0) = gr1;
        }
    }

    // binarization
    CImg<unsigned char> binaryImage = grayScaleImage.get_threshold(128);

    // expansion
    CImg<unsigned char> expandedImage = binaryImage.get_resize(
                    binaryImage.width() + maskWidth - 1,
                    binaryImage.height() + maskHeight - 1,
                    1, 1,
                    0, 0, // fill border with black
                    0.5, 0.5);

    CImg<unsigned char>
    erodedImage(binaryImage.width(), binaryImage.height(), 1, 1),
                dilatedImage(binaryImage.width(), binaryImage.height(), 1, 1);

    // do erode and dilate
    for (int repeater = 0; repeater < repeatNum; ++repeater) {
        int result;

        // broadcast image size, width, height first
        int info[5];
        info[0] = expandedImage.size();
        info[1] = expandedImage.width();
        info[2] = expandedImage.height();
        info[3] = binaryImage.width();
        info[4] = binaryImage.height();
        MPI_Bcast(&info, 5, MPI_INT, 0, MPI_COMM_WORLD);

        // broadcast image data
        MPI_Bcast(expandedImage.data(), info[0], MPI_UINT8_T, 0, MPI_COMM_WORLD);

        // retrieve image data
        int processNum;
        MPI_Comm_size(MPI_COMM_WORLD, &processNum);
        int sendCount = binaryImage.width() *
                (binaryImage.height() / (processNum - 1) + 1);
        int recvCount = sendCount * processNum;

        // retrieve erosion image
        unsigned char dummyBuf[sendCount];
        unsigned char recvBuf[recvCount];

        MPI_Gather(dummyBuf, sendCount, MPI_UINT8_T,
                recvBuf, sendCount, MPI_UINT8_T,
                0, MPI_COMM_WORLD);
        erodedImage.assign(recvBuf + sendCount,
                binaryImage.width(), binaryImage.height(), 1, 1);

        // retrive dilated image
        MPI_Gather(dummyBuf, sendCount, MPI_UINT8_T,
                recvBuf, sendCount, MPI_UINT8_T,
                0, MPI_COMM_WORLD);
        dilatedImage.assign(recvBuf + sendCount,
                binaryImage.width(), binaryImage.height(), 1, 1);

        cout << "round " << repeater << " finished" << endl;
    }

    binaryImage.normalize(0, 255);
    binaryImage.save("binary.png");

    erodedImage.normalize(0, 255);
    erodedImage.save("eroded.png");

    dilatedImage.normalize(0, 255);
    dilatedImage.save("dilated.png");
}

void workProcess() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for(int repeater=0; repeater < repeatNum; ++repeater){
        // receive image size
        int info[5];
        MPI_Bcast(&info, 5, MPI_INT, 0, MPI_COMM_WORLD);
        int imageSize = info[0];
        int imageWidth = info[1];
        int imageHeight = info[2];
        int binaryImageWidth = info[3];
        int binaryImageHeight = info[4];

        // receive image data
        unsigned char *buf = new unsigned char[imageSize];
        MPI_Bcast(buf, imageSize, MPI_UINT8_T, 0, MPI_COMM_WORLD);

        // construct image
        CImg<unsigned int> expandedImage(buf, imageWidth, imageHeight, 1, 1);
        delete []buf;

        int processNum;
        MPI_Comm_size(MPI_COMM_WORLD, &processNum);
        int sendCount = binaryImageWidth *
            (binaryImageHeight / (processNum - 1) + 1);

        CImg<unsigned char>
            erodedImage(binaryImageWidth,
                    (binaryImageHeight / (processNum - 1) + 1) * (processNum - 1), 1, 1),
            dilatedImage(binaryImageWidth,
                    (binaryImageHeight / (processNum - 1) + 1) * (processNum - 1), 1, 1);

        int startRow = (binaryImageHeight / (processNum - 1) + 1) * (rank - 1);
        for (int row = startRow;
                row < startRow + (binaryImageHeight / (processNum - 1) + 1) &&
                row < binaryImageHeight; ++row) {
            for (int col = 0; col < binaryImageWidth; ++col) {
                unsigned char erodeResult = 1;
                unsigned char dilateResult = 0;
                for(int maskRow=0; maskRow < maskHeight; ++maskRow){
                    for(int maskCol=0; maskCol < maskWidth; ++maskCol){
                        erodeResult &= (expandedImage(col + maskCol, row + maskRow) |
                                erodeMask[maskRow][maskCol]);
                        dilateResult |= (expandedImage(col + maskCol, row + maskRow) &
                                dilateMask[maskRow][maskCol]);
                    }
                }
                erodedImage(col, row) = erodeResult;
                dilatedImage(col, row) = dilateResult;
            }
        }

        // send eroded image part
        MPI_Gather(erodedImage.data() + startRow * binaryImageWidth,
                sendCount, MPI_UINT8_T,
                nullptr, 0, MPI_UINT8_T,
                0, MPI_COMM_WORLD);

        // send dilated image part
        MPI_Gather(dilatedImage.data() + startRow * binaryImageWidth,
                sendCount, MPI_UINT8_T,
                nullptr, 0, MPI_UINT8_T,
                0, MPI_COMM_WORLD);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(nullptr, nullptr);
    int processNum, curProcessRank;
    // get total process num and current process num
    MPI_Comm_size(MPI_COMM_WORLD, &processNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &curProcessRank);
    if (processNum <= 1) {
        cerr << "This program cannot be run in a single process";
        return 1;
    }

    if (curProcessRank == 0) {
        mainProcess(argc, argv);
    } else {
        workProcess();
    }

    MPI_Finalize();
    return 0;
}
