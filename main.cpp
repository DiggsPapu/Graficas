
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
#include <list>
#include "Classes/Render.cpp"

using namespace std;
int main (){
    const std::string filename = "./Image/image3.bmp";
    Render render(2340 ,1720,filename);
    render.clearAllImage();
    // Vertex pos1;Vertex pos2;Vertex pos3;
    // pos1.x = 30;pos2.x = 100; pos3.x = 100;
    // pos1.y = 500; pos2.y = 500;pos3.y = 300;
    // Pixel pixel;
    // pixel.blue = 100;
    // pixel.red = 200;
    // pixel.green = 150;
    // Triangle triangle{pos1,pos2,pos3};
    // render.renderBarycentricTriangle(triangle);
    // vector<Vertex> vectors;
    // vectors.push_back(pos1);vectors.push_back(pos3);vectors.push_back(pos2);
    // render.makeTriangle(triangle,pixel);
    // render.fillPolygon(vectors,pixel);
    // Texture texture1("/home/dieggspapu/UVG/Graficas/Textures/PIEDRA_MURO_PUNTA-SAL_1.bmp");
    // Texture texture1("/home/dieggspapu/UVG/Graficas/Textures/dog.bmp");
    Texture texture1("/home/dieggspapu/UVG/Graficas/Textures/Brick2.bmp");
    Model model(0,0,-10,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,"/home/dieggspapu/UVG/Graficas/Obj/10680_Dog_v2.obj",texture1);
    model.loadTexture(texture1);
    model.loadViewPort(2340,1720,0,0,60,0.1,1000);
    // model.loadViewPort();
    render.renderModel(&model, false);
    render.writeBmp();
    return 0;
}