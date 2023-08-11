
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
    Camera(float tx, float ty, float tz, float rx, float ry, float rz)
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
        camM = dotProductMatrixMatrix(getTranslationMatrix(tx,ty,tz),getRotationMatrix(rx,ry,rz));
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
    void lookAt(float campX, float campY, float campZ, float eyeX, float eyeY, float eyeZ)
    {
        Vertex worldUp = {0.0f,1.0f,0.0f};
        Vertex forward = {campX-eyeX, campY-eyeY, campZ-eyeZ};
        // printVertex(forward);
        normalizeVertex(forward);
        // printVertex(forward);
        Vertex right = crossProduct(worldUp, forward);
        // printVertex(right);
        normalizeVertex(right);
        // printVertex(right);
        Vertex up = crossProduct(forward,right);
        normalizeVertex(up);
        // printMatrix(camM);
        camM = {{
            {right.x, up.x, forward.x, campX},
            {right.y, up.y, forward.y, campY},
            {right.z, up.z, forward.z, campZ},
            {0, 0, 0, 1}
        }};
        // printMatrix(camM);
        // printMatrix(viewM);
        Matrix *viewPtr = &viewM;
        vector<vector<float>> temp = matrixToVector(*viewPtr);
        vector<vector<float>> *tempPtr = &temp;
        inverseGaussJordan(matrixToVector(camM),*tempPtr);
        viewM = vectorToMatrix(*tempPtr);
        // printMatrix(viewM);
        temp.clear();
        finalM = dotProductMatrixMatrix(dotProductMatrixMatrix(viewPortM,projectionM),viewM);
    }
};