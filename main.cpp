
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
    const std::string filename = "image2.bmp";
    Render render(3840,2160,filename);
    render.clearAllImage();
    Vertex pos1;Vertex pos2;Vertex pos3;
    pos1.x = 30;pos2.x = 100; pos3.x = 100;
    pos1.y = 500; pos2.y = 500;pos3.y = 300;
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
    Model model(3840/2,2160/2,100,-50,50,50,50,50,50,"/home/dieggspapu/UVG/Graficas/Obj/10680_Dog_v2.obj");
    render.renderModel(model);
    render.writeBmp();
    return 0;
}