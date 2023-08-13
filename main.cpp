#include "Classes/Render.cpp"

using namespace std;
int main (){
    const std::string filename = "./Image/image70.bmp";
    Render render(2340 ,1720,filename);
    render.clearAllImage();
    // Texture texture1("/home/dieggspapu/UVG/Graficas/Textures/PIEDRA_MURO_PUNTA-SAL_1.bmp");
    // Texture texture1("/home/dieggspapu/UVG/Graficas/Textures/dog.bmp");
    Texture texture1("/home/dieggspapu/UVG/Graficas/Textures/Brick2.bmp");
    // Load another camera
    // render.loadCamera(0.01f,0.05f,0.0f,0.0f,0.0f,20.0f);
    // Look at 
    // render.lookAt(0.0f,0.0f,0.0f,0.2f,0.0f,-30.0f);
    Model model(0.2f,0.0f,-30.0f,-90.0f,0.0f,60.0f,0.005f,0.005f,0.005f,"/home/dieggspapu/UVG/Graficas/Obj/10680_Dog_v2.obj",texture1);
    model.loadTexture(texture1);
    render.renderModel(&model, false);
    // model = Model(0,0,-30.0f,90.0f,80.0f,60.0f,0.005f,0.005f,0.005f,"/home/dieggspapu/UVG/Graficas/Obj/10680_Dog_v2.obj",texture1);
    // model.loadTexture(texture1);
    // render.renderModel(&model, false);
    render.writeBmp();
    return 0;
}