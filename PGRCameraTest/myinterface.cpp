#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>  

#include "interface.h"
#include "voronoi.h"

using namespace std;
using namespace cv;

void sort_data(std::string in_path, vector<Point2f> &points)
{

    ifstream ifs(in_path); //LATLON text file

    int x, y;
    while(ifs >> x >> y)
    {
        points.push_back(Point2f(x,y));
    }

	// Sort points
	std::stable_sort(points.begin(), points.end(), xComparator);
	std::stable_sort(points.begin(), points.end(), yComparator); 
}

int find_light(int x, int y, vector<Point2f>& points)
{
    for(unsigned int i = 0 ; i < points.size(); i++)
    {
        if((points[i].x == x) && (points[i].y == y))
        {
           return i;
        }
    }
    return -1;
}


//!!only rgb lights considered!!
void build_table(std::string out_path, vector<Point2f> &points, const unsigned int width, const unsigned int height)
{
	ofstream ofs(out_path);

	int middle = (int) (width-1)/2;
	bool first;
	int i = 0;
	int j;

	for(int x = middle; x < width ; x++) //x
    {
		j = 0;
		first = true;
        for(int y = 0 ; y < height; y++) //y
        {
			int k = find_light(x, y, points);

			if((k != -1) && !first)
			{
				ofs << k << ' ' << i << ' ' << 2*j << endl;
				j++;
			}

			if((k != -1) && first)
			{
				first = false;

				ofs << k << ' ' << i << ' ' << 2*j << endl;
				i++;
				j++;
			}
        }
    }

	for(int x = 1; x < middle ; x++) //x
    {
		j = 0;
		first = true;
        for(int y = 0 ; y < height; y++) //y
        {
			int k = find_light(x, y, points);

			if((k != -1) && !first)
			{
				//TRICK
				if(i==24)
				{
					ofs << k << ' ' << 0 << ' ' << 2*j << endl;
				}
				else
				{
					ofs << k << ' ' << i << ' ' << 2*j << endl;
				}
				//END TRICK
				j++;
			}

			if((k != -1) && first)
			{
				first = false;
				ofs << k << ' ' << i << ' ' << 2*j << endl;

				i++;
				j++;
			}
        }
    }

}

void invert_table(std::string in_path, std::string out_path)
{
	ifstream ifs(in_path);
	ofstream ofs(out_path);

	int k, i, j;
    while(ifs >> k >> i >> j)
    {
		if(i == 0)
		{
			ofs << k << ' ' << 0 << ' ' << j << endl;
		}
		else
		{
			ofs << k << ' ' << 24 - i << ' ' << j << endl;
		}
		
    }

}

void fix_table(std::string in_path, std::string out_path)
{
	ifstream ifs(in_path);
	ofstream ofs(out_path);

	int k, i, j;
    while(ifs >> k >> i >> j)
    {
		ofs << k << ' ' << (i + 1) % 24 << ' ' << j << endl;
    }

}



bool no_to_coord(std::string in_path, int no, int &arc, int &index)
{
    ifstream ifs(in_path); //TABLE text file

    int k, i, j;
    while(ifs >> k >> i >> j)
    {
        if(k == no)
		{
			arc = i;
			index = j;

			return true;
		}
    }
	return false;
}


bool coord_to_no(std::string in_path, int &no, int arc, int index)
{
    ifstream ifs(in_path); //TABLE text file

    int k, i, j;
    while(ifs >> k >> i >> j)
    {
        if(i == arc && j == index)
		{
			no = k;

			return true;
		}
    }
	return false;
}



void save_vec(std::string out_path, vector< Vec3f >& vec)
{
    ofstream ofs(out_path); //SAVING text file

	for(unsigned int i = 0; i < 175; i++)
    {
		int arc, index;
		bool success = no_to_coord("C:\\CO417-HW1\\table_inv.txt", i, arc, index);

		if(success)
		{
			ofs << i << ' ' << arc << ' ' << index << ' ' << vec[i][0] << ' ' << vec[i][1] << ' ' << vec[i][2] << endl;
		}
    }
}


void load_vec(std::string in_path, vector< Vec3f >& vec)
{
    ifstream ifs(in_path); //LOADING text file

	float r, g, b;
    while(ifs >> r >> g >> b)
    {
		Vec3f col;
		col[0] = r;
        col[1] = g;
        col[2] = b;

		vec.push_back( col );
    }
}


void float_to_int(std::string in_path, std::string out_path)
{
	ifstream ifs(in_path);
	ofstream ofs(out_path);

	int no, arc, index;
	float r, g, b;
    while(ifs >> no >> arc >> index >> r >> g >> b)
    {
		int red = (int) r;
		int green = (int) g;
		int blue = (int) b;

		ofs << unsigned((uint8_t) no) << ' ' << unsigned((uint8_t) arc) << ' ' << unsigned((uint8_t) index) << ' ' << unsigned((uint8_t) red) << ' ' << unsigned((uint8_t) green) << ' ' << unsigned((uint8_t) blue) << endl;
    }

}

