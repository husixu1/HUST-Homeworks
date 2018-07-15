#include <cuda_runtime_api.h>
#include <cuda.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "../lib/CImg-2.3.3/CImg.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

using namespace std;
using namespace cimg_library;

#define maskWidth 5
#define maskHeight 5


struct Work {
    int rowStart;
    int rowEnd;
    int colStart;
    int colEnd;
};

const unsigned char erodeMask[maskWidth * maskHeight] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 1, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};
const unsigned char dilateMask[maskWidth * maskHeight] = {
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
};

//const unsigned char erodeMask[maskWidth * maskHeight] = {
//    0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0,
//}, dilateMask[maskWidth * maskHeight] = {
//    0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0,
//    0, 0, 0, 0, 0,
//};

__global__ void pixelProcessErodeKernel(
        int blockWidth,
        int blockHeight,
        int binaryImageWidth,
        int binaryImageHeight,
        int expandedImageWidth,
        unsigned char *expandedImage,
        unsigned char *erodedImage,
        unsigned char *erodeMask
        ){
    // get cuda parameter
    int blockX = blockIdx.x;
    int blockY = blockIdx.y;
    int x = threadIdx.x;
    int y = threadIdx.y;

    // calculate index
    int blocksPerRow = (binaryImageWidth / blockWidth + 1);
    int blocksPerCol = (binaryImageHeight/ blockHeight + 1);
    int col = (blockX % blocksPerRow) * blockWidth + x;
    int row = (blockY % blocksPerCol) * blockHeight + y;
    int index = row * binaryImageWidth + col;

    unsigned char erodeResult = 1;
    unsigned char dilateResult = 1;

    if(col < binaryImageWidth && row < binaryImageHeight){
        for(int maskRow=0; maskRow < maskHeight; ++maskRow){
            for(int maskCol=0; maskCol < maskWidth; ++maskCol){
                int expandedIndex = (row + maskRow) * expandedImageWidth + (col + maskCol);
                erodeResult &= (expandedImage[expandedIndex] |
                        erodeMask[maskRow * maskWidth + maskCol]);
            }
        }
    }

    erodedImage[index] = erodeResult;
}

__global__ void pixelProcessDilateKernel(
        int blockWidth,
        int blockHeight,
        int binaryImageWidth,
        int binaryImageHeight,
        int expandedImageWidth,
        unsigned char *expandedImage,
        unsigned char *dilatedImage,
        unsigned char *dilateMask
        ){
    // get cuda parameter
    int blockX = blockIdx.x;
    int blockY = blockIdx.y;
    int x = threadIdx.x;
    int y = threadIdx.y;

    // calculate index
    int blocksPerRow = (binaryImageWidth / blockWidth + 1);
    int blocksPerCol = (binaryImageHeight/ blockHeight + 1);
    int col = (blockX % blocksPerRow) * blockWidth + x;
    int row = (blockY % blocksPerCol) * blockHeight + y;
    int index = row * binaryImageWidth + col;

    unsigned char dilateResult = 0;

    if(col < binaryImageWidth && row < binaryImageHeight){
        for(int maskRow=0; maskRow < maskHeight; ++maskRow){
            for(int maskCol=0; maskCol < maskWidth; ++maskCol){
                int expandedIndex = (row + maskRow) * expandedImageWidth + (col + maskCol);
                dilateResult |= (expandedImage[expandedIndex] &
                        dilateMask[maskRow * maskWidth + maskCol]);
            }
        }
    }

    dilatedImage[index] = dilateResult;
}

int main(int argc, char *argv[]) {
    int blockSize = 32;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-s"))
            blockSize = atoi(argv[++i]);
    }
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

    //    // split work
    //    Work works[(binaryImage.height() / blockSize + 1) *
    //        (binaryImage.width() / blockSize + 1)];
    //    int workNum = 0;
    //    for (int row = 0; row < binaryImage.height(); row += blockSize) {
    //        for(int col=0; col < binaryImage.width(); col += blockSize){
    //            works[workNum].colStart = col;
    //            works[workNum].colEnd = min(col + blockSize, binaryImage.width());
    //            works[workNum].rowStart = row;
    //            works[workNum].rowEnd = min(row + blockSize, binaryImage.height());
    //            ++workNum;
    //        }
    //    }

    unsigned char *cudaErodeMask, *cudaDilateMask;
    int maskSize = sizeof(unsigned char) * maskWidth * maskHeight;
    cudaMalloc(&cudaErodeMask, maskSize);
    cudaMalloc(&cudaDilateMask, maskSize);
    cudaMemcpy(cudaErodeMask, erodeMask, maskSize, cudaMemcpyHostToDevice);
    cudaMemcpy(cudaDilateMask, dilateMask, maskSize, cudaMemcpyHostToDevice);

    for (int repeater = 0; repeater < 1000; ++repeater) {
        int result;

        // malloc graphics ram for image, move the image to cuda
        unsigned char *cudaExpandedImage, *cudaErodedImage, *cudaDilatedImage;
        int expandedImageSize = sizeof(unsigned char) * expandedImage.size();
        int binaryImageSize = sizeof(unsigned char) * binaryImage.size();
        cudaMalloc(&cudaExpandedImage, expandedImageSize);
        cudaMemcpy(cudaExpandedImage, expandedImage.data(),
                expandedImageSize, cudaMemcpyHostToDevice);

        cudaMalloc(&cudaErodedImage, binaryImageSize);
        cudaMalloc(&cudaDilatedImage, binaryImageSize);

        dim3 thread(blockSize, blockSize);
        dim3 block(binaryImage.width() / blockSize + 1,
                binaryImage.height() / blockSize + 1);

        pixelProcessErodeKernel<<<block, thread>>>(
                blockSize, blockSize,
                binaryImage.width(), binaryImage.height(),
                expandedImage.width(), cudaExpandedImage,
                cudaErodedImage, cudaErodeMask);

        pixelProcessDilateKernel<<<block, thread>>>(
                blockSize, blockSize,
                binaryImage.width(), binaryImage.height(),
                expandedImage.width(), cudaExpandedImage,
                cudaDilatedImage, cudaDilateMask);

        cudaMemcpy(erodedImage.data(), cudaErodedImage,
                binaryImageSize, cudaMemcpyDeviceToHost);
        cudaMemcpy(dilatedImage.data(), cudaDilatedImage,
                binaryImageSize, cudaMemcpyDeviceToHost);

        cudaFree(cudaExpandedImage);
        cudaFree(cudaErodedImage);
        cudaFree(cudaDilatedImage);

        cout << "round " << repeater << " finished" << endl;
    }

    cudaFree(cudaErodeMask);
    cudaFree(cudaDilateMask);

    binaryImage.normalize(0, 255);
    binaryImage.save("binary.png");

    erodedImage.normalize(0, 255);
    erodedImage.save("eroded.png");

    dilatedImage.normalize(0, 255);
    dilatedImage.save("dilated.png");

    return 0;
}

