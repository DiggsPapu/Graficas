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
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
#include <list>
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
    Pixel backgroundColor;
};
struct Triangle{
    vector<float> v1;
    vector<float> v2;
    vector<float> v3;
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
            image.imageData[pixelIndex].blue = image.backgroundColor.blue;
            image.imageData[pixelIndex].red = image.backgroundColor.red;
            image.imageData[pixelIndex].green = image.backgroundColor.green;
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
void makeLine(std::vector<float>& pos1,std::vector<float>& pos2, int thickness, dataImg image){
    std::vector<float> pos11 = pos1;
    std::vector<float> pos22 = pos2;
    if (pos1[0]>pos2[0])
    {
        // printf("Entro al intercambio\n");
        int temp = pos1[0];
        pos1[0] = pos2[0];
        pos2[0] = temp;
        temp = pos1[1];
        pos1[1] = pos2[1];
        pos2[1] = temp;
    }
    // printf("\ninicio: x:%f y:%f   fin: x:%f y:%f\n",pos1[0],pos1[1],pos2[0],pos2[1]);
    int dx = -pos1[0]+pos2[0];int dy = -pos1[1]+pos2[1];
    int index = getPixelIndex(pos1[0], pos1[1], image);
    //Vertical
    if (dx == 0)
    {   
        if (pos1[1]>pos2[1])
        {
            // printf("Entro al intercambio vertical");
            int temp = pos1[0];
            pos1[0] = pos2[0];
            pos2[0] = temp;
            temp = pos1[1];
            pos1[1] = pos2[1];
            pos2[1] = temp;
            index = getPixelIndex(pos1[0], pos1[1], image);
        }
        // cout<<"Vertical"<<endl;
        for (int y = pos1[1]; y <= pos2[1]; y++)
        {
            image.imageData[index].blue = currentC.blue;
            image.imageData[index].red = currentC.red;
            image.imageData[index].green = currentC.green;
            index += image.width;
        }
    }
    // Horizontal
    else if (dy == 0)
    {   
        // cout<<"Horizontal"<<endl;
        for (int i = 0; i < dx; i++)
        {
            index = getPixelIndex(pos1[0]+i, pos1[1], image);
            image.imageData[index].blue = currentC.blue;
            image.imageData[index].red = currentC.red;
            image.imageData[index].green = currentC.green;
        }
    }
    else if ((float)dy/(float)dx>0)
    {
        if (dx>dy)
        {
            cout<<"dx>dy"<<endl;
            std::vector<float> linearEcuation = getLinearEcuationX(pos1, pos2);
            // printf("%f %f\n",linearEcuation[0],linearEcuation[1]);
            int y = pos1[1];
            // printf("%f,%f,%f\n",pos1[0], pos1[1],(float)dx);
            for(int x = pos1[0] ; x <= pos2[0] ; x++)
            {
                index = getPixelIndex(x,y,image);
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                float newy = (float)x*linearEcuation[0]+linearEcuation[1];
                // printf("newy:%f x:%d y:%f\n",newy,(float)y, x);
                if(newy-(float)y>=0.05)
                {
                    y++;
                    index = getPixelIndex(x,y,image);
                    image.imageData[index].blue = currentC.blue;
                    image.imageData[index].red = currentC.red;
                    image.imageData[index].green = currentC.green;
                }
            }            
        }
        // y=4x+2
        else if (dy>dx)
        {
            cout<<"dy>dx"<<endl;
            std::vector<float> linearEcuation = getLinearEcuationY(pos1, pos2);
            int x = pos1[0];
            for (int y = pos1[1] ; y <= pos2[1]; y++ )
            {
                index = getPixelIndex(x,y,image);
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                float newx = (float)y*linearEcuation[0]-linearEcuation[1];
                printf("newx:%f x:%f y:%f\n",newx, (float)x, (float)y);
                if(newx-(float)x>=0.5)
                {
                    x++;
                    index = getPixelIndex(x,y,image);
                    image.imageData[index].blue = currentC.blue;
                    image.imageData[index].red = currentC.red;
                    image.imageData[index].green = currentC.green;
                }
            } 
        }   
        else
        {
            // cout<<"dx=dy"<<endl;
            int y = pos1[1];
            for (int x = pos1[0]; x <= pos2[0]; x++)
            {
                index = getPixelIndex(x,y,image);
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                // printf("y:%f x:%d\n",(float)y, x);
                y++;
            }
        }    
    }
    else
    {
        if (abs(dx)>abs(dy))
        {
            cout<<"(neg)dx>dy"<<endl;
            std::vector<float> linearEcuation = getLinearEcuationX(pos1, pos2);
            // printf("%f  %f\n",linearEcuation[0],linearEcuation[1]);
            int y = pos1[1];
            for(int x = pos1[0] ; x <= pos2[0] ; x++)
            {
                index = getPixelIndex(x,y,image);
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                float newy = (float)x*linearEcuation[0]+linearEcuation[1];
                printf("newy:%f x:%d y:%f\n",newy,x,(float)y);
                if(newy-(float)y<=-0.05)
                {
                    y--;
                    index = getPixelIndex(x,y,image);
                    image.imageData[index].blue = currentC.blue;
                    image.imageData[index].red = currentC.red;
                    image.imageData[index].green = currentC.green;
                }
            }            
        }
        else if (abs(dy)>abs(dx))
        {
            // cout<<"(neg)dy>dx"<<endl;
            std::vector<float> linearEcuation1 = getLinearEcuationX(pos1, pos2);
            // printf("%f  %f\n",linearEcuation1[0],linearEcuation1[1]);
            std::vector<float> linearEcuation = getLinearEcuationY(pos1, pos2);

            // printf("%f  %f\n",linearEcuation[0],linearEcuation[1]);
            int x = pos1[0];
            for (int y = pos1[1] ; y >= pos2[1]; y-- )
            {
                index = getPixelIndex(x,y,image);
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                float newx = (float)y*linearEcuation[0]-linearEcuation[1];
                // printf("newx:%f x:%f y:%d\n",newx, (float)x, y);
                if(newx-(float)x>=0.05)
                {
                    x++;
                    index = getPixelIndex(x,y,image);
                    image.imageData[index].blue = currentC.blue;
                    image.imageData[index].red = currentC.red;
                    image.imageData[index].green = currentC.green;
                }
            } 
        }  
        else
        {
            // cout<<"(neg)dx=dy"<<endl;
            int y = pos1[1];
            for (int x = pos1[0]; x <= pos2[0]; x++)
            {
                index = getPixelIndex(x,y,image);
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                // printf("x:%d y:%d\n", x, y);
                y--;
            }
        }    
    }
    pos1 = pos11;
    pos2 = pos22;
}
std::vector<vector<float>> getX1X2(int x1, int x2,int y,dataImg image)
{
    Pixel currentcolor = image.backgroundColor;
    std::vector<vector<float>> returning ;
    for(int i = x1-1; i <=x2; i++)
    {
        bool switchColor = i!=0&&( (image.imageData[getPixelIndex(i,y,image)].blue != image.backgroundColor.blue) | (image.imageData[getPixelIndex(i,y,image)].green != image.backgroundColor.green) | (image.imageData[getPixelIndex(i,y,image)].red != image.backgroundColor.red));
        bool lastOne = i!=0&&image.imageData[getPixelIndex(i-1,y,image)].blue == image.backgroundColor.blue && image.imageData[getPixelIndex(i-1,y,image)].green == image.backgroundColor.green && image.imageData[getPixelIndex(i-1,y,image)].red == image.backgroundColor.red;
        if (switchColor && lastOne)
        {
            currentcolor = image.imageData[getPixelIndex(i,y,image)];
            printf("pos %f,%f    ",(float)i, (float)y);
            std::vector<float>pos{(float)i,(float)y};
            returning.push_back(pos);
        }
    }cout<<endl;
    return returning;
}
void fillDraw(std::vector<vector<float>> vertices)
{
    float yHighest = vertices[0][1];float yMin = vertices[0][1];float xHighest = vertices[0][0];float xMin = vertices[0][0];
    for (size_t i = 0; i < vertices.size(); i++){if (yHighest<vertices[i][1]){yHighest = vertices[i][1];}if (yMin>vertices[i][1]){yMin = vertices[i][1];}if (xHighest<vertices[i][0]){xHighest = vertices[i][1];}if (xMin>vertices[i][0]){xMin = vertices[i][0];}}
}
void fillPolygon(std::vector<vector<float>> vertices,dataImg image)
{
    float yHighest = vertices[0][1];
    float yMin = vertices[0][1];
    float xHighest = vertices[0][0];
    float xMin = vertices[0][0];
    for (size_t i = 0; i < vertices.size(); i++){if (yHighest<vertices[i][1]){yHighest = vertices[i][1];}if (yMin>vertices[i][1]){yMin = vertices[i][1];}if (xHighest<vertices[i][0]){xHighest = vertices[i][0];}if (xMin>vertices[i][0]){xMin = vertices[i][0];}}
    for (int i = yMin+2; i < yHighest; i++)
    {
        if (i!=yMin && i!= yHighest)
        {
            std::vector<vector<float>> xs = getX1X2(xMin,xHighest,i,image);
            if(xs.size()>0&&xs.size()%2==0)
            {
                for (size_t value = 0; value < xs.size()-1; value+=2)
                {
                    makeLine(xs[value],xs[value+1],20,image);
                }
            }
            else if(xs.size()>0 && xs.size()==3)
            {
                makeLine(xs[0],xs[xs.size()-1],20,image);
            }
            else if (xs.size()>0 && xs.size()%2!=0)
            {
                for (size_t value = 1; value < xs.size()-1; value++)
                {
                    makeLine(xs[value],xs[value+1],20,image);
                }
            }
        }
    }    
}
void makeTriangle(std::vector<float>& pos1,std::vector<float>& pos2,std::vector<float>& pos3, dataImg image){
    makeLine(pos1,pos2,20,image);
    makeLine(pos2,pos3,20,image);
    makeLine(pos1,pos3,20,image);
}
vector<Triangle> primitiveAssemblyTriangle(std::vector<std::vector<float>> & listOfVectors, dataImg image)
{
    Triangle triangle;
    vector<Triangle> triangles{};
    for (size_t i = 0; i < listOfVectors.size()-3; i+=3)
    {
        triangle.v1 = listOfVectors[i];
        triangle.v2 = listOfVectors[i+1];
        triangle.v3 = listOfVectors[i+2];
        triangles.push_back(triangle);
    }
    triangle.v1 = listOfVectors[listOfVectors.size()-3];
    triangle.v2 = listOfVectors[listOfVectors.size()-2];
    triangle.v3 = listOfVectors[listOfVectors.size()-1];
    triangles.push_back(triangle);
    return triangles;
}
vector<float> vertexShader(std::vector<float> vertice, Matrix modelMatrix){
    vertice.push_back(1);
    std::vector<float> transformedV = dotProductMatrixVector(modelMatrix, vertice);
    vertice[0] = transformedV[0]/transformedV[3];
    vertice[1] = transformedV[1]/transformedV[3];
    vertice[2] = transformedV[2]/transformedV[3];
    return vertice;
}
void makePrimitiveTriangle(vector<Triangle> triangles, dataImg image) {
    for (size_t i = 0; i < triangles.size(); i++)
    {
        makeTriangle(triangles[i].v1,triangles[i].v2,triangles[i].v3, image);
    }   
}
vector<vector<float>> polygon1(dataImg image,vector<vector<float>> array){
    currentC.blue = 10;currentC.red = 10;currentC.green = 10;
    vector<float> vect1{165, 380}; 
    vector<float> vect2{185, 360};
    vector<float> vect3{180, 330};
    vector<float> vect4{207, 345};
    vector<float> vect5{233, 330};
    vector<float> vect6{230, 360};
    vector<float> vect7{250, 380};
    vector<float> vect8{220, 385};
    vector<float> vect9{205, 410};
    vector<float> vect10{193, 383};
    array.push_back(vect1);
    array.push_back(vect2);
    array.push_back(vect3);
    array.push_back(vect4);
    array.push_back(vect5);
    array.push_back(vect6);
    array.push_back(vect7);
    array.push_back(vect8);
    array.push_back(vect9);
    array.push_back(vect10);
    for (int i = 0; i < 10; i+=2)
    {
        makeLine(array[i],array[i+1],20,image);
    }
    for(int i = 1; i < 8; i+=2)
    {
        makeLine(array[i],array[i+1],20,image);
    }
    makeLine(array[0],array[1],20,image);
    makeLine(array[1],array[2],20,image);
    makeLine(array[2],array[3],20,image);
    makeLine(array[0],array[array.size()-1],20,image);
    fillPolygon(array,image);
    currentC.blue = 255;currentC.red = 255;currentC.green = 255;
    return array;
}
vector<vector<float>> polygon2(dataImg image,vector<vector<float>> array)
{
    currentC.blue = 0;currentC.red = 0;currentC.green = 0;
    // int size = array.size();
    // dataImg image1;
    // image1.width = 1000;image1.height = 512;image1.imageData = new Pixel[1000*512];
    // image1.backgroundColor.blue=255;image1.backgroundColor.red=250;image1.backgroundColor.green=90;
    // clearAllImage(image);
    vector<float> vect1={321, 335};
    vector<float> vect2={288, 286}; 
    vector<float> vect3={339, 251}; 
    vector<float> vect4={374, 302};
    array.push_back(vect1);
    array.push_back(vect2);
    array.push_back(vect3);
    array.push_back(vect4);
    // makeLine(array[size+0],array[size+1],20,image);
    // makeLine(array[size+1],array[size+2],20,image);
    // makeLine(array[size+2],array[size+3],20,image);
    // makeLine(array[size+0],array[size+3],20,image);
    vector<vector<float>> array1;
    array1.push_back(vect1);
    array1.push_back(vect2);
    array1.push_back(vect3);
    array1.push_back(vect4);

    makeLine(array1[0],array1[1],20,image);
    makeLine(array1[1],array1[2],20,image);
    makeLine(array1[2],array1[3],20,image);
    makeLine(array1[0],array1[3],20,image);
    fillPolygon(array1,image);
    currentC.blue = 255;currentC.red = 255;currentC.green = 255;
    return array;
}
vector<vector<float>> polygon3(dataImg image,vector<vector<float>> array)
{
    vector<float> vect1{377, 249};
    vector<float> vect2{411, 197};
    vector<float> vect3{436, 249};
    makeTriangle(vect1,vect2,vect3,image);
    vector<vector<float>> array1;
    array1.push_back(vect1);
    array1.push_back(vect2);
    array1.push_back(vect3);
    array.push_back(vect1);
    array.push_back(vect2);
    array.push_back(vect3);
    fillPolygon(array1, image);
    currentC.blue = 255;currentC.red = 255;currentC.green = 255;
    return array;
}
vector<vector<float>> polygon5(dataImg image,vector<vector<float>> array)
{
    int size = array.size();
    vector<float> vect1{682, 175}; 
    vector<float> vect2{708, 120}; 
    vector<float> vect3{735, 148};
    vector<float> vect4{739, 170};
    array.push_back(vect1);
    array.push_back(vect2);
    array.push_back(vect3);
    array.push_back(vect4);
    makeLine(array[size+0],array[size+1],20,image);
    makeLine(array[size+1],array[size+2],20,image);
    makeLine(array[size+2],array[size+3],20,image);
    makeLine(array[size+0],array[size+3],20,image);
    // fillPolygon(array, image);
    return array;
}
vector<vector<float>> polygon4(dataImg image,vector<vector<float>> array)
{
    // int size = array.size();
    vector<vector<float>> array1;
    vector<float> vect1{413, 177}; 
    vector<float> vect2{448, 159}; 
    vector<float> vect3{502, 88}; 
    vector<float> vect4{553, 53}; 
    vector<float> vect5{535, 36}; 
    vector<float> vect6{676, 37};
    vector<float> vect7{660, 52};
    vector<float> vect8{750, 145};
    vector<float> vect9{761, 179}; 
    vector<float> vect10{672, 192};
    vector<float> vect11{659, 214};
    vector<float> vect12{615, 214};
    vector<float> vect13{632, 230};
    vector<float> vect14{580, 230};
    vector<float> vect15{597, 215};
    vector<float> vect16{552, 214};
    vector<float> vect17{517, 144};
    vector<float> vect18{466, 180};
    array.push_back(vect1);
    array.push_back(vect2);
    array.push_back(vect3);
    array.push_back(vect4);
    array.push_back(vect5);
    array.push_back(vect6);
    array.push_back(vect7);
    array.push_back(vect8);
    array.push_back(vect9);
    array.push_back(vect10);
    array.push_back(vect11);
    array.push_back(vect12);
    array.push_back(vect13);
    array.push_back(vect14);
    array.push_back(vect15);
    array.push_back(vect16);
    array.push_back(vect17);
    array.push_back(vect18);
    array1.push_back(vect1);
    array1.push_back(vect2);
    array1.push_back(vect3);
    array1.push_back(vect4);
    array1.push_back(vect5);
    array1.push_back(vect6);
    array1.push_back(vect7);
    array1.push_back(vect8);
    array1.push_back(vect9);
    array1.push_back(vect10);
    array1.push_back(vect11);
    array1.push_back(vect12);
    array1.push_back(vect13);
    array1.push_back(vect14);
    array1.push_back(vect15);
    array1.push_back(vect16);
    array1.push_back(vect17);
    array1.push_back(vect18);
    // makeLine(array[size+0],array[size+1],20,image);
    // makeLine(array[size+1],array[size+2],20,image);
    // makeLine(array[size+2],array[size+3],20,image);
    // makeLine(array[size+3],array[size+4],20,image);
    // makeLine(array[size+4],array[size+5],20,image);
    // makeLine(array[size+5],array[size+6],20,image);
    // makeLine(array[size+6],array[size+7],20,image);
    // makeLine(array[size+7],array[size+8],20,image);
    // makeLine(array[size+8],array[size+9],20,image);
    // makeLine(array[size+9],array[size+10],20,image);
    // makeLine(array[size+10],array[size+11],20,image);
    // makeLine(array[size+11],array[size+12],20,image);
    // makeLine(array[size+12],array[size+13],20,image);
    // makeLine(array[size+13],array[size+14],20,image);
    // makeLine(array[size+14],array[size+15],20,image);
    // makeLine(array[size+15],array[size+16],20,image);
    // makeLine(array[size+16],array[size+17],20,image);
    // makeLine(array[size+0],array[size+17],20,image);
    makeLine(array1[0],array1[1],20,image);
    makeLine(array1[1],array1[2],20,image);
    makeLine(array1[2],array1[3],20,image);
    makeLine(array1[3],array1[4],20,image);
    makeLine(array1[4],array1[5],20,image);
    makeLine(array1[5],array1[6],20,image);
    makeLine(array1[6],array1[7],20,image);
    makeLine(array1[7],array1[8],20,image);
    makeLine(array1[8],array1[9],20,image);
    makeLine(array1[9],array1[10],20,image);
    makeLine(array1[10],array1[11],20,image);
    makeLine(array1[11],array1[12],20,image);
    makeLine(array1[12],array1[13],20,image);
    makeLine(array1[13],array1[14],20,image);
    makeLine(array1[14],array1[15],20,image);
    makeLine(array1[15],array1[16],20,image);
    makeLine(array1[16],array1[17],20,image);
    makeLine(array1[0],array1[17],20,image);
    array1 = polygon5(image,array1);
    fillPolygon(array1, image);
    return array;
}
int main (){
    const std::string filename = "image.bmp";
    dataImg image;
    // Dinamico
    // cout<<"Enter the width: ";cin>>image.width;cout<<"Enter the height: ";cin>>image.height;image.imageData = new Pixel[image.width*image.height];
    // Estatico
    image.width = 1000;image.height = 512;image.imageData = new Pixel[1000*512];
    image.backgroundColor.blue=255;image.backgroundColor.red=250;image.backgroundColor.green=90;
    clearAllImage(image);
    vector<float> vect{0,500,0};
    vector<float> vect1{100,250,0};
    vector<float> vect2{100,0,0};
    vector<float> vect3{900,500,0};
    vector<float> vect4={900,0,350};
    vector<float> vect5={800,100,200};
    vector<float> vect6={200,300,220};
    vector<float> vect7={250,500,470};
    vector<float> vect8={300,500,0};
    vector<float> vect9={350,0,0};
    vector<float> vect10={400,500,0};
    vector<float> vect11={320,450,0};
    vector<float> vect12={0,0,0};
    vector<float> vect13={50,0,0};
    vector<float> vect14={25,40,0};
    // makeLine(vect,vect1,20,image);
    // makeLine(vect2,vect3,20,image);
    // makeLine(vect4,vect5,20,image);
    // makeLine(vect6,vect7,20,image);
    // makeLine(vect8,vect9,20,image);
    // makeLine(vect10,vect11,20,image);

    // makeTriangle(vect12,vect13,vect14,image);
    // makeTriangle(vect3,vect7,vect2,image);
    // makeTriangle(vect10,vect13,vect14,image);
    // makeTriangle(vect1,vect14,vect7,image);
    std::vector<std::vector<float>> vertices;
    vertices.push_back(vect);
    vertices.push_back(vect1);
    vertices.push_back(vect2);
    vertices.push_back(vect3);
    vertices.push_back(vect4);
    vertices.push_back(vect5);
    vertices.push_back(vect6);
    vertices.push_back(vect7);
    vertices.push_back(vect8);
    vertices.push_back(vect9);
    vertices.push_back(vect10);
    vertices.push_back(vect11);
    vertices.push_back(vect12);
    vertices.push_back(vect13);
    vertices.push_back(vect14);
    // // printMatrix(getTranslationMatrix(4,3,2));

    // Matrix glMatrix = finalObjectMatrix(getTranslationMatrix(1000/2,512/2,0),rotationMatrix(180,0,0),getScaleMatrix(1,1,1));
    // std::vector<std::vector<float>> vertexShaderVertices;
    // vertexShaderVertices.push_back(vertexShader(vect14,glMatrix));
    // vertexShaderVertices.push_back(vertexShader(vect12,glMatrix));
    // vertexShaderVertices.push_back(vertexShader(vect13,glMatrix));
    // makePrimitiveTriangle(primitiveAssemblyTriangle(vertexShaderVertices,image),image);
    // for (int i = 0; i < vertices.size(); i++)
    // {
    //     printf("Init: %f,%f,%f",vertices[i][0],vertices[i][1],vertices[i][2]);
    //     vertexShaderVertices.push_back(vertexShader(vertices[i],glMatrix));
    //     printf(" end: %f,%f,%f\n",vertexShaderVertices[i][0],vertexShaderVertices[i][1],vertexShaderVertices[i][2]);
    // }
    // makePrimitiveTriangle(primitiveAssemblyTriangle(vertices,image), image);
    // printMatrix(getScaleMatrix(3,2,5));
    // printMatrix(dotProductMatrixMatrix(getTranslationMatrix(4,3,2), getScaleMatrix(3,2,5)));
    // printMatrix(rotationMatrix(80,60,20));
    // vector<float> vect17 = {200,200};
    // vector<float> vect18 = {400,0};
    vector<vector<float>> array;
    array = polygon1(image, array);
    array = polygon2(image,array);
    array = polygon3(image,array);
    array = polygon4(image,array);
    // fillPolygon(array, image);
    writeBmp(filename, image);
    vector<float> vect15={3,7,5,1};
    // printVector(dotProductMatrixVector(getTranslationMatrix(4,3,2),vect15));
    // printMatrix(getScaleMatrix(20.0,50.0,30.0));
    return 0;
}
// y= 8x+50
// y=2x+50
