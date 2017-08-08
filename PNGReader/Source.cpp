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
#define ARRAYSIZE 32
#define ROI 900, 720, 250, 180	//x-coord, y-coord, width, height
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



int reading(Mat &image, int i) {
	stringstream filename;
	filename << "C:\\Users\\VomyraK\\Documents\\Visual Studio 2015\\Projects\\PGRCameraTest\\PGRCameraTest\\channel_1_" << i << ".png";
	image = imread(filename.str().c_str(), CV_LOAD_IMAGE_GRAYSCALE);
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

void get_intensity(Mat &gray_mat, const int rowstart, const int colstart, const int rowend, const int colend, vector<tuple<int, double>> &result, int iter) {
	double intensity = calculate_statistics(gray_mat, rowstart, colstart, rowend, colend);
	tuple<int, double> temp(iter, intensity);
	result.push_back(temp);
}


int main() {
	matching();
	vector<tuple<int, double>> result;
	int test;
	Mat image;
	ofstream outfile("C:\\Users\\VomyraK\\Documents\\Visual Studio 2015\\Projects\\PGRCameraTest\\PNGReader\\intensity_curve_gain.txt");
	for (int i = 0; i <= ARRAYSIZE * 8; i++) {
		try {
			cout << i << "th iteration" << endl;
			reading(image, i);

			get_intensity(image, ROI, result, i);

		}
		catch(exception &exc) {
			cout << "Not found " << i << " th picture." << endl;
		}
	}
	for (int i = 0; i < result.size(); i++) {
		outfile << get<0>(result[i]) << " " << get<1>(result[i]) << endl;
	}
	outfile.close();
	cout << "Writing complete.\n" << endl;
	cin >> test;
	cin.ignore();
	return 0;
}