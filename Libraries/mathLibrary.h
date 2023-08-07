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
struct Matrix {float arr[4][4];};
struct Vertex {float x, y, z, w;};
struct Triangle{Vertex v1;Vertex v2;Vertex v3;};
std::vector<float> getIntersect(std::vector<float>& p1,std::vector<float>& p2)
{
    while(p1!=p2)
    {
        p2[0]--;
        if(p1[1]>p2[1])
        {
            p1[1]--;
        }
        else
        {
            p1[1]++;
        }
    }
    std::vector<float> intersect = {p2[0],p1[1]};
    return intersect;
}
std::vector <float> getLinearEcuationX(std::vector<float>& pos1,std::vector<float>& pos2)
{
    float m = ((float)pos2[1]-(float)pos1[1])/((float)pos2[0]-(float)pos1[0]);
    float b = (float)pos1[1]-m*(float)pos1[0];
    vector<float> ecuation{m,b};
    return ecuation;
}
std::vector<float> getLinearEcuationX(Vertex& pos1,Vertex& pos2)
{
    float m = ((float)pos2.y-(float)pos1.y)/((float)pos2.x-(float)pos1.x);
    float b = (float)pos1.y-m*(float)pos1.x;
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

std::vector <float> getLinearEcuationY(Vertex& pos1,Vertex& pos2)
{
    float m = ((float)pos2.y-(float)pos1.y)/((float)pos2.x-(float)pos1.x);
    float b = (float)pos1.y-m*(float)pos1.x;
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
Vertex dotProductMatrixVertex(Matrix matrix, Vertex vector){Vertex newV;newV.x = (matrix.arr[0][0]*vector.x+matrix.arr[0][1]*vector.y+matrix.arr[0][2]*vector.z+matrix.arr[0][3]*vector.w);newV.y = (matrix.arr[1][0]*vector.x+matrix.arr[1][1]*vector.y+matrix.arr[1][2]*vector.z+matrix.arr[1][3]*vector.w);newV.z = (matrix.arr[2][0]*vector.x+matrix.arr[2][1]*vector.y+matrix.arr[2][2]*vector.z+matrix.arr[2][3]*vector.w);newV.w = (matrix.arr[3][0]*vector.x+matrix.arr[3][1]*vector.y+matrix.arr[3][2]*vector.z+matrix.arr[3][3]*vector.w);return newV;}
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
// Function to calculate the cross product of two vectors
float crossProduct(Vertex v1, Vertex v2) {return (v1.x * v2.y) - (v1.y * v2.x);}
// Function to check if three vertices form a counterclockwise order
bool isCounterClockWise(Vertex A, Vertex B, Vertex C) {
    Vertex AB = {B.x - A.x, B.y - A.y,1,1};
    Vertex BC = {C.x - B.x, C.y - B.y,1,1};
    float cross_product = crossProduct(AB, BC);
    return cross_product > 0.0f;
}
int orderCounterClockWise(Vertex A, Vertex B, Vertex C)
{
    if(isCounterClockWise(A,B,C)){return 0;}
    if(isCounterClockWise(A,C,B)){return 1;}
    if(isCounterClockWise(B,A,C)){return 2;}
    if(isCounterClockWise(B,C,A)){return 3;}
    if(isCounterClockWise(C,A,B)){return 4;}
    return 5;
}
Vertex barycentriCoords(Triangle triangle, Vertex P){
    // Ordering vertex;
    Vertex v1 =triangle.v1; Vertex v2 = triangle.v2; Vertex v3 = triangle.v3;Vertex v4;
    float PCB = abs((v2.y-v3.y)*(P.x-v3.x)+(v3.x-v2.x)*(P.y-v3.y));
    float ACP = abs((v3.y-v1.y)*(P.x-v3.x)+(v1.x-v3.x)*(P.y-v3.y));
    float ABC = abs((v2.y-v3.y)*(v1.x-v3.x)+(v3.x-v2.x)*(v1.y-v3.y));
    v4.x = PCB/ABC;v4.y = ACP/ABC; v4.z = 1-v4.x-v4.y;
    return v4;
}
Vertex minMaxXY(vector<Vertex> verts)
{
    float minX = verts[0].x;float maxX = verts[0].x;
    float minY = verts[0].y;float maxY = verts[0].y;
    for(auto& vec :verts )
    {
        if(minX>vec.x){minX = vec.x;}
        if(maxX<vec.x){maxX=vec.x;}
        if(minY>vec.y){minY = vec.y;}
        if(maxY<vec.y){maxY=vec.y;}
    }
    Vertex done{minX,maxX,minY,maxY};
    return done;
}
double determinant(const std::vector<std::vector<double>>& matrix) {
    int n = matrix.size();
    
    // Base case: for a 1x1 matrix, the determinant is the only element
    if (n == 1) {
        return matrix[0][0];
    }
    
    double det = 0;
    int sign = 1;
    
    for (int i = 0; i < n; ++i) {
        // Create a submatrix excluding the current row and column
        std::vector<std::vector<double>> subMatrix(n - 1, std::vector<double>(n - 1));
        
        for (int row = 1; row < n; ++row) {
            int colIndex = 0;
            for (int col = 0; col < n; ++col) {
                if (col != i) {
                    subMatrix[row - 1][colIndex] = matrix[row][col];
                    colIndex++;
                }
            }
        }
        
        // Recursive call to calculate the determinant of the submatrix
        det += sign * matrix[0][i] * determinant(subMatrix);
        
        // Alternate sign for each term
        sign = -sign;
    }
    
    return det;
}
bool inverseGaussJordan(std::vector<std::vector<float>>& matrix, std::vector<std::vector<float>>& inverse) {
    int n = matrix.size();
    
    // Create an augmented matrix [matrix | identity]
    std::vector<std::vector<float>> augmented(n, std::vector<float>(2 * n));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented[i][j] = matrix[i][j];
        }
        augmented[i][i + n] = 1;
    }
    
    // Perform row operations to transform the left side of the augmented matrix into the identity matrix
    for (int i = 0; i < n; ++i) {
        if (augmented[i][i] == 0) {
            // If the diagonal element is zero, the matrix is singular, and its inverse does not exist
            return false;
        }
        
        // Make the diagonal element 1
        float divisor = augmented[i][i];
        for (int j = 0; j < 2 * n; ++j) {
            augmented[i][j] /= divisor;
        }
        
        // Make all other elements in the current column zero
        for (int k = 0; k < n; ++k) {
            if (k != i) {
                double factor = augmented[k][i];
                for (int j = 0; j < 2 * n; ++j) {
                    augmented[k][j] -= factor * augmented[i][j];
                }
            }
        }
    }
    
    // Copy the right half of the augmented matrix (inverse of the input matrix) to the 'inverse' matrix
    inverse.resize(n, std::vector<float>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverse[i][j] = augmented[i][j + n];
        }
    }
    return true;
}
vector<vector<float>> matrixToVector(const Matrix& matrix){
    vector<vector<float>> newMatrix;
    for (size_t i = 0; i < 4; i++)
    {
        vector<float> row;
        for (size_t j = 0; j < 4; j++)
        {
            row.push_back(matrix.arr[i][j]);
        }
        newMatrix.push_back(row);
    }
    return newMatrix;
}
Matrix vectorToMatrix(const vector<vector<float>> vector){
    Matrix matrix;
    if (vector.size()!=4||vector[0].size()!=4)
    {
        return matrix;
    }
    for (size_t i = 0; i < vector.size(); i++)
    {
        for (size_t j = 0; j < vector[i].size(); j++)
        {
            matrix.arr[i][j] = vector[i][j];
        }
    }
    return matrix;
    
}