void play_rotations(std::string in_path)
{
	for(unsigned int i = 0 ; i < 361 ; i++)
	{
		string in(in_path);
		in += to_string(i);
		in += ".png";

		Mat img;
		img = imread(in, CV_LOAD_IMAGE_COLOR); 
	
		imshow("Rotation", img);
		waitKey(20);
	}
}


void play_one_rotation(std::string in_path, int i)
{
	Mat img;
	img = imread(in_path, CV_LOAD_IMAGE_COLOR); 
	
	imshow("Rotation", img);
}


void rename_rotation(std::string in, std::string out)
{
	int no = 0;

	//INIT
	string in_path(in);
	string out_path(out);

	in_path += to_string(0);
	in_path += ".";
	in_path += to_string(0);
	in_path += ".png";

	Mat img;
	img = imread(in_path, 1);

	out_path += to_string(no);
	out_path += ".png";


	cout << "no: " << no << endl;
	cv::imwrite(out_path, img);
	no++;
	//end INIT

	for(unsigned int j = 0 ; j < 12 ; j++)
	{
		for(unsigned int i = 1 ; i < (unsigned int)(STEPS+1) ; i++)
		{
			string in_path(in);
			string out_path(out);

			in_path += to_string(j);
			in_path += ".";
			in_path += to_string(i);
			in_path += ".png";

			Mat img;
			img = imread(in_path, 1);

			out_path += to_string(no);
			out_path += ".png";


			cout << "no: " << no << endl;
			cv::imwrite(out_path, img);
			no++;
		}
	}
}


void rgb_to_hsv_file(std::string in_path, std::string out_path)
{
	ifstream ifs(in_path);
	ofstream ofs(out_path);

	int no, arc, index;
	float r, g, b;
    while(ifs >> no >> arc >> index >> r >> g >> b)
    {
		float h, s, v;
		rgb_to_hsv(r, g, b, h, s, v);

		ofs << no << ' ' << arc << ' ' << index << ' ' << r << ' ' << g << ' ' << b << ' ' << h << ' ' << s << ' ' << v << endl;
    }

}


void rgb_to_hsv(float& r_orig, float& g_orig, float& b_orig, float& final_h, float& final_s, float& final_v)
{
	float r = r_orig / 255.0;
    float g = g_orig / 255.0;
    float b = b_orig / 255.0;

	float h, s, v; // h:0-360.0, s:0.0-1.0, v:0.0-1.0

	float max = std::max(r, std::max(g, b) );
    float min = std::min(r, std::min(g, b) );
	float delta = max - min;

	v = max;

	if (max == 0.0 || delta == 0.0) 
	{
        s = 0.0;
        h = 0.0;
    }
    else 
	{
        s = delta / max;

        if (max == r) 
		{
            h = 60 * ((g - b) / delta);
        }

        if (max == g) 
		{
            h = 60 * ((b - r) / delta + 2);
        }

        if (max == b)  
		{
            h = 60 * ((r - g) / delta + 4);
        }
    }

	if (h < 0) 
	{
		h += 360.0;
	}

	
    final_h = h;   // final_h : 0-360.0
    final_s = s; // final_s : 0.0-1.0
    final_v = v; // final_v : 0.0-1.0

	/*
    final_h = (unsigned char)(h / 2);   // final_h : 0-180
    final_s = (unsigned char)(s * 255); // final_s : 0-255
    final_v = (unsigned char)(v * 255); // final_v : 0-255
	*/

}


void compute_ratio_file(std::string in_path, std::string out_path)
{
	ifstream ifs(in_path);
	ofstream ofs(out_path);

	int no, arc, index;
	float r, g, b;
	float h, s, v;
    while(ifs >> no >> arc >> index >> r >> g >> b >> h >> s >> v)
    {
		float rgb_weight, w_weight;
		float avg = (r + g + b) / 3;
		compute_ratio(rgb_weight, w_weight, s);

		ofs << no << ' ' << arc << ' ' << index << ' ' << r * rgb_weight << ' ' << g * rgb_weight << ' ' << b * rgb_weight << endl;
		ofs << no << ' ' << arc << ' ' << index+1 << ' ' << avg * w_weight << ' ' << avg * w_weight << ' ' << avg * w_weight << endl;
    }

}


void compute_ratio(float& rgb_weight, float& w_weight, float& s)
{
	float ratio = s/2.0;

	rgb_weight = 0.5 + ratio;
	w_weight = 0.5 - ratio;
}


void re_compute_file(std::string in_path, std::string out_path)
{
	ifstream ifs(in_path);
	ofstream ofs(out_path);

	int no, arc, index;
	float r, g, b;
    while(ifs >> no >> arc >> index >> r >> g >> b)
    {
		ofs << no << ' ' << (arc + 1) % 24  << ' ' << index << ' ' << r  << ' ' << g  << ' ' << b  << endl;
    }
}