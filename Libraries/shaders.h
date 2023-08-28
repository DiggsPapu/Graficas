#include "structs.h"
#include "mathLibrary.h"
#include <cstdlib> 
Vertex vertexShader(Vertex vertice, Matrix finalM, Matrix glMatrix)
{
    Vertex transformedV = dotProductMatrixVertex(dotProductMatrixMatrix(finalM,glMatrix),vertice);
    vertice.x = transformedV.x/transformedV.w;
    vertice.y = transformedV.y/transformedV.w;
    vertice.z = transformedV.z/transformedV.w;
    return vertice;
}
Vertex fragmentationShader(Vertex vertice, Matrix finalM, Matrix glMatrix, float frequency, float amplitude) {
    Vertex transformedV = dotProductMatrixVertex(dotProductMatrixMatrix(finalM, glMatrix), vertice);
    vertice.x = (transformedV.x+amplitude * sin(frequency)) / transformedV.w;
    vertice.y = (transformedV.y+amplitude * sin(frequency)) / transformedV.w;
    vertice.z = transformedV.z / transformedV.w;
    return vertice;
}
Vertex vertexTransformationVertex(Vertex vertice, Matrix finalM, Matrix glMatrix, bool shouldEnlarge) {
    Vertex transformedV = dotProductMatrixVertex(dotProductMatrixMatrix(finalM, glMatrix), vertice);
    if (shouldEnlarge) {
        float scaleFactor = 2.5f;
        transformedV.x *= scaleFactor;
        transformedV.y *= scaleFactor;
        transformedV.z *= scaleFactor;
    }

    // Apply perspective division
    vertice.x = transformedV.x / transformedV.w;
    vertice.y = transformedV.y / transformedV.w;
    vertice.z = transformedV.z / transformedV.w;

    return vertice;
}
Vertex twistDeformationShader(Vertex vertex, Matrix finalM, Matrix glMatrix, float time)
{
    float twistAngle = 0.5f;
    float centerX = 0.5f;
    float centerY = 0.5f;
    float dx = vertex.x - centerX;
    float dy = vertex.y - centerY;
    float radius = std::sqrt(dx * dx + dy * dy);
    float angle = twistAngle * radius;
    float newX = dx * std::cos(angle) - dy * std::sin(angle) + centerX;
    float newY = dx * std::sin(angle) + dy * std::cos(angle) + centerY;
    vertex.x = newX;
    vertex.y = newY;
    return vertexShader(vertex, finalM, glMatrix);
}
Vertex translateShader(Vertex vertice, Matrix finalM, Matrix glMatrix, float translationAmount) {
    Vertex transformedV = dotProductMatrixVertex(dotProductMatrixMatrix(finalM, glMatrix), vertice);
    transformedV.x += translationAmount; // Translate along the x-axis
    vertice.x = transformedV.x / transformedV.w;
    vertice.y = transformedV.y / transformedV.w;
    vertice.z = transformedV.z / transformedV.w;
    return vertice;
}
Vertex rotateShader(Vertex vertice, Matrix finalM, Matrix glMatrix, float angle) {
    Vertex transformedV = dotProductMatrixVertex(dotProductMatrixMatrix(finalM, glMatrix), vertice);
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    float newX = cosAngle * transformedV.x + sinAngle * transformedV.z;
    float newZ = -sinAngle * transformedV.x + cosAngle * transformedV.z;
    transformedV.x = newX;
    transformedV.z = newZ;
    
    vertice.x = transformedV.x / transformedV.w;
    vertice.y = transformedV.y / transformedV.w;
    vertice.z = transformedV.z / transformedV.w;
    return vertice;
}
Pixel heatMapShader(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight) {
    Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
    float intensity = dotProductVertex(norm, negativeVertex(dLight));
    Vertex text = activeTexture.getColorFloat(u, v);
    Pixel color;
    int r, g, b;
    if (intensity >= 0 && intensity <= 0.33) {
        r = 0.1f*text.x*intensity;
        g = static_cast<int>(255 * intensity / 0.33);
        b = 255;
    } else if (intensity > 0.33 && intensity <= 0.66) {
        r = 0.1f*text.x*intensity;
        g = 255;
        b = static_cast<int>(255 - 255 * (intensity - 0.33) / 0.33);
    } else {
        r = static_cast<int>(255 * (intensity - 0.66) / 0.33);
        g = 255;
        b = 0.1f*text.y*intensity;
    }
    color = { static_cast<unsigned char>(r * text.x), static_cast<unsigned char>(g * text.y), static_cast<unsigned char>(b * text.z) };
    return color;
}

