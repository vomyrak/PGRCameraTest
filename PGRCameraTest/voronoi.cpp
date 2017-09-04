#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>  

#include "voronoi.h"
#include "edit.h"

using namespace std;
using namespace cv;

vector< int > px_cell;
vector< float > lights_max;
vector< Point2f > lights_pos;
vector< Vec3f > lights_col;
vector< Vec3f > lights_rot;
vector< Vec3f > lights_top; //contains averages for each pair of light in the first 2 rows
vector< Vec3f > lights_weight;
vector< vector< Vec3f > > lights_rot_complete;

// Draw a single point
Mat apply_voronoi( Mat& img, string environment, string pts_path, string img_path, bool rotation )
{
    // Define colors for drawing.
    Scalar points_color(0, 0, 1);
     
    // Keep a copy around
    Mat img_orig = img.clone();
     
    // Rectangle to be used with Subdiv2D
    Size size = img.size();
    Rect rect(0, 0, size.width, size.height);
 
    // Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);
     
    // Create a vector of points.
    vector<Point2f> points;
 
    // Read in the points from a text file
    ifstream ifs(pts_path); //<< INPUT
    int x, y;
    while(ifs >> x >> y)
    {
        points.push_back(Point2f(x,y));
    }

	// Sort points
	std::stable_sort(points.begin(), points.end(), xComparator);
	std::stable_sort(points.begin(), points.end(), yComparator);
     
    // Insert points into subdiv
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
    {
        subdiv.insert(*it);
    }
 
    // Draw points
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
    {
		draw_point(img, *it, points_color);
    }
     
    // Allocate space for Voronoi Diagram
    Mat img_voronoi = Mat::zeros(img.rows, img.cols, CV_8UC3);
     
	// Data for Voronoi diagram
	vector<vector<Point2f> > facets;
    vector<Point2f> centers;

    // Draw Voronoi diagram
    draw_voronoi( img_voronoi, subdiv, img_orig, facets, centers );

	// Save Voronoi diagram
	imwrite(img_path, img_voronoi);



	string save_path;
	save_path = "C:\\CO417-HW1\\voronoi_";
	save_path += environment;
	save_path += "\\";


	string img_orig_path(save_path);
	img_orig_path += "original.png";
	apply_gamma( img_orig );
	imwrite(img_orig_path, img_orig);


	draw_centers( img, subdiv, facets, centers );
	string img_centers_path(save_path);
	img_centers_path += "centers.png";
	apply_gamma( img );
	imwrite(img_centers_path, img);


	draw_centers_lines( img, subdiv, facets, centers );
	string img_lines_path(save_path);
	img_lines_path += "centers_lines.png";
	imwrite(img_lines_path, img);




	cout << "Close the image to continue" << endl; 

    // Show results
    imshow( "Voronoi Diagram", img_voronoi);
    waitKey(0);

	// Rotate
	if(rotation)
	{
		string to_path;
		to_path = "C:\\CO417-HW1\\voronoi_";
		to_path += environment;
		to_path += "\\rotation_x12\\rotated";

		rotate(img_voronoi, to_path, facets, centers);
		set_rot();
	}
	// END Rotate
	




	Mat img_ret;
	img_voronoi.Mat::convertTo(img_ret , CV_32FC3, 1/255.0);

	return img_ret;
}

bool xComparator (Point2f p1, Point2f p2)
{
  return (p1.x < p2.x);
}

bool yComparator (Point2f p1, Point2f p2)
{
  return (p1.y < p2.y);
}

// Draw a single point
void draw_point( Mat& img, Point2f fp, Scalar color )
{
    circle( img, fp, 2, color, CV_FILLED, CV_AA, 0 );
}
 
