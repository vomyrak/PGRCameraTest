#ifndef VORONOI_H
#define VORONOI_H

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>


#define STEPS 30.0


using namespace std;
using namespace cv;



Mat apply_voronoi( Mat& img, string environment, string pts_path, string img_path, bool rotation );
bool xComparator (Point2f p1, Point2f p2);
bool yComparator (Point2f p1, Point2f p2);
void draw_point( Mat& img, Point2f fp, Scalar color );
void draw_voronoi( Mat& img, Subdiv2D& subdiv,  Mat& img_orig, vector<vector<Point2f> >& facets, vector<Point2f>& centers );
void draw_centers( Mat& img, Subdiv2D& subdiv, vector<vector<Point2f> >& facets, vector<Point2f>& centers );
void draw_centers_lines( Mat& img, Subdiv2D& subdiv, vector<vector<Point2f> >& facets, vector<Point2f>& centers );
void apply_gamma( Mat& img );
void delete_gamma( Mat& img );
void set_rot() ;
int get_rot_size();
vector< vector< Vec3f > > get_rot_complete() ;
vector< Vec3f > get_rot(int k);
vector< Vec3f > get_rot_gamma(int k);
vector< Vec3f > get_weights();
vector< Vec3f > get_weights_gamma();
int findLightSource(int x, int y);
void compute_col(Mat &img, Subdiv2D& subdiv);
void compute_max();
void max_vec(vector<float> &vec, float &maxValue, int &position);
void max_vec3f(Vec3f &vec, float &maxValue, int &position);
void brightest_cell_col(float &max, int &pos_max);
void compute_weights();
void print_weights();
void draw_new( Mat& img, vector<vector<Point2f> >& facets, vector<Point2f>& centers);
void compute_rotation(float t, vector< Vec3f > lights_temp);
void compute_rotation_top(float t);
void init_top(vector< Vec3f > lights_temp);
void swap_top();
void rotate(  Mat& img, string environment_path, vector<vector<Point2f> >& facets, vector<Point2f>& centers );


#endif