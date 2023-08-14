#include "structs.h"
#include "mathLibrary.h"
Vertex vertexShader(Vertex vertice, Matrix finalM, Matrix glMatrix)
{
    Vertex transformedV = dotProductMatrixVertex(dotProductMatrixMatrix(finalM,glMatrix),vertice);
    vertice.x = transformedV.x/transformedV.w;
    vertice.y = transformedV.y/transformedV.w;
    vertice.z = transformedV.z/transformedV.w;
    return vertice;
}
Pixel spikyShader(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight,int i, int j,int width, int height)
{
    Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
    float intensity = dotProductVertex(norm, negativeVertex(dLight));
    Vertex text = activeTexture.getColorFloat(u,v);
    Pixel color = {static_cast<unsigned char>(255.0f-text.x*intensity),static_cast<unsigned char>(255.0f-text.y*intensity),static_cast<unsigned char>(255.0f-text.z*intensity)};;
    // if (intensity>=0.7)
    // {
    //     color = {static_cast<unsigned char>(255.0f-text.z*intensity),static_cast<unsigned char>(255.0f-text.x*intensity),static_cast<unsigned char>(255.0f-text.y*intensity)};
    // }
    // else
    // {
    //     color = {static_cast<unsigned char>(text.x*intensity),static_cast<unsigned char>(text.y*intensity),static_cast<unsigned char>(text.z*intensity)};
    // }
    if(intensity>=0 && intensity<=1)
    {
        return color;
    }
    return Pixel{0,0,0};  
}
Pixel inverseShader(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight)
{
    Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
    float intensity = dotProductVertex(norm, negativeVertex(dLight));
    Vertex text = activeTexture.getColorFloat(u,v);
    Pixel color = {static_cast<unsigned char>(255.0f-text.x*intensity),static_cast<unsigned char>(255.0f-text.y*intensity),static_cast<unsigned char>(255.0f-text.z*intensity)};
    return color;  
}
Pixel fragmentShader(Texture &activeTexture, float u, float v, Vertex normal, Vertex dLight)
{
    return activeTexture.getColor(u,v);
}
Pixel flatShader(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight)
{
    Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
    float intensity = dotProductVertex(norm, negativeVertex(dLight));
    Vertex text = activeTexture.getColorFloat(u,v);
    Pixel color = {static_cast<unsigned char>(text.x*intensity),static_cast<unsigned char>(text.y*intensity),static_cast<unsigned char>(text.z*intensity)};
    if(intensity>=0 && intensity<=1)
    {
        return color;
    }
    return Pixel{0,0,0};    
}
