#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "./Classes/Model.cpp"
int main() {
    std::string filename = "/home/dieggspapu/UVG/Graficas/10680_Dog_v2.obj";
    Model obj(20,30,40,50,60,70,80,90,0,filename);

    // Print out the extracted data for testing purposes
    for (const auto& vertex : obj.getVertices()) {
        std::cout << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }
    for (const auto& texCoord : obj.getTextureCoords()) {
        std::cout << "vt " << texCoord.u << " " << texCoord.v << std::endl;
    }

    for (const auto& normal : obj.getNormals()) {
        std::cout << "vn " << normal.x << " " << normal.y << " " << normal.z << std::endl;
    }

    for (const auto& face : obj.getFaces()) {
        std::cout << "f ";
        for (const auto& faceVertex : face.vertices) {
            std::cout << faceVertex.vertexIndex;
            if (faceVertex.textureIndex != 0) {
                std::cout << "/" << faceVertex.textureIndex;
            }
            if (faceVertex.normalIndex != 0) {
                std::cout << "/" << faceVertex.normalIndex;
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}