#include <iostream>
#include <fstream>
#include <cstring>
#include <opencv2\opencv.hpp>
#include <cmath>
#include <sstream>
#include <tuple>
#include <regex>
#include <cstdio>
#include <direct.h>
using namespace std;
using namespace cv;

#define ARRAYSIZE 9

//#define ROI 900, 720, 250, 180	//x-coord, y-coord, width, height
#define ROI1 1197, 705, 30, 30
#define ROI2 1260, 694, 30, 30
#define ROI3 1321, 682, 30, 30
#define ROI4 1386, 673, 30, 30
#define ROI5 1452, 665, 30, 30
#define ROI6 1513, 656, 30, 30

#define ROI7 1201, 766, 30, 30
#define ROI8 1265, 756, 30, 30
#define ROI9 1329, 747, 30, 30
#define ROI10 1394, 737, 30, 30
#define ROI11 1459, 729, 30, 30
#define ROI12 1522, 721, 30, 30

#define ROI13 1209, 832, 30, 30
#define ROI14 1275, 820, 30, 30
#define ROI15 1339, 811, 30, 30
#define ROI16 1403, 803, 30, 30
#define ROI17 1466, 795, 30, 30
#define ROI18 1529, 782, 30, 30

#define ROI19 1217, 892, 30, 30
#define ROI20 1284, 883, 30, 30
#define ROI21 1350, 875, 30, 30
#define ROI22 1411, 869, 30, 30
#define ROI23 1475, 860, 30, 30
#define ROI24 1541, 847, 30, 30


void something() {

	struct dirent {
		char *d_name;
	};
}

bool matching() {
	string s("LT_10-10-10-0-0-0_10000000_123_BASHNW.png");
	regex pattern("^LT_(\\d{1,3}-){5}\\d{1,3}_\\d{8}_\\d{1,3}_?B?A?S?H?N?M?T?G?F?W?.png$");
	return regex_match(s, pattern);
}

bool findMinDiff(vector<vector<double>> & data) {
	
	ifstream filename("..\\image_folder\\reference.txt");
	return true;
}

int reading(Mat &image, int i, int numtest) {
	stringstream filename;
	filename << "..\\image_folder\\image_" << i << "_iter_" << numtest << ".png";
	image = imread(filename.str().c_str(), CV_LOAD_IMAGE_COLOR);
	return 0;
}



streampos save_intensity_map(const Mat &gray_mat) {
	ofstream intensity_map("intensity.bin", ofstream::binary);
	char* dimension = new char[6];
	dimension[0] = (gray_mat.rows & 0xF);
	dimension[1] = ((gray_mat.rows >> 4) & 0xF);
	dimension[2] = ((gray_mat.rows >> 8) & 0xF);
	dimension[3] = (gray_mat.cols & 0xF);
	dimension[4] = ((gray_mat.cols >> 4) & 0xF);
	dimension[5] = ((gray_mat.cols >> 8) & 0xF);
	cout << dimension[0];
	intensity_map.write(dimension, 6);
	for (int row = 0; row < 4; row++) {
		char* row_data = new char[gray_mat.cols];
		for (int col = 0; col < 4; col++) {
			row_data[col] = gray_mat.at<unsigned char>(row, col) - 128;
			cout << row << " " << col << " " << int((row_data[col])) + 128 << endl;
		}
		intensity_map.write(row_data, sizeof(char) * gray_mat.rows);
		delete[] row_data;
	}
	intensity_map.seekp(0, intensity_map.end);
	streampos pos1 = intensity_map.tellp();
	intensity_map.seekp(0, intensity_map.beg);
	streampos pos2 = intensity_map.tellp();
	streampos size = pos1 - pos2;
	intensity_map.close();
	return size;
}

void load_intensity_map(const Mat &mat) {
	string locale = "intensity.bin";
	ifstream intensity_map(locale.c_str(), istream::binary);
	char* dimension = new char[6];
	intensity_map.readsome(dimension, 4);
	int row = 0;
	int col = 0;
	row = ((dimension[2] << 8) & (dimension[1] << 4) & dimension[0]);
	col = ((dimension[5] << 8) & (dimension[4] << 4) & dimension[3]);
	cout << hex << row << " " << col << endl;
}

