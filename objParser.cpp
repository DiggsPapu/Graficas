#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "obj.cpp"
int main() {
    std::string filename = "/home/dieggspapu/UVG/Graficas/10680_Dog_v2.obj";
    Obj obj(filename);

    // Print out the extracted data for testing purposes
    for (const auto& vertex : obj.getVertices()) {
        std::cout << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    for (const auto& texCoord : obj.getTextureCoords()) {
        std::cout << "vt " << texCoord.u << " " << texCoord.v << std::endl;
    }

    for (const auto& face : obj.getFaces()) {
        std::cout << "f ";
        for (int i = 0; i < 3; ++i) {
            std::cout << face.vertexIndex[i] << "/" << face.textureIndex[i] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
