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
#include "mathLibrary.h"
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
dataImg image;
Pixel currentC = {255,255,255};
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
void makeLine(std::vector<int>& pos1,std::vector<int>& pos2, int thickness, dataImg image){
    int dx = -pos1[0]+pos2[0];int dy = -pos1[1]+pos2[1];
    int index = getPixelIndex(pos1[0], pos1[1], image);
    //Vertical
    if (dx == 0)
    {   
        cout<<"Vertical"<<endl;
        for (int i = 0; i < dy; i++)
        {
            image.imageData[index].blue = 255;
            image.imageData[index].red = 255;
            image.imageData[index].green = 255;
            index += image.width;
        }
    }
    // Horizontal
    else if (dy == 0)
    {   
        cout<<"Horizontal"<<endl;
        for (int i = 0; i < dx; i++)
        {
            image.imageData[index].blue = 255;
            image.imageData[index].red = 255;
            image.imageData[index].green = 255;
            index = getPixelIndex(pos1[0]+i, pos1[1], image);
        }
    }
    else if ((float)dy/(float)dx>0)
    {
        if (dx>=dy)
        {
            cout<<"dx>=dy"<<endl;
            std::vector<float> linearEcuation = getLinearEcuationX(pos1, pos2);
            int y = pos1[1];
            for(int x = pos1[0] ; x < dx ; x++)
            {
                image.imageData[index].blue = 255;
                image.imageData[index].red = 255;
                image.imageData[index].green = 255;
                index = getPixelIndex(x,y,image);
                float newy = (float)x*linearEcuation[0]+linearEcuation[1];
                printf("newy:%f y:%f\n",newy,(float)y);
                if(newy-(float)y>=0.5){y++;}
            }            
        }
        // y=4x+2
        else if (dy>dx)
        {
            cout<<"dx<dy"<<endl;
            std::vector<float> linearEcuation = getLinearEcuationY(pos1, pos2);
            int x = pos1[0];
            for (int y = pos1[1] ; y < dy; y++ )
            {
                image.imageData[index].blue = 255;
                image.imageData[index].red = 255;
                image.imageData[index].green = 255;
                index = getPixelIndex(x,y,image);
                float newx = (float)y*linearEcuation[0]-linearEcuation[1];
                printf("newx:%f x:%f\n",newx, (float)x);
                if(newx-(float)x>=0.5){x++;}
            }
            
        }
        
    }
}
int main (){
    const std::string filename = "image.bmp";
    dataImg image;
    // Dinamico
    // cout<<"Enter the width: ";cin>>image.width;cout<<"Enter the height: ";cin>>image.height;image.imageData = new Pixel[image.width*image.height];
    // Estatico
    image.width = 1000;image.height = 512;image.imageData = new Pixel[1000*512];
    clearAllImage(image);
    // vector<int> vect{10,10};
    // vector<int> vect1{100,10};
    // makeLine(vect,vect1,20,image);
    // vector<int> vect2{10,0};
    // vector<int> vect3{10,100};
    // makeLine(vect2,vect3,20,image);

    vector<int> vect4={7,0};
    vector<int> vect5={40,500};
    makeLine(vect4,vect5,20,image);
    vector<int> vect6={0,7};
    vector<int> vect7={500,40};
    makeLine(vect6,vect7,20,image);
    writeBmp(filename, image);
    return 0;
}
// y= 8x+50
// y=2x+50