//Draw voronoi diagram
void draw_voronoi( Mat& img, Subdiv2D& subdiv,  Mat& img_orig, vector<vector<Point2f> >& facets, vector<Point2f>& centers )
{
    subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

	lights_pos = centers; //save the light positions <<<<<< TO CLEAR MAYBE?

    vector<Point> ifacet;
    vector<vector<Point> > ifacets(1);

	compute_col(img_orig, subdiv);

	//weights calculus:  
	compute_weights();   //<<<<<<WEIGHTS
 
    for( size_t i = 0; i < facets.size(); i++ )
    {
        ifacet.resize(facets[i].size());
        for( size_t j = 0; j < facets[i].size(); j++ )
            ifacet[j] = facets[i][j];


		
		//Gamma corrected colors:
		double gamma = 2.2;
		Scalar color; //<<<< COLOUR
		color[0] = pow(lights_weight[i][2], 1.0/gamma) * 255;
        color[1] = pow(lights_weight[i][1], 1.0/gamma) * 255;
        color[2] = pow(lights_weight[i][0], 1.0/gamma) * 255;
		


		/*
		//Weighted colors:
		Scalar color; //<<<< COLOUR
		color[0] = lights_weight[i].val[2] * 255;
        color[1] = lights_weight[i].val[1] * 255;
        color[2] = lights_weight[i].val[0] * 255;
		*/

		
		/*
		//Integral over one cell:
		Scalar color; //<<<< COLOUR
		color[0] = lights_col[i].val[2];
        color[1] = lights_col[i].val[1];
        color[2] = lights_col[i].val[0];
		*/
		
		/*
		//Point-based colours:
		float x = centers[i].x;
		float y = centers[i].y;
 
        Vec3f intensity = img_orig.at<Vec3f>(Point(x, y));
		cout << "b: " << intensity.val[0] << endl;
		cout << "g: " << intensity.val[1] << endl;
		cout << "r: " << intensity.val[2] << endl;

		Scalar color; //<<<< COLOUR
        color[0] = intensity.val[0];
        color[1] = intensity.val[1];
        color[2] = intensity.val[2];
		*/

		/*
		//Example colours:
		Scalar color;
        color[0] = rand() & 255;
        color[1] = rand() & 255;
        color[2] = rand() & 255;
		*/

        fillConvexPoly(img, ifacet, color, 8, 0);
 

		Scalar lines_col(255,0,0);
		Scalar circles_col(0,0,255);

        ifacets[0] = ifacet;
        polylines(img, ifacets, true, lines_col, 1, CV_AA, 0);
        circle(img, centers[i], 3, circles_col, CV_FILLED, CV_AA, 0);
    }
}



void draw_centers( Mat& img, Subdiv2D& subdiv, vector<vector<Point2f> >& facets, vector<Point2f>& centers )
{
    subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

    for( size_t i = 0; i < facets.size(); i++ )
    {
		Scalar circles_col(0,0,255);
        circle(img, centers[i], 3, circles_col, CV_FILLED, CV_AA, 0);
    }
}




void draw_centers_lines( Mat& img, Subdiv2D& subdiv, vector<vector<Point2f> >& facets, vector<Point2f>& centers )
{
    subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

    vector<Point> ifacet;
    vector<vector<Point> > ifacets(1);

    for( size_t i = 0; i < facets.size(); i++ )
    {
        ifacet.resize(facets[i].size());
        for( size_t j = 0; j < facets[i].size(); j++ )
            ifacet[j] = facets[i][j];

		Scalar lines_col(255,0,0);
		Scalar circles_col(0,0,255);

        ifacets[0] = ifacet;
        polylines(img, ifacets, true, lines_col, 1, CV_AA, 0);
        circle(img, centers[i], 3, circles_col, CV_FILLED, CV_AA, 0);
    }
}


void apply_gamma( Mat& img )
{
	double gamma = 2.2;
	int width(img.cols), height(img.rows);
   
	
    for(int i = 0 ; i < height ; i++) //y
    {
        for(int j = 0 ; j < width ; j++) //x
        {
			Vec3f color = Vec3f(0.0, 0.0, 0.0);

			float R, G, B;
            R = img.at<Vec3f>(Point(j,i)).val[2] ;
            G = img.at<Vec3f>(Point(j,i)).val[1] ;
            B = img.at<Vec3f>(Point(j,i)).val[0] ;
			
			color[0] = pow(B, 1.0/gamma) * 255 ;
			color[1] = pow(G, 1.0/gamma) * 255 ;
			color[2] = pow(R, 1.0/gamma) * 255 ;
			

			img.at<Vec3f>(Point2i(j,i)) = color;
        }
    }
}



