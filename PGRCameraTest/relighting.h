#ifndef RELIGHTING_H
#define RELIGHTING_H


#include "stdafx.h"
#include <fstream>
#include <cstring>
#include <cmath>
#include <tuple>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include "LightStage.h"
#include "LightStageCamera.h"
#include "edit.h"
#include "loadPNM.h"
#include "voronoi.h"

#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

#define turn_on 255,255,255,255,255,255
#define turn_half_lit 128,128,128,128,128,128
#define turn_off 0,0,0,0,0,0

#define ROI 900, 720, 250, 180

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace FlyCapture2;
using namespace std;
using namespace cv;

int RunSingleCamera16(LightStage& stage, LightStageCamera& cam, std::string out_path);
void compute_relighting_table(std::string out_path);
bool i_to_arc_index(int& i, int& arc, int& index, std::string in_path);
bool arc_index_to_rgb(int& a, int& i, float& red, float& green, float& blue, std::string in_path);
void multiply_image_i(std::string in_path, std::string out_path, int k, string environment);
void sum_images(std::string in_path, std::string out_path);


#endif