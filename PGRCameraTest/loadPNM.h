#ifndef LOADPNM_H
#define LOADPNM_H

#include <iostream>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;


/**
 * Loads a PFM image stored in little endian and returns the image as an OpenCV Mat.
 * @brief loadPFM
 * @param filePath
 * @return
 */
Mat loadPFM(const string filePath);

/**
 * Saves the image as a PFM file.
 * @brief savePFM
 * @param image
 * @param filePath
 * @return
 */
bool savePFM(const Mat image, const string filePath);













/*
unsigned char *
loadPNM( const char *filename,
	 unsigned int& width, unsigned int &height,
	 unsigned int &numComponents );

void
WritePNM( const char *filename,
	 unsigned int& width, unsigned int &height,
	 unsigned int &numComponents,
   unsigned char* imageData );
   

float *
loadPFM( const char *filename,
	 unsigned int& width, unsigned int &height,
	 unsigned int &numComponents );

void
WritePFM( const char *filename,
	 unsigned int& width, unsigned int &height,
	 unsigned int &numComponents,
   float* imageData );
*/   


#endif


