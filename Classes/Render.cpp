#include <vector>
#include "Model.cpp"
#include "../Libraries/mathLibrary.h"
#include "../Libraries/structs.h"
#include "Camera.cpp"
#include "../Libraries/shaders.h"
// Class
class Render {
    public:
        dataImg image;
        BmpHeader header;
        std::string filename;
        vector<Triangle> primitiveTriangles;
        Vertex directionalLight = {1.0f,0.0f,0.0f,0.0f};
        Texture activeTexture;
        Camera camera = Camera();
        Render(int width,int height,const std::string file)
        {filename = file;image.width = width;image.height = height;image.imageData = new Pixel[width*height];image.zbuffer = new float[width*height];image.backgroundColor.blue=255;image.backgroundColor.red=255;image.backgroundColor.green=255;}
        int getPixelIndex (int x, int y){return y*image.width + x;}
        void clearAllImage(){
            for (int y = 0; y < image.height; y++)
            {
                for (int x = 0; x < image.width; x++)
                {
                    int pixelIndex = getPixelIndex(x,y);
                    image.imageData[pixelIndex].blue = image.backgroundColor.blue;
                    image.imageData[pixelIndex].red = image.backgroundColor.red;
                    image.imageData[pixelIndex].green = image.backgroundColor.green;
                    image.zbuffer[pixelIndex] = INFINITY;
                }
            }
        }
        void writeBmp() {
            std::ofstream file(this->filename, std::ios::binary);
            if(!file){cout<<"File couldnt be opened"<<endl;return;}
            // Header fields initialization
            header.signature = 0x4D42;
            header.filesize = sizeof(BmpHeader) + image.height * image.width * 3;
            header.reserved = 0;
            header.dataoffset = sizeof(BmpHeader);
            header.size = 40;
            header.width = image.width;
            header.height = image.height;
            header.planes = 1;
            header.bitsPerPixel = 24;
            header.compression = 0;
            header.imageSize = 0;
            header.XpixelsPerM = 0;
            header.YpixelsPerM = 0;
            header.ColorsUsed = 0;
            header.ImportantColors = 0;
            file.write(reinterpret_cast<const char*>(&header), sizeof(BmpHeader));
            const unsigned char* rawData = reinterpret_cast<const unsigned char*>(image.imageData);
            file.write(reinterpret_cast<const char*>(rawData), image.width * image.height * 3);
            file.close();
            delete rawData;
        }
        void makeLine(Vertex pos1, Vertex pos2, Pixel currentC){
            Vertex pos11 = pos1;Vertex pos22 = pos2;
            if (pos1.x>pos2.x)
            {
                // printf("Entro al intercambio\n");
                int temp = pos1.x;
                pos1.x = pos2.x;
                pos2.x = temp;
                temp = pos1.y;
                pos1.y = pos2.y;
                pos2.y = temp;
            }
            // printf("\ninicio: x:%f y:%f   fin: x:%f y:%f\n",pos1.x,pos1.y,pos2.x,pos2.y);
            int dx = -pos1.x+pos2.x;int dy = -pos1.y+pos2.y;
            int index = getPixelIndex(pos1.x, pos1.y);
            //Vertical
            if (dx == 0)
            {   
                if (pos1.y>pos2.y)
                {
                    // printf("Entro al intercambio vertical");
                    int temp = pos1.x;
                    pos1.x = pos2.x;
                    pos2.x = temp;
                    temp = pos1.y;
                    pos1.y = pos2.y;
                    pos2.y = temp;
                    index = getPixelIndex(pos1.x, pos1.y);
                }
                // cout<<"Vertical"<<endl;
                for (int y = pos1.y; y <= pos2.y; y++)
                {
                    image.imageData[index].blue = currentC.blue;
                    image.imageData[index].red = currentC.red;
                    image.imageData[index].green = currentC.green;
                    index += image.width;
                }
            }
            // Horizontal
            else if (dy == 0)
            {   
                // cout<<"Horizontal"<<endl;
                for (int i = 0; i < dx; i++)
                {
                    index = getPixelIndex(pos1.x+i, pos1.y);
                    image.imageData[index].blue = currentC.blue;
                    image.imageData[index].red = currentC.red;
                    image.imageData[index].green = currentC.green;
                }
            }
            else if ((float)dy/(float)dx>0)
            {
                if (dx>dy)
                {
                    // cout<<"dx>dy"<<endl;
                    std::vector<float> linearEcuation = getLinearEcuationX(pos1, pos2);
                    // printf("%f %f\n",linearEcuation[0],linearEcuation[1]);
                    int y = pos1.y;
                    // printf("%f,%f,%f\n",pos1.x, pos1.y,(float)dx);
                    for(int x = pos1.x ; x <= pos2.x ; x++)
                    {
                        index = getPixelIndex(x,y);
                        image.imageData[index].blue = currentC.blue;
                        image.imageData[index].red = currentC.red;
                        image.imageData[index].green = currentC.green;
                        float newy = (float)x*linearEcuation[0]+linearEcuation[1];
                        // printf("newy:%f x:%d y:%f\n",newy,(float)y, x);
                        if(newy-(float)y>=0.05)
                        {
                            y++;
                            index = getPixelIndex(x,y);
                            image.imageData[index].blue = currentC.blue;
                            image.imageData[index].red = currentC.red;
                            image.imageData[index].green = currentC.green;
                        }
                    }            
                }
                // y=4x+2
                else if (dy>dx)
                {
                    // cout<<"dy>dx"<<endl;
                    std::vector<float> linearEcuation = getLinearEcuationY(pos1, pos2);
                    int x = pos1.x;
                    for (int y = pos1.y ; y <= pos2.y; y++ )
                    {
                        index = getPixelIndex(x,y);
                        image.imageData[index].blue = currentC.blue;
                        image.imageData[index].red = currentC.red;
                        image.imageData[index].green = currentC.green;
                        float newx = (float)y*linearEcuation[0]-linearEcuation[1];
                        // printf("newx:%f x:%f y:%f\n",newx, (float)x, (float)y);
                        if(newx-(float)x>=0.5)
                        {
                            x++;
                            index = getPixelIndex(x,y);
                            image.imageData[index].blue = currentC.blue;
                            image.imageData[index].red = currentC.red;
                            image.imageData[index].green = currentC.green;
                        }
                    } 
                }   
                else
                {
                    // cout<<"dx=dy"<<endl;
                    int y = pos1.y;
                    for (int x = pos1.x; x <= pos2.x; x++)
                    {
                        index = getPixelIndex(x,y);
                        image.imageData[index].blue = currentC.blue;
                        image.imageData[index].red = currentC.red;
                        image.imageData[index].green = currentC.green;
                        // printf("y:%f x:%d\n",(float)y, x);
                        y++;
                    }
                }    
            }
            else
            {
                if (abs(dx)>abs(dy))
                {
                    // cout<<"(neg)dx>dy"<<endl;
                    std::vector<float> linearEcuation = getLinearEcuationX(pos1, pos2);
                    // printf("%f  %f\n",linearEcuation[0],linearEcuation[1]);
                    int y = pos1.y;
                    for(int x = pos1.x ; x <= pos2.x ; x++)
                    {
                        index = getPixelIndex(x,y);
                        image.imageData[index].blue = currentC.blue;
                        image.imageData[index].red = currentC.red;
                        image.imageData[index].green = currentC.green;
                        float newy = (float)x*linearEcuation[0]+linearEcuation[1];
                        // printf("newy:%f x:%d y:%f\n",newy,x,(float)y);
                        if(newy-(float)y<=-0.05)
                        {
                            y--;
                            index = getPixelIndex(x,y);
                            image.imageData[index].blue = currentC.blue;
                            image.imageData[index].red = currentC.red;
                            image.imageData[index].green = currentC.green;
                        }
                    }            
                }
                else if (abs(dy)>abs(dx))
                {
                    // cout<<"(neg)dy>dx"<<endl;
                    std::vector<float> linearEcuation1 = getLinearEcuationX(pos1, pos2);
                    // printf("%f  %f\n",linearEcuation1[0],linearEcuation1[1]);
                    std::vector<float> linearEcuation = getLinearEcuationY(pos1, pos2);

                    // printf("%f  %f\n",linearEcuation[0],linearEcuation[1]);
                    int x = pos1.x;
                    for (int y = pos1.y ; y >= pos2.y; y-- )
                    {
                        index = getPixelIndex(x,y);
                        image.imageData[index].blue = currentC.blue;
                        image.imageData[index].red = currentC.red;
                        image.imageData[index].green = currentC.green;
                        float newx = (float)y*linearEcuation[0]-linearEcuation[1];
                        // printf("newx:%f x:%f y:%d\n",newx, (float)x, y);
                        if(newx-(float)x>=0.05)
                        {
                            x++;
                            index = getPixelIndex(x,y);
                            image.imageData[index].blue = currentC.blue;
                            image.imageData[index].red = currentC.red;
                            image.imageData[index].green = currentC.green;
                        }
                    } 
                }  
                else
                {
                    // cout<<"(neg)dx=dy"<<endl;
                    int y = pos1.y;
                    for (int x = pos1.x; x <= pos2.x; x++)
                    {
                        index = getPixelIndex(x,y);
                        image.imageData[index].blue = currentC.blue;
                        image.imageData[index].red = currentC.red;
                        image.imageData[index].green = currentC.green;
                        // printf("x:%d y:%d\n", x, y);
                        y--;
                    }
                }    
            }
            pos1 = pos11;
            pos2 = pos22;
        }
        void makeTriangle(Triangle triangle,Pixel currentC){makeLine(triangle.v1,triangle.v2,currentC);makeLine(triangle.v2,triangle.v3,currentC);makeLine(triangle.v1,triangle.v3,currentC);}
        std::vector<Vertex> getX1X2(int x1, int x2,int y)
        {
            Pixel currentcolor = image.backgroundColor;
            std::vector<Vertex> returning ;
            for(int i = x1-1; i <=x2; i++)
            {
                bool switchColor = i!=0&&( (image.imageData[getPixelIndex(i,y)].blue != image.backgroundColor.blue) | (image.imageData[getPixelIndex(i,y)].green != image.backgroundColor.green) | (image.imageData[getPixelIndex(i,y)].red != image.backgroundColor.red));
                bool lastOne = i!=0&&image.imageData[getPixelIndex(i-1,y)].blue == image.backgroundColor.blue && image.imageData[getPixelIndex(i-1,y)].green == image.backgroundColor.green && image.imageData[getPixelIndex(i-1,y)].red == image.backgroundColor.red;
                if (switchColor && lastOne)
                {
                    currentcolor = image.imageData[getPixelIndex(i,y)];
                    printf("pos %f,%f    ",(float)i, (float)y);
                    Vertex pos; pos.x = (float)i;pos.y = (float)y;
                    returning.push_back(pos);
                }
            }cout<<endl;
            return returning;
        }
        void fillPolygon(std::vector<Vertex> vertices,Pixel color)
        {
            float yHighest = vertices[0].y;float yMin = vertices[0].y;float xHighest = vertices[0].x;float xMin = vertices[0].x;
            for (size_t i = 0; i < vertices.size(); i++){if (yHighest<vertices[i].y){yHighest = vertices[i].y;}if (yMin>vertices[i].y){yMin = vertices[i].y;}if (xHighest<vertices[i].x){xHighest = vertices[i].x;}if (xMin>vertices[i].x){xMin = vertices[i].x;}}
            for (int i = yMin+2; i < yHighest; i++)
            {
                if (i!=yMin && i!= yHighest)
                {
                    std::vector<Vertex> xs = getX1X2(xMin,xHighest,i);
                    if(xs.size()>0&&xs.size()%2==0)
                    {
                        for (size_t value = 0; value < xs.size()-1; value+=2)
                        {
                            makeLine(xs[value],xs[value+1],color);
                        }
                    }
                    else if(xs.size()>0 && xs.size()==3)
                    {
                        makeLine(xs[0],xs[xs.size()-1],color);
                    }
                    else if (xs.size()>0 && xs.size()%2!=0)
                    {
                        for (size_t value = 1; value < xs.size()-1; value++)
                        {
                            makeLine(xs[value],xs[value+1],color);
                        }
                    }
                }
            }    
        }
    void renderModel(Model *model, int vShader, int fShader){
        std::vector<Face> faces = model->getFaces();
        std::vector<Vertex> verts = model->getVertices();
        std::vector<TextureCoord> cords = model->getCords();
        std::vector<Vertex> normals = model->getNormals();
        Pixel pixel{90,0,200};Triangle triangle;
        Vertex vert0, vert1, vert2, vert3;
        TextureCoord vt0, vt1, vt2, vt3;
        Vertex normal0, normal1, normal2, normal3;
        Vertex SpikyShaderNormal,spikyV1,spikyV2,spikyV3;
        activeTexture = model->getTexture();
        for (size_t i = 0; i < faces.size(); i++)
        {

            // Coords
            vt0 = model->getCords()[faces[i].vertices[0].textureIndex-1];
            vt1 = model->getCords()[faces[i].vertices[1].textureIndex-1];
            vt2 = model->getCords()[faces[i].vertices[2].textureIndex-1];
            vector<TextureCoord> textureCoords{vt0,vt1,vt2};
            // Normals
            normal0 = model->getNormals()[faces[i].vertices[0].normalIndex-1];
            normal1 = model->getNormals()[faces[i].vertices[1].normalIndex-1];
            normal2 = model->getNormals()[faces[i].vertices[2].normalIndex-1];
            vector<Vertex> normals{normal0, normal1, normal2};
            if (vShader==1)
            {
                // waveDeformationShader
                vert0 = twistDeformationShader(verts[faces[i].vertices[0].vertexIndex-1],camera.finalM,model->glMatrix,100);
                vert1 = twistDeformationShader(verts[faces[i].vertices[1].vertexIndex-1],camera.finalM,model->glMatrix,100);
                vert2 = twistDeformationShader(verts[faces[i].vertices[2].vertexIndex-1],camera.finalM,model->glMatrix,100);
            }
            else
            {
                // VertexShader
                vert0 = vertexShader(verts[faces[i].vertices[0].vertexIndex-1],camera.finalM,model->glMatrix);
                vert1 = vertexShader(verts[faces[i].vertices[1].vertexIndex-1],camera.finalM,model->glMatrix);
                vert2 = vertexShader(verts[faces[i].vertices[2].vertexIndex-1],camera.finalM,model->glMatrix);    
            }
            triangle.v1 = vert0;
            triangle.v2 = vert1;
            triangle.v3 = vert2;
            if(faces[i].vertices.size()>3)
            {
                // textures
                vt3 = model->getCords()[faces[i].vertices[3].textureIndex-1];
                textureCoords.push_back(vt3);
                // normals
                normal3 = model->getNormals()[faces[i].vertices[3].normalIndex-1];
                // Transformation
                if (vShader==1)
                {
                    vert3 = twistDeformationShader(verts[faces[i].vertices[3].vertexIndex-1],camera.finalM,model->glMatrix,100);
                }
                else
                {
                    vert3 = vertexShader(verts[faces[i].vertices[3].vertexIndex-1],camera.finalM,model->glMatrix);
                }
                vector<Vertex> normals2{normal0,normal2,normal3};
                vector<TextureCoord> textureCoords2 = {vt0,vt2,vt3};
                Triangle triangle2 = {vert0,vert3,vert2};
                renderBarycentricTriangle(triangle2,textureCoords2, normals2, fShader);
            }
            renderBarycentricTriangle(triangle,textureCoords, normals, fShader);
        }
    }
    void paintPoint(int x, int y, Pixel color){if (x<=image.width && y<=image.height){image.imageData[getPixelIndex(x,y)].red =color.red;image.imageData[getPixelIndex(x,y)].blue =color.blue;image.imageData[getPixelIndex(x,y)].green =color.green;}}
    void renderBarycentricTriangle(Triangle triangle, vector<TextureCoord> textureCoords, vector<Vertex> normals,int fShader)
    {
        Pixel col{0,0,0};
        vector<Vertex> verts;
        Vertex done;
        verts.push_back(triangle.v1);verts.push_back(triangle.v2);verts.push_back(triangle.v3);
        done = minMaxXY(verts);
        for (int i = done.x; i < done.y; i++)
        {
            for (int j = done.z; j < done.w; j++)
            {
                if (0<=i && i<image.width && 0<=j && j<image.height)
                {
                    Vertex P{(float)i,(float)j,0,0}; Vertex done1;
                    done1 = barycentriCoords(triangle,P);
                    if (0<=done1.x && done1.x<=1 && 0<=done1.y && done1.y<=1 && 0<=done1.z && done1.z<=1)
                    {
                        float z = done1.x*triangle.v1.z+done1.y*triangle.v2.z+done1.z*triangle.v3.z;
                        int index = getPixelIndex(i,j);
                        if (0<=i&& i<image.width && 0<=j&& j<image.height )
                        {
                            float value = image.zbuffer[index];
                            
                            if (z<image.zbuffer[getPixelIndex((int)i,(int)j)])
                            {

                                // fragment shader
                                float u = done1.x*textureCoords[0].u+done1.y*textureCoords[1].u+done1.z*textureCoords[2].u;
                                float v = done1.x*textureCoords[0].v+done1.y*textureCoords[1].v+done1.z*textureCoords[2].v;
                                image.zbuffer[getPixelIndex((int)i,(int)j)] = (float)z;
                                if (fShader ==1 )
                                {
                                    col = flatShader(activeTexture, u, v, normals, Vertex{-1.0f,0.0f,0.0f});
                                }
                                else if (fShader == 2)
                                {
                                    col = inverseShader(activeTexture, u, v, normals, Vertex{1,0,0});
                                }
                                else if (fShader == 3)
                                {
                                    col = heatMapShader(activeTexture, u, v, normals, Vertex{1,0,0});
                                }
                                else if (fShader == 4)
                                {
                                    col.red = 255*done1.x;col.blue = 255*done1.y;col.green = 255*done1.z;
                                }
                                else
                                {
                                    col = fragmentShader(activeTexture, u, v);
                                }
                                paintPoint((int)i,(int)j,col);
                            }
                        }
                    }
                }
            }
        }
    }
    void loadCamera(float tx,float ty, float tz, float rx, float ry, float rz )
    {
        camera = Camera(tx, ty, tz, rx, ry, rz);
    }
    void lookAt(float campX, float campY, float campZ, float eyeX, float eyeY, float eyeZ)
    {
        camera.lookAt(campX, campY, campZ, eyeX, eyeY, eyeZ);
    }
};