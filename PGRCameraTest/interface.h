#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdint>
#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

#define STEPS 30.0


using namespace std;
using namespace cv;

void sort_data(std::string in_path, vector<Point2f> &points);
int find_light(int x, int y, vector<Point2f>& points);
void build_table(std::string out_path, vector<Point2f> &points, const unsigned int width, const unsigned int height);
void invert_table(std::string in_path, std::string out_path);
void fix_table(std::string in_path, std::string out_path);
bool no_to_coord(std::string in_path, int no, int &arc, int &index);
bool coord_to_no(std::string in_path, int &no, int arc, int index);
void save_vec(std::string out_path, vector< Vec3f >& vec);
void load_vec(std::string in_path, vector< Vec3f >& vec);
void float_to_int(std::string in_path, std::string out_path);
void play_rotations(std::string in_path);
void play_one_rotation(std::string in_path, int i);
void rename_rotation(std::string in, std::string out);
void rgb_to_hsv_file(std::string in_path, std::string out_path);
void rgb_to_hsv(float& r_orig, float& g_orig, float& b_orig, float& final_h, float& final_s, float& final_v);
void compute_ratio_file(std::string in_path, std::string out_path);
void compute_ratio(float& rgb_weight, float& w_weight, float& s);
void re_compute_file(std::string in_path, std::string out_path);



#endif