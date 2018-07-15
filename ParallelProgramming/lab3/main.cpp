#include <iostream>
#include <cstring>
#include <cstdlib>
#include <omp.h>
#include "../lib/CImg-2.3.3/CImg.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

using namespace std;
using namespace cimg_library;

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

int main(int argc, char *argv[]) {
    int threadNum = 4;
    int blockSize = 128;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-n"))
            threadNum = atoi(argv[++i]);
        if (!strcmp(argv[i], "-s"))
            blockSize = atoi(argv[++i]);
    }
    cout << "running in " << threadNum << " thread(s)." << endl;
    cout << "block size " << blockSize << endl;

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

    // do erode and dilate
    CImg<unsigned char>
    erodedImage(binaryImage.width(), binaryImage.height(), 1, 1),
                dilatedImage(binaryImage.width(), binaryImage.height(), 1, 1);

    // split work
    Work works[(binaryImage.height() / blockSize + 1) *
        (binaryImage.width() / blockSize + 1)];
    int workNum = 0;
    for (int row = 0; row < binaryImage.height(); row += blockSize) {
        for(int col=0; col < binaryImage.width(); col += blockSize){
            works[workNum].colStart = col;
            works[workNum].colEnd = min(col + blockSize, binaryImage.width());
            works[workNum].rowStart = row;
            works[workNum].rowEnd = min(row + blockSize, binaryImage.height());
            ++workNum;
        }
    }

    // remove unnecessary threads
    if (threadNum > workNum){
        threadNum = workNum;
        cout << "using only " << workNum << " threads for "
            << workNum << " works" << endl;
    }

    for (int repeater = 0; repeater < 1000; ++repeater) {
        int result;

        #pragma omp parallel for num_threads(threadNum) schedule(dynamic, 2)
        for(int i=0; i < workNum; ++i){
            for (int row = works[i].rowStart; row < works[i].rowEnd; ++row) {
                for (int col = works[i].colStart; col < works[i].colEnd; ++col) {
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
        }
        cout << "round " << repeater << " finished" << endl;
    }

    binaryImage.normalize(0, 255);
    binaryImage.save("binary.png");

    erodedImage.normalize(0, 255);
    erodedImage.save("eroded.png");

    dilatedImage.normalize(0, 255);
    dilatedImage.save("dilated.png");

    return 0;
}

