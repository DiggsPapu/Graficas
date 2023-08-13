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
Pixel fragmentShader(Texture &activeTexture, float u, float v)
{
    return activeTexture.getColor(u,v);
}
Pixel flatShader(Texture &activeTexture, Vertex normal, Vertex dLight)
{
    Vertex intensity = crossProduct(normal, negativeVertex(dLight));
}