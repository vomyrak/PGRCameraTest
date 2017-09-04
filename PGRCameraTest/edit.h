#ifndef EDIT_H
#define EDIT_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <Windows.h>

using namespace std;
using namespace cv;


Mat linear_edit( Mat& img_orig, double alpha );
//int keyboard_entry( string in_path );
float interpolate(float startValue, float endValue, float fraction);
Vec3f interpolate_col(Vec3f col_a, Vec3f col_b, float t);


#endif