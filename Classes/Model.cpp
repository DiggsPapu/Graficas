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
#include "Texture.cpp"
#include "../Libraries/structs.h"
#include "../Libraries/mathLibrary.h"
using namespace std;
class Model {
    private:
    Texture texture;
    public:
        Matrix glMatrix, translationMatrix, rotMatrix, scaleMatrix;
        std::vector<Vertex> vertices;
        std::vector<TextureCoord> textureCoords;
        std::vector<Face> faces;
        std::vector<Vertex> normals;
        Model(){}
        Model(float tX, float tY, float tZ, float rX, float rY, float rZ, float sX, float sY, float sZ, const std::string& filename,Texture texture1)
        {
            int vtMax = 0;
            int vnMax = 0;
            translationMatrix = getTranslationMatrix(tX,tY,tZ);
            rotMatrix = getRotationMatrix(rX, rY, rZ);
            scaleMatrix = getScaleMatrix(sX, sY, sZ);
            glMatrix = finalObjectMatrix(translationMatrix,rotMatrix,scaleMatrix);
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
                    vertex.w = (float)1;
                    iss >> vertex.x >> vertex.y >> vertex.z;
                    std::string temp;
                    iss >>temp;
                    if(temp!="")
                    {
                        vertex.w = std::stof(temp);
                    }
                    vertices.push_back(vertex);
                } else if (type == "vt") {
                    TextureCoord texCoord;
                    texCoord.v = 0;texCoord.w = 0;
                    iss >> texCoord.u;
                    std::string temp;
                    iss >>temp;
                    if(temp!="")
                    {
                        texCoord.v = std::stof(temp);
                    }
                    iss >> temp;
                    if(temp!="")
                    {
                        texCoord.w = std::stof(temp);
                    }                    
                    textureCoords.push_back(texCoord);
                } else if (type == "vn") {
                    Vertex normal;
                    iss >> normal.x >> normal.y >> normal.z;
                    normals.push_back(normal);
                } else if (type == "f") {
                    Face face;
                    while (iss) {
                        std::string vertexInfo;
                        iss >> vertexInfo;
                        if (vertexInfo.empty()) {
                            break;
                        }

                        std::istringstream viss(vertexInfo);
                        FaceVertex faceVertex;

                        viss >> faceVertex.vertexIndex;
                        if (viss.peek() == '/'||viss.peek() != ' ') {
                            // cout<<viss.peek()<<endl;
                            viss.ignore();
                            if (viss.peek() != '/'||viss.peek() != ' ') {
                                viss >> faceVertex.textureIndex;
                                if (vtMax<faceVertex.textureIndex)
                                {
                                    vtMax = faceVertex.textureIndex;
                                }
                            }
                            if (viss.peek() == '/'||viss.peek() != ' ') {
                                viss.ignore();
                                viss >> faceVertex.normalIndex;
                                if (vnMax<faceVertex.textureIndex)
                                {
                                    vnMax = faceVertex.textureIndex;
                                }
                            }
                        }

                        face.vertices.push_back(faceVertex);
                    }
                    faces.push_back(face);
                }
            }
            file.close();
        }
        Texture getTexture(){return this->texture;}
        std::vector<TextureCoord> getCords(){return this->textureCoords;}
        void setCords(std::vector<TextureCoord> cords){
            this->textureCoords = cords;
        }
        std::vector<Vertex> getNormals(){
            return this->normals;
        }
        void setNormals(std::vector<Vertex> normals)
        {
            this->normals = normals;
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
        void loadTexture(Texture texture)
        {
            this->texture = texture;
        }
        void parseOBJSpace(const std::string& filename) {
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
                } else if (type == "vn") {
                    Vertex normal;
                    iss >> normal.x >> normal.y >> normal.z;
                    normals.push_back(normal);
                } else if (type == "f") {
                    Face face;
                    while (iss) {
                        std::string vertexInfo;
                        iss >> vertexInfo;
                        if (vertexInfo.empty()) {
                            break;
                        }

                        std::istringstream viss(vertexInfo);
                        FaceVertex faceVertex;

                        viss >> faceVertex.vertexIndex;
                        if (viss.peek() == ' ') {
                            viss.ignore();
                            if (viss.peek() != ' ') {
                                viss >> faceVertex.textureIndex;
                            }
                            if (viss.peek() == ' ') {
                                viss.ignore();
                                viss >> faceVertex.normalIndex;
                            }
                        }

                        face.vertices.push_back(faceVertex);
                    }
                    faces.push_back(face);
                }
            }
            file.close();
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
                } else if (type == "vn") {
                    Vertex normal;
                    iss >> normal.x >> normal.y >> normal.z;
                    normals.push_back(normal);
                } else if (type == "f") {
                    Face face;
                    while (iss) {
                        std::string vertexInfo;
                        iss >> vertexInfo;
                        if (vertexInfo.empty()) {
                            break;
                        }

                        std::istringstream viss(vertexInfo);
                        FaceVertex faceVertex;

                        viss >> faceVertex.vertexIndex;
                        if (viss.peek() == '/') {
                            viss.ignore();
                            if (viss.peek() != '/') {
                                viss >> faceVertex.textureIndex;
                            }
                            if (viss.peek() == '/') {
                                viss.ignore();
                                viss >> faceVertex.normalIndex;
                            }
                        }

                        face.vertices.push_back(faceVertex);
                    }
                    faces.push_back(face);
                }
            }
            file.close();
        }
};