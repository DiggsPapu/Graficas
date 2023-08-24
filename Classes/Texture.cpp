#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
#include <list>
#include "../Libraries/structs.h"
using namespace std;
class Texture {
private:
    int width, height;
    Pixel* pixels;
    Vertex* colors;
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
        pixels = new Pixel[height*width];
        colors = new Vertex[height*width];
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                unsigned char r, g, b;
                image.read(reinterpret_cast<char*>(&r), 1);
                image.read(reinterpret_cast<char*>(&g), 1);
                image.read(reinterpret_cast<char*>(&b), 1);
                colors[getPixelIndex(x,y)] = Vertex{
                    static_cast<float>(r),
                    static_cast<float>(g),
                    static_cast<float>(b)
                };
                // Normalize the color values to the range [0, 1] and store in the colors array
                pixels[getPixelIndex(x, y)] = Pixel{
                    static_cast<unsigned char>((float)r),
                    static_cast<unsigned char>((float)g),
                    static_cast<unsigned char>((float)b)
                };
            }
        }
    }
    int getPixelIndex (int x, int y){return y*width + x;}
    Pixel getColor(float u, float v) {
        if (0 <= u && u < 1 && 0 <= v && v < 1) {
            int x = static_cast<int>(u * width);
            int y = static_cast<int>(v * height);
            
            Pixel pixel{
                static_cast<unsigned char>(pixels[getPixelIndex(x, y)].red ),
                static_cast<unsigned char>(pixels[getPixelIndex(x, y)].green),
                static_cast<unsigned char>(pixels[getPixelIndex(x, y)].blue)
            };
            // Convert the normalized colors back to [0, 255]
            return pixel;
        } else {
            return Pixel{0, 0, 0};
        }
    }
    Vertex getColorFloat(float u, float v) {
        Vertex color {0,0,0};
        if (0 <= u && u < 1 && 0 <= v && v < 1) {
            int x = static_cast<int>(u * width);
            int y = static_cast<int>(v * height);
            int index = getPixelIndex(x,y);
            color = colors[index];
            // Convert the normalized colors back to [0, 255]
            return color;
        }
        return color;
    }

};