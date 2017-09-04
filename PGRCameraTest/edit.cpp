#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <Windows.h>

#include "edit.h"

using namespace std;
using namespace cv;


Mat linear_edit( Mat& img_orig, double alpha ) // contrast control + brightness control
{
	Mat new_img = Mat::zeros( img_orig.size(), img_orig.type() );

	for( int y = 0; y < img_orig.rows; y++ )
	{ 
		for( int x = 0; x < img_orig.cols; x++ )
		{ 
			for( int c = 0; c < 3; c++ )
			{
				new_img.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha * ( img_orig.at<Vec3b>(y,x)[c] ) );
			}
		}
	}

	return new_img;
}


/*
int keyboard_entry( string in_path )
{
	Mat img;
	bool going = true;
	//char input;

	cout << "press up down or left" << endl;

	while(going)
	{
		
		//cin >> input;

		if (GetKeyState(VK_UP) & 0x8000)
		{
			cout << "you pressed UP" << endl;
			img = linear_edit( "\\\\icnas2.cc.ic.ac.uk\\acf16\\CO417-HW1\\voronoi.png", 2.0);

			
			if (img.empty()) //check whether the image is loaded or not
			 {
				  cout << "Error : Image cannot be loaded..!!" << endl;
				  //system("pause"); //wait for a key press
				  return -1;
			 }
			 

			imshow("New image", img );
			waitKey(0);

		}

		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			cout << "you pressed DOWN"  << endl;
			img = linear_edit( "\\\\icnas2.cc.ic.ac.uk\\acf16\\CO417-HW1\\voronoi.png", 0.5);

			imshow("New image", img );
			waitKey(0);
		}

		
		if (GetKeyState(VK_LEFT) & 0x8000) 
		{
			going = false;
		}
		

	}
	return 0;

}
*/

float interpolate(float startValue, float endValue, float fraction)
{
    return (endValue - startValue) * fraction + startValue;
}

Vec3f interpolate_col(Vec3f col_a, Vec3f col_b, float t)
{
    Vec3f middle = Vec3f(0.0, 0.0, 0.0);

    middle[0] = interpolate(col_a[0], col_b[0], t);
    middle[1] = interpolate(col_a[1], col_b[1], t);
    middle[2] = interpolate(col_a[2], col_b[2], t);

    return middle;
}