void delete_gamma( Mat& img )
{
	double gamma = 2.2;
	int width(img.cols), height(img.rows);
   
	
    for(int i = 0 ; i < height ; i++) //y
    {
        for(int j = 0 ; j < width ; j++) //x
        {
			Vec3f color = Vec3f(0.0, 0.0, 0.0);

			float R, G, B;
            R = img.at<Vec3f>(Point(j,i)).val[2] / 255;
            G = img.at<Vec3f>(Point(j,i)).val[1] / 255;
            B = img.at<Vec3f>(Point(j,i)).val[0] / 255;
			
			color[0] = pow(B, gamma) * 255 ;
			color[1] = pow(G, gamma) * 255 ;
			color[2] = pow(R, gamma) * 255 ;
			

			img.at<Vec3f>(Point2i(j,i)) = color;
        }
    }
}





void set_rot() 
{

	for(unsigned int i = 0; i < lights_rot_complete.size(); i++)
    {
		for(unsigned int k = 0; k < lights_rot_complete[i].size(); k++)
		{
			lights_rot_complete[i][k][0] = lights_rot_complete[i][k][0] * 255;
			lights_rot_complete[i][k][1] = lights_rot_complete[i][k][1] * 255;
			lights_rot_complete[i][k][2] = lights_rot_complete[i][k][2] * 255;
		}
    }
	
}

int get_rot_size() 
{
	return lights_rot_complete.size();
}

vector< vector< Vec3f > > get_rot_complete() 
{
	return lights_rot_complete;
}

vector< Vec3f > get_rot(int k) 
{
	return lights_rot_complete[k];
}


vector< Vec3f > get_rot_gamma(int k)
{
	vector< Vec3f > lights_rot_k;
	double gamma = 2.2;

	for(unsigned int i = 0; i < lights_rot_complete[k].size(); i++)
    {
		Vec3f rot;
		rot[0] = pow(lights_rot_complete[k][i][0], 1.0/gamma) * 255;
        rot[1] = pow(lights_rot_complete[k][i][1], 1.0/gamma) * 255;
        rot[2] = pow(lights_rot_complete[k][i][2], 1.0/gamma) * 255;

		lights_rot_k.push_back( rot );
    }
	return lights_rot_k;
}



vector< Vec3f > get_weights()
{
	vector< Vec3f > weights;

	for(unsigned int i = 0; i < lights_weight.size(); i++)
    {
		Vec3f wei;
		wei[0] = lights_weight[i][0] * 255;
        wei[1] = lights_weight[i][1] * 255;
        wei[2] = lights_weight[i][2] * 255;

		weights.push_back( wei );
    }
	return weights;
}



vector< Vec3f > get_weights_gamma()
{
	vector< Vec3f > weights;
	double gamma = 2.2;

	for(unsigned int i = 0; i < lights_weight.size(); i++)
    {
		Vec3f wei;
		wei[0] = pow(lights_weight[i][0], 1.0/gamma) * 255;
        wei[1] = pow(lights_weight[i][1], 1.0/gamma) * 255;
        wei[2] = pow(lights_weight[i][2], 1.0/gamma) * 255;

		weights.push_back( wei );
    }
	return weights;
}






int findLightSource(int x, int y)
{
    //Point2f currentPoint;

    for(unsigned int i = 0 ; i < lights_pos.size(); i++)
    {
        //currentPoint = lights_pos[i];
        if((lights_pos[i].x == x) && (lights_pos[i].y == y))
        {
           return i;
        }
    }
    return -1;
}

