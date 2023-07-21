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
void makeLine(std::vector<float>& pos1,std::vector<float>& pos2, int thickness, dataImg image){
    std::vector<float> pos11 = pos1;
    std::vector<float> pos22 = pos2;
    if (pos1[0]>pos2[0])
    {
        printf("Entro al intercambio\n");
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
            printf("Entro al intercambio vertical");
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
            image.imageData[index].blue = currentC.blue;
            image.imageData[index].red = currentC.red;
            image.imageData[index].green = currentC.green;
            index = getPixelIndex(pos1[0]+i, pos1[1], image);
        }
    }
    else if ((float)dy/(float)dx>0)
    {
        if (dx>dy)
        {
            cout<<"dx>dy"<<endl;
            std::vector<float> linearEcuation = getLinearEcuationX(pos1, pos2);
            printf("%f %f\n",linearEcuation[0],linearEcuation[1]);
            int y = pos1[1];
            printf("%f,%f,%f\n",pos1[0], pos1[1],(float)dx);
            for(int x = pos1[0] ; x <= pos2[0] ; x++)
            {
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                float newy = (float)x*linearEcuation[0]+linearEcuation[1];
                // printf("newy:%f y:%f x:%d\n",newy,(float)y, x);
                if(newy-(float)y>=0.5){y++;}
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
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                float newx = (float)y*linearEcuation[0]-linearEcuation[1];
                printf("newx:%f x:%f y:%f\n",newx, (float)x, (float)y);
                if(newx-(float)x>=0.5){x++;}
            } 
        }   
        else
        {
            cout<<"dx=dy"<<endl;
            int y = pos1[1];
            for (int x = pos1[0]; x <= pos2[0]; x++)
            {
                
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
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
            printf("%f  %f\n",linearEcuation[0],linearEcuation[1]);
            int y = pos1[1];
            for(int x = pos1[0] ; x <= pos2[0] ; x++)
            {
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                float newy = (float)x*linearEcuation[0]+linearEcuation[1];
                // printf("newy:%f y:%f x:%d\n",newy,(float)y, x);
                if(newy-(float)y<=-0.1){y--;}
            }            
        }
        else if (abs(dy)>abs(dx))
        {
            cout<<"(neg)dy>dx"<<endl;
            std::vector<float> linearEcuation1 = getLinearEcuationX(pos1, pos2);
            printf("%f  %f\n",linearEcuation1[0],linearEcuation1[1]);
            std::vector<float> linearEcuation = getLinearEcuationY(pos1, pos2);

            printf("%f  %f\n",linearEcuation[0],linearEcuation[1]);
            int x = pos1[0];
            for (int y = pos1[1] ; y >= pos2[1]; y-- )
            {
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                float newx = (float)y*linearEcuation[0]-linearEcuation[1];
                // printf("newx:%f x:%f y:%d\n",newx, (float)x, y);
                if(newx-(float)x>=0.5){x++;}
            } 
        }  
        else
        {
            cout<<"(neg)dx=dy"<<endl;
            int y = pos1[1];
            for (int x = pos1[0]; x <= pos2[0]; x++)
            {
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                // printf("x:%d y:%d\n", x, y);
                y--;
            }
        }    
    }
    pos1 = pos11;
    pos2 = pos22;
}
void makeLineFillingDownTheLine(std::vector<float>& pos1,std::vector<float>& pos2, int thickness, dataImg image,vector<vector<float>> vectors){
    std::vector<float> pos11 = pos1;
    std::vector<float> pos22 = pos2;
    if (pos1[0]>pos2[0])
    {
        printf("Entro al intercambio\n");
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
            printf("Entro al intercambio vertical");
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
            image.imageData[index].blue = currentC.blue;
            image.imageData[index].red = currentC.red;
            image.imageData[index].green = currentC.green;
            index = getPixelIndex(pos1[0]+i, pos1[1], image);
        }
    }
    else if ((float)dy/(float)dx>0)
    {
        if (dx>dy)
        {
            cout<<"dx>dy"<<endl;
            std::vector<float> linearEcuation = getLinearEcuationX(pos1, pos2);
            printf("%f %f\n",linearEcuation[0],linearEcuation[1]);
            int y = pos1[1];
            printf("%f,%f,%f\n",pos1[0], pos1[1],(float)dx);
            for(int x = pos1[0] ; x <= pos2[0] ; x++)
            {
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                float newy = (float)x*linearEcuation[0]+linearEcuation[1];
                // printf("newy:%f y:%f x:%d\n",newy,(float)y, x);
                if(newy-(float)y>=0.5){y++;}
                vector<float> p1= pos1;vector<float> p2= pos2;
                
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
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                float newx = (float)y*linearEcuation[0]-linearEcuation[1];
                printf("newx:%f x:%f y:%f\n",newx, (float)x, (float)y);
                if(newx-(float)x>=0.5){x++;}
            } 
        }   
        else
        {
            cout<<"dx=dy"<<endl;
            int y = pos1[1];
            for (int x = pos1[0]; x <= pos2[0]; x++)
            {
                
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
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
            printf("%f  %f\n",linearEcuation[0],linearEcuation[1]);
            int y = pos1[1];
            for(int x = pos1[0] ; x <= pos2[0] ; x++)
            {
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                float newy = (float)x*linearEcuation[0]+linearEcuation[1];
                // printf("newy:%f y:%f x:%d\n",newy,(float)y, x);
                if(newy-(float)y<=-0.1){y--;}
            }            
        }
        else if (abs(dy)>abs(dx))
        {
            cout<<"(neg)dy>dx"<<endl;
            std::vector<float> linearEcuation1 = getLinearEcuationX(pos1, pos2);
            printf("%f  %f\n",linearEcuation1[0],linearEcuation1[1]);
            std::vector<float> linearEcuation = getLinearEcuationY(pos1, pos2);

            printf("%f  %f\n",linearEcuation[0],linearEcuation[1]);
            int x = pos1[0];
            for (int y = pos1[1] ; y >= pos2[1]; y-- )
            {
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                float newx = (float)y*linearEcuation[0]-linearEcuation[1];
                // printf("newx:%f x:%f y:%d\n",newx, (float)x, y);
                if(newx-(float)x>=0.5){x++;}
            } 
        }  
        else
        {
            cout<<"(neg)dx=dy"<<endl;
            int y = pos1[1];
            for (int x = pos1[0]; x <= pos2[0]; x++)
            {
                image.imageData[index].blue = currentC.blue;
                image.imageData[index].red = currentC.red;
                image.imageData[index].green = currentC.green;
                index = getPixelIndex(x,y,image);
                // printf("x:%d y:%d\n", x, y);
                y--;
            }
        }    
    }
    pos1 = pos11;
    pos2 = pos22;
}
// void fillPolygon(std::vector<vector<float>> vertices)
// {
//     for(1000)
    
    
// }
void makeTriangle(std::vector<float>& pos1,std::vector<float>& pos2,std::vector<float>& pos3, dataImg image){
    currentC.green = 150;
    currentC.blue = 0;
    currentC.red = 0;
    makeLine(pos1,pos2,20,image);
    currentC.green = 0;
    currentC.blue = 150;
    currentC.red = 0;
    makeLine(pos2,pos3,20,image);
    currentC.green = 0;
    currentC.blue = 0;
    currentC.red = 150;
    makeLine(pos1,pos3,20,image);
}
vector<Triangle> primitiveAssemblyTriangle(std::vector<std::vector<float>> & listOfVectors, dataImg image)
{
    Triangle triangle;
    vector<Triangle> triangles{};
    for (int i = 0; i < listOfVectors.size()-3; i+=3)
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
    for (int i = 0; i < triangles.size(); i++)
    {
        makeTriangle(triangles[i].v1,triangles[i].v2,triangles[i].v3, image);
    }   
}
void polygon1(dataImg image){
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
vector<vector<float>> array;
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
for(int i = 1; i < 9; i+=2)
{
    makeLine(array[i],array[i+1],20,image);
}
makeLine(array[array.size()-2],array[array.size()-1],20,image);
makeLine(array[0],array[array.size()-1],20,image);
}
void fillPolygon(vector<vector<float>> vertices, dataImg image)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        /* code */
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
    vector<float> vect{0,500,0};
    vector<float> vect1{100,250,0};
    // makeLine(vect,vect1,20,image);
    vector<float> vect2{100,0,0};
    vector<float> vect3{900,500,0};
    // makeLine(vect2,vect3,20,image);
    vector<float> vect4={900,0,350};
    vector<float> vect5={800,100,200};
    // makeLine(vect4,vect5,20,image);
    vector<float> vect6={200,300,220};
    vector<float> vect7={250,500,470};
    // makeLine(vect6,vect7,20,image);
    vector<float> vect8={300,500,0};
    vector<float> vect9={350,0,0};
    // makeLine(vect8,vect9,20,image);
    vector<float> vect10={400,500,0};
    vector<float> vect11={320,450,0};
    // makeLine(vect10,vect11,20,image);

    vector<float> vect12={0,0,0};
    vector<float> vect13={50,0,0};
    vector<float> vect14={25,40,0};
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
    // printMatrix(getTranslationMatrix(4,3,2));
    // Matrix glMatrix = finalObjectMatrix(getTranslationMatrix(1000/2,512/2,0),rotationMatrix(0,0,0),getScaleMatrix(1,1,1));
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
    // makeLine(vect15,vect16,20,image);
    // vector<float> vect17 = {200,200};
    // vector<float> vect18 = {400,0};
    // makeLine(vect17,vect18,20,image);
    polygon1(image);
    writeBmp(filename, image);
    vector<float> vect15={3,7,5,1};
    // printVector(dotProductMatrixVector(getTranslationMatrix(4,3,2),vect15));
    // printMatrix(getScaleMatrix(20.0,50.0,30.0));
    return 0;
}
// y= 8x+50
// y=2x+50
