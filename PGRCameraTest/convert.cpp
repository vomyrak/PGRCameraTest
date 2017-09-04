#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "convert.h"

using namespace std;

void convert_data(std::string in_path, std::string out_path, const unsigned int width, const unsigned int height)
{
    ifstream ifs(in_path);
	ofstream ofs(out_path);

    double x, y, z;
    while(ifs >> x >> y >> z)
    {
		double theta; 
		double phi;
		int lat = cartesianToLat(x, z - RADIUS, y, theta, phi, width, height);
		int lon = cartesianToLon(x, z - RADIUS, y, theta, phi, width, height);


		//TRICK FOR SYMMETRY
		if(lat == (width-1) ){ ////////////////////////////////////////////////////////////////////////////////////////////////
			ofs << 0 << ' ' << lon << endl;
		}
		//END TRICK

		ofs << lat << ' ' << lon << endl;

		//cout << "lat: " << lat << endl;
		//cout << "lon: " << lon << endl;
    }
}


void sphericalToCartesian(const double r, const double &theta_deg, const double &phi_deg, double &x, double &y, double &z)
{
    double theta = degToRad(theta_deg);
    double phi = degToRad(phi_deg);
    x = r * sin(theta) * cos(phi);
    z = r * sin(theta) * sin(phi);
    y = r * cos(theta);
}

void cartesianToSpherical(const double &x, const double &y, const double &z, double &theta, double &phi)
{
    double r = sqrt(x*x + y*y + z*z);

	//cout << "r: " << r << endl;

    theta = acos(y/r);

    //atan2 compute the the correct value for phi depending on the quadrant (x,z) are in
    //The value return is in the range -Pi ; Pi. Hence Pi is added for the result to be in the range 0 2Pi.
    phi = fmod(atan2(z, x), 2.0*M_PI) + M_PI;
}

int cartesianToLat(const double &x, const double &y, const double &z, double &theta, double &phi, const unsigned int width, const unsigned int height)
{
	cartesianToSpherical(x, y, z, theta, phi);
    return floor((double) (width-1)*phi/(2.0*M_PI));
}

int cartesianToLon(const double &x, const double &y, const double &z, double &theta, double &phi, const unsigned int width, const unsigned int height)
{
	cartesianToSpherical(x, y, z, theta, phi);
    return floor((double) (height-1)*theta/M_PI);
}


double radToDeg(const double &radianValue)
{
    return radianValue * 180.0 / M_PI;
}

double degToRad(const double &degreeValue)
{
    return degreeValue * M_PI / 180.0;
}