void compute_col(Mat &img, Subdiv2D& subdiv)
{
    float R = 0.0, G = 0.0, B = 0.0;
    int cellNumber;
	int width(img.cols), height(img.rows);

    //Initialisation
    for(unsigned int k = 0; k < lights_pos.size(); k++)
    {
        lights_col.push_back( Vec3f(0.0, 0.0, 0.0) );
		//px_cell.push_back( 0 );
    }

	
    Point2i currentPoint;
	Point2f nearest_center;
	
	cout << "compute_col started " << endl;
    for(int i = 0 ; i < height ; i++) //y
    {
        for(int j = 0 ; j < width ; j++) //x
        {
            currentPoint = Point2i(j,i);

			subdiv.findNearest(currentPoint, &nearest_center);
            cellNumber = findLightSource( (nearest_center).x, (nearest_center).y );


			cout << "WIP: " << ((double)(i) / height) * 100 << " %" << endl;


            //OpenCV uses BGR
            R = img.at<Vec3f>(Point(j,i)).val[2];
            G = img.at<Vec3f>(Point(j,i)).val[1];
            B = img.at<Vec3f>(Point(j,i)).val[0];

            lights_col[cellNumber][0] += R*sin((float) i*M_PI/height); //Multiply the intensity by the solid angle
            lights_col[cellNumber][1] += G*sin((float) i*M_PI/height); //Multiply the intensity by the solid angle
            lights_col[cellNumber][2] += B*sin((float) i*M_PI/height); //Multiply the intensity by the solid angle	

			//px_cell[cellNumber] += 1;
        }
    }


	//TRICK FOR SYMMETRY
	for(int i = 0 ; i < height ; i++) //y
    {
		int cell_right = findLightSource( 1023, i );
		int cell_left = findLightSource( 0, i );

		if((cell_right != -1) && (cell_left != -1))
		{
			lights_col[cell_right][0] = (lights_col[cell_right][0] + lights_col[cell_left][0]);
            lights_col[cell_right][1] = (lights_col[cell_right][1] + lights_col[cell_left][1]);
            lights_col[cell_right][2] = (lights_col[cell_right][2] + lights_col[cell_left][2]);

			lights_col[cell_left][0] = (lights_col[cell_right][0] + lights_col[cell_left][0]);
            lights_col[cell_left][1] = (lights_col[cell_right][1] + lights_col[cell_left][1]);
            lights_col[cell_left][2] = (lights_col[cell_right][2] + lights_col[cell_left][2]);
		}
    }
	//END TRICK

}

void compute_max()
{
    for(unsigned int i = 0 ; i< lights_col.size() ; i++)
    {
		float max_col;
		int pos_col;
		max_vec3f(lights_col[i], max_col, pos_col);
		lights_max.push_back( max_col );
    }
}

void max_vec(vector<float> &vec, float &maxValue, int &position)
{

    float maxSoFar = 0.0;
    for(unsigned int i = 0 ; i< vec.size() ; i++)
    {
        if(vec[i] > maxSoFar)
        {
            maxSoFar = vec[i];
            position = i;
        }
    }

    maxValue = maxSoFar;
}

void max_vec3f(Vec3f &vec, float &maxValue, int &position)
{

    float maxSoFar = 0.0;
    for(unsigned int i = 0 ; i < 3 ; i++)
    {
        if(vec[i] > maxSoFar)
        {
            maxSoFar = vec[i];
            position = i;
        }
    }

    maxValue = maxSoFar;
}

void brightest_cell_col(float &max, int &pos_max)
{
	compute_max();
	max_vec(lights_max, max, pos_max);
}


void compute_weights()
{
	float max;
	int pos_max;

	brightest_cell_col(max, pos_max);

	//Initialisation
    for(unsigned int k = 0; k < lights_col.size(); k++)
    {
        lights_weight.push_back( Vec3f(0.0, 0.0, 0.0) );
    }

    for(unsigned int i = 0 ; i< lights_col.size() ; i++)
    {
			lights_weight[i][0] = lights_col[i][0] / max;
			lights_weight[i][1] = lights_col[i][1] / max;
			lights_weight[i][2] = lights_col[i][2] / max;
    }


	//DEBUG
	/*
		lights_weight[1] = Vec3f(0.0, 0.1, 0.0); //G
		lights_weight[2] = Vec3f(0.0, 0.0, 0.1); //B
		lights_weight[7] = Vec3f(0.1, 0.0, 0.0); //R
		lights_weight[12] = Vec3f(0.0, 0.0, 0.1); //B
		lights_weight[12] = Vec3f(0.0, 0.0, 255.0); //B
		lights_weight[13] = Vec3f(0.0, 0.1, 0.0); //G
		lights_weight[27] = Vec3f(0.1, 0.0, 0.0); //R
		lights_weight[40] = Vec3f(0.1, 0.0, 0.0); //R
		lights_weight[151] = Vec3f(0.1, 0.0, 0.0); //R
		lights_weight[163] = Vec3f(0.1, 0.0, 0.0); //R
	*/

	//DEBUG
	/*
		lights_weight[5] = Vec3f(0.0, 0.0, 255.0); //B
		lights_weight[12] = Vec3f(0.0, 255.0, 0.0); //G
		lights_weight[24] = Vec3f(255.0, 0.0, 0.0); //R
		lights_weight[36] = Vec3f(0.0, 0.0, 255.0); //B
		lights_weight[49] = Vec3f(0.0, 255.0, 0.0); //G
		lights_weight[61] = Vec3f(255.0, 0.0, 0.0); //R


		lights_weight[136] = Vec3f(0.0, 0.0, 255.0); //B
		lights_weight[149] = Vec3f(0.0, 255.0, 0.0); //G
		lights_weight[161] = Vec3f(255.0, 0.0, 0.0); //R
		lights_weight[174] = Vec3f(0.0, 0.0, 255.0); //B
	*/
	//END DEBUG
}

