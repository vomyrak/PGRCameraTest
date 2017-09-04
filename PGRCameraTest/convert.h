#ifndef CONVERT_H
#define CONVERT_H

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>

#define RADIUS       1250.0 //mm



void convert_data(std::string in_path, std::string out_path, const unsigned int width, const unsigned int height);
void sphericalToCartesian(const double r, const double &theta_deg, const double &phi_deg, double &x, double &y, double &z);
void cartesianToSpherical(const double &x, const double &y, const double &z, double &theta, double &phi);
int cartesianToLat(const double &x, const double &y, const double &z, double &theta, double &phi, const unsigned int width, const unsigned int height);
int cartesianToLon(const double &x, const double &y, const double &z, double &theta, double &phi, const unsigned int width, const unsigned int height);
double radToDeg(const double &radianValue);
double degToRad(const double &degreeValue);


#endif