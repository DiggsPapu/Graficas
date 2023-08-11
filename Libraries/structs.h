#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
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