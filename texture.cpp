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
#include <cstdint> // for specific size integers
#include <fstream> // for file handling
using namespace std;

struct BmpHeader {
	char bitmapSignatureBytes[2] = {'B', 'M'};
	uint32_t sizeOfBitmapFile = 54 + 786432; // total size of bitmap file
	uint32_t reservedBytes = 0;
	uint32_t pixelDataOffset = 54;
} bmpHeader;

struct BmpInfoHeader {
	uint32_t sizeOfThisHeader = 40;
	int32_t width = 1000; // in pixels
	int32_t height = 512; // in pixels
	uint16_t numberOfColorPlanes = 1; // must be 1
	uint16_t colorDepth = 24;
	uint32_t compressionMethod = 0;
	uint32_t rawBitmapDataSize = 0; // generally ignored
	int32_t horizontalResolution = 3780; // in pixel per meter
	int32_t verticalResolution = 3780; // in pixel per meter
	uint32_t colorTableEntries = 0;
	uint32_t importantColors = 0;
} bmpInfoHeader;

struct Pixel {
	uint8_t blue = 0;
	uint8_t green = 0;
	uint8_t red = 255;
} pixel;

int main(int argc, char **argv) {
	ofstream fout("image.bmp", ios::binary);

	fout.write((char *) &bmpHeader, 14);
	fout.write((char *) &bmpInfoHeader, 40);

	// writing pixel data
	size_t numberOfPixels = bmpInfoHeader.width * bmpInfoHeader.height;
	for (int i = 0; i < numberOfPixels; i++) {
		fout.write((char *) &pixel, 4);
	}
	fout.close();

	return 0;
}
