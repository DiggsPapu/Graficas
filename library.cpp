/**
 * @file texture.cpp
 * @author Diego Alonzo (alo20172@uvg.edu.gt)
 * @brief 
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <threads.h>
struct Pixel {
    char red,green,blue,reserved;    
};
struct BmpHeader {
    char signature[2] = {'B', 'M'};
    uint32_t filesize,reserved,dataoffset;
};
struct BmpInfoHeader {
    uint32_t size,width,height,compression,imageSize,XpixelsPerM,YpixelsPerM,ColorsUsed,ImportantColors;
    uint16_t planes,bitsPerPixel;
};
