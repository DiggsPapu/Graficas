#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
#include <list>
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
struct Vertex {
    float x, y, z, w;
};

struct TextureCoord {
    float u, v, w;
};

struct FaceVertex {
    int vertexIndex;
    int textureIndex;
    int normalIndex;
};

struct Face {
    std::vector<FaceVertex> vertices;
};
struct Triangle{Vertex v1,v2,v3;};
