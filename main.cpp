#include "Classes/Render.cpp"

using namespace std;
int main (){
    const std::string filename = "./Image/project1.bmp";
    Render render(2340 ,1720,filename);
    render.clearAllImage();
    // Texture texture1("/home/dieggspapu/UVG/Graficas/Textures/PIEDRA_MURO_PUNTA-SAL_1.bmp");
    // Texture texture1("/home/dieggspapu/UVG/Graficas/Textures/dog.bmp");
    Texture texture1("/home/dieggspapu/UVG/Graficas/Textures/Brick2.bmp");
    Texture texture2("/home/dieggspapu/UVG/Graficas/Textures/ufo_diffuse_glow.bmp");
    Texture texture3("/home/dieggspapu/UVG/Graficas/Textures/TREX.bmp");
    Texture texture4("/home/dieggspapu/UVG/Graficas/Textures/Raptor_SP.bmp");
    Texture texture5("/home/dieggspapu/UVG/Graficas/Textures/Triceratops_body_BaseColor2K.bmp");
    
    // Look at 
    render.lookAt(0.0f,0.0f,10.0f,0.0f,0.0f,0.0f);//Medium shot 
    Model model(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.005f,0.005f,0.005f,"/home/dieggspapu/UVG/Graficas/Obj/Raptor2/RAPTOR_CAGE_MODEL.OBJ",texture1);
    model.loadTexture(texture1);
    render.renderModel(&model, 2,3);
    model = Model(0.05f,0.0f,0.0f,90.0f,80.0f,60.0f,0.005f,0.005f,0.005f,"/home/dieggspapu/UVG/Graficas/Obj/bird.obj",texture1);
    model.loadTexture(texture1);
    render.renderModel(&model, 2,2);
    model = Model(0.04f,0.05f,0.0f,0.0f,0.0f,0.0f,0.001f,0.001f,0.001f,"/home/dieggspapu/UVG/Graficas/Obj/UFO/UFO.obj",texture1);
    model.loadTexture(texture2);
    render.renderModel(&model, 2,3);
    model = Model(-0.04f,0.05f,0.0f,0.0f,-150.0f,0.0f,0.005f,0.005f,0.005f,"/home/dieggspapu/UVG/Graficas/Obj/Triceratops.obj",texture5);
    model.loadTexture(texture5);
    render.renderModel(&model, 2,1);
    model = Model(-0.09f,0.05f,0.0f,0.0f,0.0f,0.0f,0.01f,0.01f,0.01f,"/home/dieggspapu/UVG/Graficas/Obj/Trex/Trex.obj",texture3);
    model.loadTexture(texture3);
    render.renderModel(&model, 2,5);
    model = Model(-0.06f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0007f,0.0007f,0.0007f,"/home/dieggspapu/UVG/Graficas/Obj/Raptor/Raptor.obj",texture4);
    model.loadTexture(texture4);
    render.renderModel(&model, 2,4);
    render.writeBmp();
    return 0;
}