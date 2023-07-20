#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <list>
#include <threads.h>

vector<Triangle> primitiveAssemblyTriangle(vector<vector<int> >& listOfVectors, dataImg image)
{
    vector<Triangle> triangles{};
    for (int i = 0; i < listOfVectors.size(); i+=3)
    {   
        Triangle triangle;
        triangle.v1=listOfVectors[i];
        triangle.v2=listOfVectors[i+1];
        triangle.v3=listOfVectors[i+2];
        triangles[i/3] = triangle;
    }
    return triangles;
}
