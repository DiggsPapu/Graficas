#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
// Structs
struct BmpHeader 
{
    uint16_t signature;
    uint32_t filesize,reserved,dataoffset,size;
    int32_t width,height;
    uint16_t planes,bitsPerPixel;
    uint32_t compression,imageSize,ColorsUsed,ImportantColors;
    int32_t XpixelsPerM,YpixelsPerM;
};
#pragma pack(pop)
struct dataImg 
{
    int width,height;
    Pixel* imageData;
    Pixel backgroundColor;
    float* zbuffer;
};
struct Matrix {
    float arr[4][4];
};
struct Vertex {
    float x, y, z, w;
};
struct Triangle{
    Vertex v1, v2, v3;
};
struct TextureCoord {
    float u, v, w;
};

struct FaceVertex {
    int vertexIndex, textureIndex, normalIndex;
};

struct Face {
    std::vector<FaceVertex> vertices;
};