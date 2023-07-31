#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
#include <list>
using namespace std;


struct Vertex {
    float x, y, z;
};

struct TextureCoord {
    float u, v;
};

struct Face {
    int vertexIndex[3];
    int textureIndex[3];
};
class Obj {
    private:
        std::vector<Vertex> vertices;
        std::vector<TextureCoord> textureCoords;
        std::vector<Face> faces;
    public:
        Obj(const std::string& filename){
            std::ifstream file(filename);
            if (!file) {
                std::cerr << "Error opening file: " << filename << std::endl;
                return;
            }

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string type;
                iss >> type;
                if (type == "v") {
                    Vertex vertex;
                    iss >> vertex.x >> vertex.y >> vertex.z;
                    vertices.push_back(vertex);
                } else if (type == "vt") {
                    TextureCoord texCoord;
                    iss >> texCoord.u >> texCoord.v;
                    textureCoords.push_back(texCoord);
                } else if (type == "f") {
                    Face face;
                    for (int i = 0; i < 3; ++i) {
                        std::string vertexInfo;
                        iss >> vertexInfo;
                        std::istringstream viss(vertexInfo);
                        viss >> face.vertexIndex[i];
                        if (viss.peek() == '/') {
                            viss.ignore();
                            if (viss.peek() != '/') {
                                viss >> face.textureIndex[i];
                            }
                        }
                    }
                    faces.push_back(face);
                }
            }

            file.close();
        }
        std::vector<Vertex> getVertices(){
            return this->vertices;
        }
        void setVertices(std::vector<Vertex> vertices)
        {
            this->vertices= vertices;
        }
        std::vector<TextureCoord> getTextureCoords(){
            return this->textureCoords;
        }
        void setTextureCoords(std::vector<TextureCoord>coords)
        {
            this->textureCoords= coords;
        } 
        std::vector<Face> getFaces(){
            return this->faces;
        }
        void setFaces(std::vector<Face>Faces)
        {
            this->faces = Faces;
        }
        void parseOBJ(const std::string& filename) {
            std::ifstream file(filename);
            if (!file) {
                std::cerr << "Error opening file: " << filename << std::endl;
                return;
            }

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string type;
                iss >> type;

                if (type == "v") {
                    Vertex vertex;
                    iss >> vertex.x >> vertex.y >> vertex.z;
                    vertices.push_back(vertex);
                } else if (type == "vt") {
                    TextureCoord texCoord;
                    iss >> texCoord.u >> texCoord.v;
                    textureCoords.push_back(texCoord);
                } else if (type == "f") {
                    Face face;
                    for (int i = 0; i < 3; ++i) {
                        std::string vertexInfo;
                        iss >> vertexInfo;
                        std::istringstream viss(vertexInfo);
                        viss >> face.vertexIndex[i];
                        if (viss.peek() == '/') {
                            viss.ignore();
                            if (viss.peek() != '/') {
                                viss >> face.textureIndex[i];
                            }
                        }
                    }
                    faces.push_back(face);
                }
            }
            file.close();
        }
};