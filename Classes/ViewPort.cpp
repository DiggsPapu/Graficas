#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
#include <list>
#include "../Libraries/mathLibrary.h"

class ViewPort {
    public:
    int vWidth, vHeight, posX, posY;float aspectRatio, fov, n, f, t, r;Matrix projectionMatrix, viewPortMatrix; 
    ViewPort()
    {
        vWidth = 2340; vHeight = 1720; aspectRatio = vHeight/vWidth; posX = 0; posY = 0; n = 0.1; f = 1000;
        t = tan(toRadians(1)/2)*n;
        r = t*vWidth/vHeight;
        projectionMatrix = getIdentityMatrix();
        projectionMatrix.arr[0][0] = n/r;
        projectionMatrix.arr[1][1] = n/t;
        projectionMatrix.arr[2][2] = -(f + n)/(f - n);
        projectionMatrix.arr[2][3] = -1;
        projectionMatrix.arr[3][2] = -2*f*n/(f - n);
        projectionMatrix.arr[3][3] = -0;
        viewPortMatrix = getIdentityMatrix();
        viewPortMatrix.arr[0][0] = vWidth/2;viewPortMatrix.arr[0][3] = posX + vWidth/2;
        viewPortMatrix.arr[1][1] = vWidth/2;viewPortMatrix.arr[1][3] = posY + vHeight/2;
        viewPortMatrix.arr[2][2] = 0.5;viewPortMatrix.arr[2][3] = 0.5;
    }
    ViewPort(int width, int height, int posX, int posY, float fov, float n, float f):vWidth(width), vHeight(height), posX(posX),posY(posY), n(n)
    {
        aspectRatio = width/height;
        t = n*tan((toRadians(fov))/2);
        r = t*width/height;
        projectionMatrix = getIdentityMatrix();
        projectionMatrix.arr[0][0] = n/r;
        projectionMatrix.arr[1][1] = n/t;
        projectionMatrix.arr[2][2] = -(f + n)/(f - n);
        projectionMatrix.arr[2][3] = -1;
        projectionMatrix.arr[3][2] = -2*f*n/(f - n);
        projectionMatrix.arr[3][3] = 0;
        viewPortMatrix = getIdentityMatrix();
        viewPortMatrix.arr[0][0] = width/2;
        viewPortMatrix.arr[0][3] = posX + width/2;
        viewPortMatrix.arr[1][1] = height/2;
        viewPortMatrix.arr[1][3] = posY + height/2;
        viewPortMatrix.arr[2][2] = 0.5;
        viewPortMatrix.arr[2][3] = 0.5;
    }
};