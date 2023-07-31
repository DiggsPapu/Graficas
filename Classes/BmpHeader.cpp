#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
#include <list>
#include "mathLibrary.h"

class BmpHeader {
    public:
        uint16_t signature;
        uint32_t filesize,reserved,dataoffset,size;
        int32_t width,height;
        uint16_t planes,bitsPerPixel;
        uint32_t compression,imageSize,ColorsUsed,ImportantColors;
        int32_t XpixelsPerM,YpixelsPerM;
};