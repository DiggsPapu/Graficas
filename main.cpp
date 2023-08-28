#include "Classes/Render.cpp"

using namespace std;
int main (){
    const std::string filename = "./Image/project1Night2.bmp";
    Render render(3840,2160,filename);
    render.clearAllImage();
    render.renderBackground("./Textures/Background3.bmp");
    Texture texture3("./Textures/TREX.bmp");
    Texture texture4("./Textures/Raptor.bmp");
    Texture texture5("./Textures/Triceratops_body_BaseColor2K.bmp");
    Texture texture1("./Textures/suntext.bmp");
    Texture texture6("./Textures/Carnotaurus.bmp");
    render.lookAt(0.0f,1.0f,8.0f,0.0f,0.0f,0.0f);
    Model model; 
    model = Model(0.08f,0.01f,0.0f,0.0f,20.0f,0.0f,0.009f,0.009f,0.009f,"./Obj/Carnotaurus/Carnotaurus.obj",texture6);
    model.loadTexture(texture6);
    render.renderModel(&model, 0,2,Vertex{-1.0f,-1.0f,1.0f});      
    model = Model(0.15f,0.0f,0.0f,0.0f,-120.0f,0.0f,0.01f,0.01f,0.01f,"./Obj/Carnotaurus/Carnotaurus.obj",texture6);
    model.loadTexture(texture6);
    render.renderModel(&model, 0,5,Vertex{-1.0f,-1.0f,1.0f});    
    model = Model(0.14f,-0.03f,0.0f,0.0f,30.0f,0.0f,0.02f,0.02f,0.02f,"./Obj/Triceratops.obj",texture5);
    model.loadTexture(texture5);
    render.renderModel(&model, 2,2,Vertex{-1.0f,-1.0f,1.0f});    
    model = Model(-0.05f,-0.05f,0.0f,0.0f,-30.0f,0.0f,0.02f,0.02f,0.02f,"./Obj/Triceratops.obj",texture5);
    model.loadTexture(texture5);
    render.renderModel(&model, 1,6,Vertex{-1.0f,0.0f,0.0f}); 
    model = Model(-0.06f,0.02f,-0.11f,0.0f,0.0f,0.0f,0.008f,0.008f,0.008f,"./Obj/Raptor2/RAPTOR_CAGE_MODEL.OBJ",texture4);
    model.loadTexture(texture4);
    render.renderModel(&model, 0,5,Vertex{0.0f,-1.0f,0.05f});
    model = Model(-0.03f,-0.03f,-0.09f,0.0f,0.0f,0.0f,0.009f,0.009f,0.009f,"./Obj/Raptor2/RAPTOR_CAGE_MODEL.OBJ",texture4);
    model.loadTexture(texture4);
    render.renderModel(&model, 3,3,Vertex{0.0f,-1.0f,0.05f});
    model = Model(-0.02f,0.01f,-0.1f,0.0f,0.0f,0.0f,0.023f,0.023f,0.023f,"./Obj/Trex/Trex.obj",texture3);
    model.loadTexture(texture3);
    render.renderModel(&model, 3,1,Vertex{-1.0f,-1.0f,1.0f});
    model = Model(0.0f,0.01f,-0.1f,0.0f,90.0f,0.0f,0.023f,0.023f,0.023f,"./Obj/Trex/Trex.obj",texture3);
    model.loadTexture(texture3);
    render.renderModel(&model, 2,0,Vertex{-1.0f,-1.0f,1.0f});
    render.writeBmp();
    return 0;
}
