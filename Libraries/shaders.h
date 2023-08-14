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
// Pixel heatMapShader(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight)
// {
//     Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
//     float intensity = dotProductVertex(norm, negativeVertex(dLight));
//     Vertex text = activeTexture.getColorFloat(u,v);
//     Pixel color;
//     if (intensity<=0.25)
//     {
//         // 255,69,0
//         color = {static_cast<unsigned char>(text.x*intensity*1.0f),static_cast<unsigned char>(text.y*intensity*1.0f),static_cast<unsigned char>(text.z*intensity*255.0f)};
//     }
//     else if (0.25<intensity && intensity<=0.5)
//     {
//         color = {static_cast<unsigned char>(text.x*intensity*255),static_cast<unsigned char>(text.y*intensity*1.0f),static_cast<unsigned char>(text.z*intensity*1.0f)};
//     }
//     else if (0.5<intensity && intensity<=0.75)
//     {
//         color = {static_cast<unsigned char>(text.x*intensity*255.0f),static_cast<unsigned char>(text.y*intensity*69.0f),static_cast<unsigned char>(text.z*intensity*1.0f)};
//     }
//     else
//     {
//         color = {static_cast<unsigned char>((text.x*intensity+255.0f)%255.0f),static_cast<unsigned char>(text.y*intensity*255.0f),static_cast<unsigned char>(text.z*intensity*1.0f)};
//     }
//     return color;  
// }
Pixel heatMapShader(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight) {
    Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
    float intensity = dotProductVertex(norm, negativeVertex(dLight));
    Vertex text = activeTexture.getColorFloat(u, v);
    Pixel color;

    // Map intensity to a color range for the heat map
    int r, g, b;
    if (intensity >= 0 && intensity <= 0.33) {
        r = 0;
        g = static_cast<int>(255 * intensity / 0.33);
        b = 255;
    } else if (intensity > 0.33 && intensity <= 0.66) {
        r = 0;
        g = 255;
        b = static_cast<int>(255 - 255 * (intensity - 0.33) / 0.33);
    } else {
        r = static_cast<int>(255 * (intensity - 0.66) / 0.33);
        g = 255;
        b = 0;
    }
    color = { static_cast<unsigned char>(r * text.x), static_cast<unsigned char>(g * text.y), static_cast<unsigned char>(b * text.z) };
    return color;
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
