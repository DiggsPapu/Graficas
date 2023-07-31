
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
#include <list>
#include "mathLibrary.h"
#include "library.cpp"

using namespace std;
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