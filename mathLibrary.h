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
                identityM.arr[i][j]=1;
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