#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
#include <list>
struct Pixel {unsigned char red, green, blue;};
#pragma pack(push, 1)
class Texture {
private:
    int width;
    int height;
    Pixel* colors;
    std::vector<std::vector<std::vector<float>>> pixels;
public:
    Texture(){}
    Texture(const std::string& filename) {
        std::ifstream image(filename, std::ios::binary);
        if (!image) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }

        image.seekg(10);
        int headerSize;
        image.read(reinterpret_cast<char*>(&headerSize), 4);

        image.seekg(18);
        image.read(reinterpret_cast<char*>(&width), 4);
        image.read(reinterpret_cast<char*>(&height), 4);

        image.seekg(headerSize, std::ios::beg);
        colors = new Pixel[height*width];
        pixels.resize(height, std::vector<std::vector<float>>(width, std::vector<float>(3)));

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                unsigned char r, g, b;
                image.read(reinterpret_cast<char*>(&b), 1);
                image.read(reinterpret_cast<char*>(&g), 1);
                image.read(reinterpret_cast<char*>(&r), 1);
                colors[getPixelIndex(x,y)]= Pixel{(unsigned char)((float)r*255),(unsigned char)((float)g*255),(unsigned char)((float)b*255)};
            }
        }
    }
    int getPixelIndex (int x, int y){return y*width + x;}
    Pixel getColor(float u,float v)
    {
        if (0 <= u && u < 1 && 0 <= v && v < 1) {
            int x = static_cast<int>(u * width);
            int y = static_cast<int>(v * height);
            return colors[getPixelIndex(x,y)];
        }
        else {
            return Pixel {0,0,0};
        }
    }
};