double calculate_statistics(Mat &mat, const int rowstart, const int colstart, const int rowend, const int colend) {
	Mat roi(mat, Rect(rowstart, colstart, rowend, colend));
	//namedWindow("display", WINDOW_AUTOSIZE);
	//imshow("display", roi);
	//waitKey(0);
	Scalar averageIntensity = mean(roi);
	return averageIntensity[0];
}

//get mapping array
void get_mapping_array(double (&dest)[ARRAYSIZE], Mat &src) {
	for (int i = 0; i < ARRAYSIZE; i++) {
		dest[i] = calculate_statistics(src, 0, 0, src.rows - 1, src.cols - 1);
	}
}


void get_intensity(Mat &gray_mat, const int rowstart, const int colstart, const int rowend, const int colend, vector<tuple<int, double>> &result, vector<vector<double>> &rgb, int iter) {
	Mat roi(gray_mat, Rect(rowstart, colstart, rowend, colend));
	//namedWindow("display", WINDOW_AUTOSIZE);
	//imshow("display", roi);
	//waitKey(0);
	Scalar intensity = mean(roi);
	tuple<int, double> temp(iter, intensity[0]);
	result.push_back(temp);

	vector<double> temp2;
	temp2.resize(3);
	temp2[0] = gray_mat.at<Vec3b>(colstart, rowstart)[2];
	temp2[1] = gray_mat.at<Vec3b>(colstart, rowstart)[1];
	temp2[2] = gray_mat.at<Vec3b>(colstart, rowstart)[0];
	rgb.push_back(temp2);
}


int main() {
	matching();
	vector<tuple<int, double>> result;
	vector<vector<double>> rgb;
	int test;
	Mat image;
	int numtest;
	ifstream random("..\\image_folder\\take_num.config");
	random >> numtest;
	random.close();
	stringstream filename;
	filename << "..\\image_folder\\intensity_curve_gain_" << numtest <<".txt";
	ofstream outfile("..\\image_folder\\intensity_curve_gain.txt");
	for (int i = 1; i < ARRAYSIZE + 1; i++) {
		try {
			cout << i << "th iteration" << endl;
			reading(image, i, numtest);
			get_intensity(image, ROI1, result, rgb, i);
			get_intensity(image, ROI2, result, rgb, i);
			get_intensity(image, ROI3, result, rgb, i);
			get_intensity(image, ROI4, result, rgb, i);
			get_intensity(image, ROI5, result, rgb, i);
			get_intensity(image, ROI6, result, rgb, i);
			get_intensity(image, ROI7, result, rgb, i);
			get_intensity(image, ROI8, result, rgb, i);
			get_intensity(image, ROI9, result, rgb, i);
			get_intensity(image, ROI10, result, rgb, i);
			get_intensity(image, ROI11, result, rgb, i);
			get_intensity(image, ROI12, result, rgb, i);
			get_intensity(image, ROI13, result, rgb, i);
			get_intensity(image, ROI14, result, rgb, i);
			get_intensity(image, ROI15, result, rgb, i);
			get_intensity(image, ROI16, result, rgb, i);
			get_intensity(image, ROI17, result, rgb, i);
			get_intensity(image, ROI18, result, rgb, i);
			get_intensity(image, ROI19, result, rgb, i);
			get_intensity(image, ROI20, result, rgb, i);
			get_intensity(image, ROI21, result, rgb, i);
			get_intensity(image, ROI22, result, rgb, i);
			get_intensity(image, ROI23, result, rgb, i);
			get_intensity(image, ROI24, result, rgb, i);
		}
		catch(exception &exc) {
			cout << "Not found " << i << " th picture." << endl;
		}
	}
	vector<vector<double>> data;
	vector<double> temporary;
	for (int i = 0; i < result.size(); i++) {
		outfile << get<0>(result[i]) << " " << get<1>(result[i]) << " " << rgb[i][0] << " " << rgb[i][1] << " " << rgb[i][2] << endl;
		temporary.push_back(get<1>(result[i]));
		temporary.push_back(rgb[i][0]);
		temporary.push_back(rgb[i][1]);
		temporary.push_back(rgb[i][2]);
		data.push_back(temporary);
	}
	outfile.close();

	findMinDiff(data);

	cout << "Writing complete.\n" << endl;
	//cin >> test;
	//cin.ignore();
	return 0;
}