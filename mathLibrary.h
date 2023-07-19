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
