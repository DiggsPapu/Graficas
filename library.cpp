/**
 * @file texture.cpp
 * @author Diego Alonzo (alo20172@uvg.edu.gt)
 * @brief 
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
using namespace std;
struct Pixel {
    unsigned char red, green, blue;
};
#pragma pack(push, 1)
struct BmpHeader {
    uint16_t signature;
    uint32_t filesize,reserved,dataoffset,size;
    int32_t width,height;
    uint16_t planes,bitsPerPixel;
    uint32_t compression,imageSize,ColorsUsed,ImportantColors;
    int32_t XpixelsPerM,YpixelsPerM;
};
#pragma pack(pop)
struct dataImg {
    int width,height;
    Pixel* imageData;
};
// Global variables
// dataImg image;
// Pixel currentC = {255,255,255};
int getPixelIndex (int x, int y, dataImg image){
    return y*image.width + x;
}
void clearAllImage(dataImg image){
    for (int y = 0; y < image.height; y++)
    {
        for (int x = 0; x < image.width; x++)
        {
            int pixelIndex = getPixelIndex(x,y,image);
            image.imageData[pixelIndex].blue = 255;
            image.imageData[pixelIndex].red = 250;
            image.imageData[pixelIndex].green = 90;
        }
    }
}
void writeBmp(const std::string& filename, dataImg image) {
    std::ofstream file(filename, std::ios::binary);
    if(!file){cout<<"File couldnt be opened"<<endl;return;}
    BmpHeader header;
    // Header fields initialization
    header.signature = 0x4D42;
    header.filesize = sizeof(BmpHeader) + image.height * image.width * 3;
    header.reserved = 0;
    header.dataoffset = sizeof(BmpHeader);
    header.size = 40;
    header.width = image.width;
    header.height = image.height;
    header.planes = 1;
    header.bitsPerPixel = 24;
    header.compression = 0;
    header.imageSize = 0;
    header.XpixelsPerM = 0;
    header.YpixelsPerM = 0;
    header.ColorsUsed = 0;
    header.ImportantColors = 0;
    file.write(reinterpret_cast<const char*>(&header), sizeof(BmpHeader));
    const unsigned char* rawData = reinterpret_cast<const unsigned char*>(image.imageData);
    file.write(reinterpret_cast<const char*>(rawData), image.width * image.height * 3);
    file.close();
}

int main (){
    const std::string filename = "image.bmp";
    dataImg image;
    // Dinamico
    // cout<<"Enter the width: ";cin>>image.width;cout<<"Enter the height";cin>>image.height;
    // Estatico
    image.width = 1000;image.height = 512;image.imageData = new Pixel[1000*512];
    clearAllImage(image);
    writeBmp(filename, image);
    return 0;
}