/**
 * @file mathLibrary.h
 * @author Diego Alonzo (alo20172@uvg.edu.gt)
 * @brief 
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
using namespace std;
struct Matrix {
    float arr[4][4];
};
std::vector <float> getLinearEcuationX(std::vector<float>& pos1,std::vector<float>& pos2)
{
    float m = ((float)pos2[1]-(float)pos1[1])/((float)pos2[0]-(float)pos1[0]);
    float b = (float)pos1[1]-m*(float)pos1[0];
    vector<float> ecuation{m,b};
    return ecuation;
}
std::vector <float> getLinearEcuationY(std::vector<float>& pos1,std::vector<float>& pos2)
{
    float m = ((float)pos2[1]-(float)pos1[1])/((float)pos2[0]-(float)pos1[0]);
    float b = (float)pos1[1]-m*(float)pos1[0];
    vector<float> ecuation{1/m,b/m};
    return ecuation;
}
// Representa un objeto que no se ha rotado, trasladado o se le ha hecho alguna modificacion.
Matrix getIdentityMatrix(){
    Matrix identityM;
    for (int i = 0; i < 4; i++)
    {
        for (int j=0; j<4; j++)
        {
            identityM.arr[i][j]=(float)0;
            if (i == j)
            {
                identityM.arr[i][j] = 1;
            }
        }
    }
    return identityM;    
}
Matrix getTranslationMatrix(float x, float y, float z){
    Matrix identityM = getIdentityMatrix();
    identityM.arr[0][3] = x;identityM.arr[1][3] = y;identityM.arr[2][3] = z;
    return identityM;
}
Matrix getScaleMatrix(float x, float y, float z){
    Matrix identityM = getIdentityMatrix();
    identityM.arr[0][0] = x;identityM.arr[1][1] = y;identityM.arr[2][2] = z;
    return identityM;
}
void printMatrix(Matrix matrix) {
    cout<<endl;
    for (int i = 0 ; i < 4 ; i ++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout <<matrix.arr[i][j]<<"   ";
        }
        cout<<endl;    
    }
}
Matrix dotProductMatrixMatrix(Matrix m1, Matrix m2){
    Matrix result;
    for (int i = 0; i < 4; i++)
    {
        result.arr[i][0] = m1.arr[i][0]*m2.arr[0][0] + m1.arr[i][1]*m2.arr[1][0]+m1.arr[i][2]*m2.arr[2][0]+m1.arr[i][3]*m2.arr[3][0];
        result.arr[i][1] = m1.arr[i][0]*m2.arr[0][1] + m1.arr[i][1]*m2.arr[1][1]+m1.arr[i][2]*m2.arr[2][1]+m1.arr[i][3]*m2.arr[3][1];
        result.arr[i][2] = m1.arr[i][0]*m2.arr[0][2] + m1.arr[i][1]*m2.arr[1][2]+m1.arr[i][2]*m2.arr[2][2]+m1.arr[i][3]*m2.arr[3][2];
        result.arr[i][3] = m1.arr[i][0]*m2.arr[0][3] + m1.arr[i][1]*m2.arr[1][3]+m1.arr[i][2]*m2.arr[2][3]+m1.arr[i][3]*m2.arr[3][3];
    }    
    return result;    
}
float toRadians(float angle){
    return angle*2*M_PI/360;
}
Matrix  rotationMatrix(float angleX, float angleY, float angleZ){
    Matrix x = getIdentityMatrix(); x.arr[1][1] = cos(toRadians(angleX)); x.arr[1][2] = -sin(toRadians(angleX)); x.arr[2][1] = sin(toRadians(angleX)); x.arr[2][2] = cos(toRadians(angleX));
    Matrix y = getIdentityMatrix(); y.arr[0][0] = cos(toRadians(angleY)); y.arr[0][2] = sin(toRadians(angleY)); y.arr[2][0] = -sin(toRadians(angleY)); y.arr[2][2] = cos(toRadians(angleY));
    Matrix z = getIdentityMatrix(); z.arr[0][0] = cos(toRadians(angleZ)); z.arr[0][1] = -sin(toRadians(angleZ)); z.arr[1][0] = sin(toRadians(angleZ)); z.arr[1][1] = cos(toRadians(angleZ)); 
    return dotProductMatrixMatrix(dotProductMatrixMatrix(x,y),z);
}
vector<float> dotProductMatrixVector(Matrix matrix, vector<float> vector){
    float val1 = (matrix.arr[0][0]*vector[0]+matrix.arr[0][1]*vector[1]+matrix.arr[0][2]*vector[2]+matrix.arr[0][3]*1);
    std::vector<float> newV ={(matrix.arr[0][0]*vector[0]+matrix.arr[0][1]*vector[1]+matrix.arr[0][2]*vector[2]+matrix.arr[0][3]*1),(matrix.arr[1][0]*vector[0]+matrix.arr[1][1]*vector[1]+matrix.arr[1][2]*vector[2]+matrix.arr[1][3]*1),(matrix.arr[2][0]*vector[0]+matrix.arr[2][1]*vector[1]+matrix.arr[2][2]*vector[2]+matrix.arr[2][3]*1),(matrix.arr[3][0]*vector[0]+matrix.arr[3][1]*vector[1]+matrix.arr[3][2]*vector[2]+matrix.arr[3][3]*1)};
    return newV;
}
Matrix finalObjectMatrix(Matrix traslation, Matrix rotation, Matrix scale){
    return dotProductMatrixMatrix(dotProductMatrixMatrix(traslation, rotation), scale);
}
void printVector(vector<float> vector){
    for (int i = 0; i < vector.size(); i++)
    {
        cout<<vector[i]<<" ";
    }
    cout<<endl;    
}