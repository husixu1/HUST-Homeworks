#include <iostream>
#include "../lib/CImg-2.3.3/CImg.h"
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

int main() {
    // TODO: argv
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

    for(int repeater=0; repeater < 1000; ++repeater){
        for (int row = 0; row < binaryImage.height(); ++row) {
            for (int col = 0; col < binaryImage.width(); ++col) {
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