void print_weights()
{
	for(unsigned int i = 0 ; i< lights_weight.size() ; i++)
    {
		cout << "lights_weight[i][0] : " << lights_weight[i][0] << endl;
		cout << "lights_weight[i][1] : " << lights_weight[i][1] << endl;
		cout << "lights_weight[i][2] : " << lights_weight[i][2] << endl;
    }
	cout << "lights_weight.size() : " << lights_weight.size() << endl;
	cout << "lights_col.size() : " << lights_col.size() << endl;
	
	
}



//Draw new voronoi diagram
void draw_new( Mat& img, vector<vector<Point2f>>& facets, vector<Point2f>& centers)
{
    vector<Point> ifacet;
    vector<vector<Point> > ifacets(1);
 
    for( size_t i = 0; i < facets.size(); i++ )
    {
        ifacet.resize(facets[i].size());
        for( size_t j = 0; j < facets[i].size(); j++ )
            ifacet[j] = facets[i][j];


		
		//Gamma corrected colors:
		double gamma = 2.2;
		Scalar color; //<<<< COLOUR
		color[0] = pow(lights_rot[i][2], 1.0/gamma) * 255;
        color[1] = pow(lights_rot[i][1], 1.0/gamma) * 255;
        color[2] = pow(lights_rot[i][0], 1.0/gamma) * 255;
		

		/*
		//NO-GAMMA colors:
		Scalar color; //<<<< COLOUR
		color[0] = lights_rot[i][2] * 255;
        color[1] = lights_rot[i][1] * 255;
        color[2] = lights_rot[i][0] * 255;
		*/


        fillConvexPoly(img, ifacet, color, 8, 0);
 

		Scalar lines_col(255,0,0);
		Scalar circles_col(0,0,255);

        ifacets[0] = ifacet;
        polylines(img, ifacets, true, lines_col, 1, CV_AA, 0);
        circle(img, centers[i], 3, circles_col, CV_FILLED, CV_AA, 0);
    }
}



void compute_rotation(float t, vector< Vec3f > lights_temp)
{
	//Initialisation
    for(unsigned int k = 0; k < lights_col.size(); k++)
    {
        lights_rot.push_back( Vec3f(0.0, 0.0, 0.0) );
    }

	int len = 0;

	for(unsigned int i = 13 ; i < lights_col.size() ; i++)
    {

		if(lights_pos[i].y == lights_pos[i-1].y)
		{
			lights_rot[i] = interpolate_col(lights_temp[i], lights_temp[i-1], t);
			
			len++;
		}
		
		if((lights_pos[i].y != lights_pos[i-1].y))
		{
			if(lights_pos[i-1].x == 1023)
			{
				lights_rot[i-1] = interpolate_col(lights_temp[i-1], lights_temp[i-2], t);
				lights_rot[i-len-1] = interpolate_col(lights_temp[i-len-1], lights_temp[i-2], t);
			}

			if(lights_pos[i-1].x != 1023)
			{
				lights_rot[i-len-1] = interpolate_col(lights_temp[i-len-1], lights_temp[i-1], t);
			}

			len = 0;
		}

    }

	//last one missing:
	lights_rot[163] = interpolate_col(lights_temp[163], lights_temp[174], t);
}