Pixel inverseShader(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight)
{
    Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
    float intensity = dotProductVertex(norm, negativeVertex(dLight))+0.2f;
    Vertex text = activeTexture.getColorFloat(u,v);
    Pixel color = {static_cast<unsigned char>((255.0f-text.x)),static_cast<unsigned char>((255.0f-text.y)),static_cast<unsigned char>((255.0f-text.z))};
    return color;  
}
Pixel fragmentShader(Texture &activeTexture, float u, float v)
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
Pixel inverseNormal(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight){
    float randomValue = static_cast<float>(rand()) / RAND_MAX;
    if (randomValue>=0.66)
    {
        Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
        float intensity = dotProductVertex(norm, negativeVertex(dLight))+0.2f;
        Vertex text = activeTexture.getColorFloat(u,v);
        Pixel color = {static_cast<unsigned char>((255.0f-text.x)*intensity),static_cast<unsigned char>((255.0f-text.y)*intensity),static_cast<unsigned char>((255.0f-text.z)*intensity)};
        return color;  
    }
    else if(randomValue>=0.33)
    {
        return fragmentShader( activeTexture, u, v );
    }
    else
    {
        return flatShader( activeTexture, u, v, normals, dLight);
    }
}
Pixel sepiaToneShader(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight) {
    Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
    float intensity = dotProductVertex(norm, negativeVertex(dLight))+0.6f;
    Vertex text = activeTexture.getColorFloat(u, v);
    
    text.x = (text.x * 0.272f)*intensity + intensity*(text.y* 0.534f) + intensity*(text.z * 0.131f);
    text.y = (text.x * 0.349f)*intensity + intensity*(text.y* 0.686f) + intensity*(text.z * 0.168f);
    text.z = (text.x * 0.393f)*intensity + intensity*(text.y* 0.769f) + intensity*(text.z * 0.189f);
    
    return Pixel {
        static_cast<unsigned char>(text.x),
        static_cast<unsigned char>(text.y),
        static_cast<unsigned char>(text.z)
    };
}
Pixel monochromeShader(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight) {
    Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y};
    float intensity = dotProductVertex(norm, negativeVertex(dLight))+0.9f;
    Vertex text = activeTexture.getColorFloat(u, v);
    
    // Convert to grayscale
    float gray = (text.x + text.y + text.z) / 3.0f;
    
    Pixel color = {
        static_cast<unsigned char>(gray * intensity * 255),
        static_cast<unsigned char>(gray * intensity * 255),
        static_cast<unsigned char>(gray * intensity * 255)
    };
    
    if (intensity >= 0 && intensity <= 1) {
        return color;
    }
    return Pixel{0, 0, 0};
}




Vertex enlargementY(Vertex vertice, Matrix finalM, Matrix glMatrix) {
    // Apply the transformation matrices
    Vertex transformedV = dotProductMatrixVertex(dotProductMatrixMatrix(finalM, glMatrix), vertice);


    // Generate a random number between 0 and 1
    // float randomValue = static_cast<float>(rand()) / RAND_MAX;
    // // Apply scaling randomly to the last vertices
    // if (randomValue < 0.5f) {
    //     // Enlarge in y-direction by a factor of 3
    //     transformedV.y *= 2.0f+randomValue;
    // } else {
    //     // Enlarge in x-direction by a factor of 1.5
    //     transformedV.x *= 1.0f+randomValue;
    // }

    // Apply perspective division
    vertice.x = transformedV.x / transformedV.w;
    vertice.y = transformedV.y / transformedV.w;
    vertice.z = transformedV.z / transformedV.w;

    return vertice;
}

Pixel otherIntensity(Texture &activeTexture, float u, float v, vector<Vertex> normals, Vertex dLight, Vertex vertex) {
    Vertex norm = { u * normals[0].x + v * normals[0].y, u * normals[1].x + v * normals[1].y, u * normals[2].x + v * normals[2].y };
    
    // Calculate the distance from the vertex to the y=0 plane
    float yDistance = std::abs(vertex.y);

    // Calculate intensity based on yDistance
    float intensity = dotProductVertex(norm, negativeVertex(dLight)) * (1.0f - yDistance);

    Vertex text = activeTexture.getColorFloat(u, v);
    Pixel color = {
        static_cast<unsigned char>(std::min(255.0f, text.x * intensity)),
        static_cast<unsigned char>(std::min(255.0f, text.y * intensity)),
        static_cast<unsigned char>(std::min(255.0f, text.z * intensity))
    };

    if (intensity >= 0 && intensity <= 1) {
        return color;
    }

    return Pixel{ 0, 0, 0 };
}

