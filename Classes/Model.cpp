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
#include "obj.cpp"
#include "../Libraries/mathLibrary.h"
using namespace std;

class Model {
    private:
        Obj object;
        Matrix dimensMatrix;
    public:
        Model(float tX, float tY, float tZ, float rX, float rY, float rZ, float sX, float sY, float sZ, std::string filename)
        :object(filename)
        {dimensMatrix = finalObjectMatrix(getTranslationMatrix(tX,tY,tZ),rotationMatrix(rX, rY, rZ),getScaleMatrix(sX, sY, sZ));}
};