void compute_rotation_top(float t)
{
	for(unsigned int i = 1 ; i < 13 ; i++)
    {
		lights_rot[i] = interpolate_col(lights_top[2*i], lights_top[2*i-1], t);
    }

	lights_rot[6] = interpolate_col(lights_top[0], lights_top[11], t);
	lights_rot[0] = interpolate_col(lights_top[0], lights_top[11], t);

	lights_rot[7] = interpolate_col(lights_top[14], lights_top[13], t);
}


void init_top(vector< Vec3f > lights_temp)
{
	//Initialisation
	for(unsigned int k = 0; k < 25; k++)
    {
        lights_top.push_back( Vec3f(0.0, 0.0, 0.0) );
    }


	for(unsigned int i = 0; i < 6; i++)
    {
		lights_top[2*i] = lights_temp[i];
        lights_top[2*i+1] = (lights_temp[i] + lights_temp[i+1]) / 2;
    }

	lights_top[12] = lights_temp[6];
	lights_top[13] = (lights_temp[7] + lights_temp[12]) / 2;

	for(unsigned int i = 7; i < 12; i++)
    {
        lights_top[2*i] = lights_temp[i];
        lights_top[2*i+1] = (lights_temp[i] + lights_temp[i+1]) / 2;
    }

	lights_top[24] = lights_temp[12];

}

void swap_top()
{
	vector< Vec3f > lights_temp(lights_top);

	for(unsigned int i = 1; i < 25; i++)
    {
		lights_top[i] = lights_temp[i-1];
    }


	lights_top[12] = lights_temp[11];
	lights_top[0] = lights_temp[11];

	lights_top[13] = lights_temp[24];

}



void rotate( Mat& img, string environment_path, vector<vector<Point2f> >& facets, vector<Point2f>& centers )
{
	vector< Vec3f > lights_temp(lights_weight);

	//BASE CASE
	// Allocate space for Voronoi Diagram
    Mat img_rotated = Mat::zeros(img.rows, img.cols, CV_8UC3);

	// Init lights_rot
	compute_rotation(0.0, lights_temp);

	// Init lights_top
	init_top(lights_temp);

	// Init lights_rot for top 2 rows
	compute_rotation_top(0.0);

	// Save new rotation in lights_rot_complete <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	lights_rot_complete.push_back( lights_rot );

    // Draw NEW Voronoi diagram
    draw_new( img_rotated, facets, centers );
 
	// Save Voronoi diagram
	string write_path(environment_path);
	write_path += "0.0.png";
	imwrite(write_path, img_rotated);
	//END BASE CASE

	for(unsigned int j = 0 ; j < 12 ; j++)
	{
		for(unsigned int i = 1 ; i < (unsigned int)(STEPS+1) ; i++)
		{
			// Allocate space for Voronoi Diagram
			Mat img_rotated = Mat::zeros(img.rows, img.cols, CV_8UC3);

			// Compute lights_rot
			compute_rotation(i/STEPS, lights_temp);

			// Compute lights_rot for top 2 rows
			compute_rotation_top(i/STEPS);

			// Save new rotation in lights_rot_complete <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			lights_rot_complete.push_back( lights_rot );

			// Draw NEW Voronoi diagram
			draw_new( img_rotated, facets, centers );
 
			// Save Voronoi diagram
			string var(environment_path);
			var += to_string(j);
			var += ".";
			var += to_string(i);
			var += ".png";
			imwrite(var, img_rotated);
		}
		
		lights_temp = lights_rot;

		swap_top();
	}




	

	/*
    // Allocate space for Voronoi Diagram
    Mat img_rotated = Mat::zeros(img.rows, img.cols, CV_8UC3);

	// Compute lights_rot
	compute_rotation(0.5);

    // Draw NEW Voronoi diagram
    draw_new( img_rotated, facets, centers );
 
	// Save Voronoi diagram
	imwrite(img_path, img_rotated);

    // Show results.
    imshow( "Rotated Diagram", img_rotated );
    waitKey(0);
	*/
	
	/*
	Mat img_ret;

	img_rotated.Mat::convertTo(img_ret , CV_32FC3, 1/255.0);

	return img_ret;
	*/
}

