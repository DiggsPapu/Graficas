
#include "../Libraries/structs.h"
#include "../Libraries/mathLibrary.h"

class Camera{
    public:
    Matrix camM, viewM, projectionM, viewPortM, finalM;
    int vWidth, vHeight, posX, posY;
    float aspectRatio, fov, n, f, t, r;
    Camera()
    {
        vWidth = 2340.0f; 
        vHeight = 1720.0f; 
        aspectRatio = vHeight/vWidth; 
        posX = 0.0f; 
        posY = 0.0f; 
        n = 0.1f; 
        f = 1000.0f;
        t = tan(toRadians(1)/2)*n;
        r = t*vWidth/vHeight;
        camM = dotProductMatrixMatrix(getTranslationMatrix(0,0,0),getRotationMatrix(0,0,0));
        Matrix *viewPtr = &viewM;
        vector<vector<float>> temp = matrixToVector(*viewPtr);
        vector<vector<float>> *tempPtr = &temp;
        inverseGaussJordan(matrixToVector(camM),*tempPtr);
        viewM=vectorToMatrix(*tempPtr);
        temp.clear(); 

        projectionM ={{
            {n/r,0,0,0},
            {0,n/t,0,0},
            {0,0,-(f+n)/(f-n),-2.0f*f*n/(f-n)},
            {0,0,-1,0}
        }};
        viewPortM={{
            {vWidth/2.0f,0,0,posX + vWidth/2.0f},
            {0,vHeight/2.0f,0,posY + vHeight/2.0f},
            {0,0,0.5,0.5},
            {0,0,0,1}
        }};
        finalM = dotProductMatrixMatrix(dotProductMatrixMatrix(viewPortM,projectionM),viewM);
    }
    Camera(float width,float height, float x, float y )
    {
        vWidth = width; 
        vHeight = height; 
        aspectRatio = vHeight/vWidth; 
        posX = x; 
        posY = y; 
        n = 0.1f; 
        f = 1000.0f;
        t = tan(toRadians(1)/2)*n;
        r = t*vWidth/vHeight;
        camM = dotProductMatrixMatrix(getTranslationMatrix(0,0,0),getRotationMatrix(0,0,0));
        Matrix *viewPtr = &viewM;
        vector<vector<float>> temp = matrixToVector(*viewPtr);
        vector<vector<float>> *tempPtr = &temp;
        inverseGaussJordan(matrixToVector(camM),*tempPtr);
        viewM=vectorToMatrix(*tempPtr);
        temp.clear(); 

        projectionM ={{
            {n/r,0,0,0},
            {0,n/t,0,0},
            {0,0,-(f+n)/(f-n),-2.0f*f*n/(f-n)},
            {0,0,-1,0}
        }};
        viewPortM={{
            {vWidth/2.0f,0,0,posX + vWidth/2.0f},
            {0,vHeight/2.0f,0,posY + vHeight/2.0f},
            {0,0,0.5,0.5},
            {0,0,0,1}
        }};
        finalM = dotProductMatrixMatrix(dotProductMatrixMatrix(viewPortM,projectionM),viewM);
    }
};