#include "stdafx.h"
#include <fstream>
#include <cstring>
#include <cmath>
#include <tuple>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
//#include "test_interface.h"
#include "light_stage.h"
#include "LightStageCamera.h"
#include "FlyCapture2.h"
//#include "GUI.h"

#define turn_on 255,255,255,255,255,255
#define turn_half_lit 128,128,128,128,128,128
#define turn_off 0,0,0,0,0,0

#define ROI 900, 720, 250, 180

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace FlyCapture2;
using namespace std;

LightStage stage;
LightStageCamera cam;

bool check_matching() {
	return true;
}
void test_cases(int i) {
	
	switch (i) {
		
		case 0:
			stage.adjustRGB(20, 20, 20, 0, 0, 0);
			//stage.adjustRGB(40, 40, 40, 0, 0, 0);
			break;
		case 1:
			stage.adjustWhite(20, 20, 20, 0, 0, 0);
			//stage.adjustWhite(40, 40, 40, 0, 0, 0);
			break;
		case 2:
			stage.adjustRGB(10, 10, 10, 0, 0, 0);
			stage.adjustWhite(10, 10, 10, 0, 0, 0);
			//stage.adjustRGB(40, 40, 40, 0, 0, 0);
			break;
		case 3:
			stage.adjustRGB(60, 0, 0, 0, 0, 0);
			break;
		case 4:
			stage.adjustRGB(0, 60, 0, 0, 0, 0);
			break;
		case 5:
			stage.adjustRGB(0, 0, 60, 0, 0, 0);
			break;
		case 6:
			stage.adjustWhite(60, 0, 0, 0, 0, 0);
			break;
		case 7:
			stage.adjustWhite(0, 60, 0, 0, 0, 0);
			break;
		case 8:
			stage.adjustWhite(0, 0, 60, 0, 0, 0);
			break;
			
		
	}
	/*
	stage.adjustWhite(20 + i * 2, 0, 60 - i * 2, 0, 0, 0);
	*/

}
int RunSingleCamera16(int &count)
{

	cam.init_control();
	// CAMERA
	cam.setNumImages(9);

	// Connect to a camera
	cam.connect();
	cam.StatusQuery();

	// Get the camera information
	cam.getCameraInfo();
	cam.StatusQuery();
	cam.PrintCameraInfo16();

	// Set the camera control
	


	// Start capturing images
	cam.StartCapture();
	cam.StatusQuery();
	Image rawImage;
	std::vector<Image> vecImages;
	vecImages.resize(cam.getNumImages());
	int counter = 0;

	stage.adjustAll(turn_off);
	Sleep(100); // wait for the lights to turn on

	for (int i = 0; i < cam.getNumImages();) {//cam.getNumImages()
		
		test_cases(i);
		Sleep(150);
		cam.RetrieveBuffer(vecImages, &rawImage, i);
		stage.adjustAll(turn_off);
		i++;
	}

	// turn off all lights
	stage.adjustAll(turn_off);


	// Stop capturing images
	cam.StopCapture();

	// Disconnect the camera
	cam.Disconnect();

	// Save images to disk
	for (int imageCnt = 0; imageCnt < cam.getNumImages(); imageCnt++)
	{
		// Create a converted image
		Image convertedImage;
		// Convert the raw image
		cam.performFunc(vecImages[imageCnt].Convert(PIXEL_FORMAT_RGB, &convertedImage));
		cam.StatusQuery();
		printf("%dth picture, take %d\n", imageCnt + 1, count);

		// Create a unique filename
		ostringstream filename;
		filename << "..\\image_folder\\image_" << imageCnt + 1 << "_iter_" << count << ".png";

		// Save the image. If a file format is not passed in, then the file
		// extension is parsed to attempt to determine the file format.
		cam.performFunc(convertedImage.Save(filename.str().c_str()));
	}

	return 0;
}

/*
int InterfaceTest(PGRGuid guid) {
	// LIGHTS
	WSADATA wsaData;
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	// initialize lights
	

	arc_0_c->go();

	// CAMERA
	const int k_numImages = 32;

	Error error;
	Camera cam;

	// Connect to a camera
	error = cam.Connect(&guid);
	if (error != PGRERROR_OK)
	{
		PrintError16(error);
		return -1;
	}

	// Get the camera information
	CameraInfo camInfo;
	error = cam.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK)
	{
		PrintError16(error);
		return -1;
	}
	PrintCameraInfo16(&camInfo);


	// Initialise the camera control
	Property prop[10];
	init_control(prop, cam, error);

	test_interface test(cam, Old_Fixture, prop);
	test.set_light_value(turn_half_lit);
	arc_0_c->go();
	arc_0_w->go();

	error = cam.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError16(error);
		return -1;
	}

	Image rawImage;
	std::vector<Image> vecImages;
	vecImages.resize(k_numImages);
	int counter = 0;
	string get[k_numImages];
	//set one parameter
	test.set_light_step(10, 100, 0b111, k_numImages);
	for (size_t i = 0; i < k_numImages; i++) {
		test.set_light_value(test.min + int(test.stepsize * i), test.min + int(test.stepsize * i), test.min + int(test.stepsize * i), 0, 0, 0);
		arc_0_c->go();
		arc_0_w->go();
		Sleep(100);
		error = cam.RetrieveBuffer(&rawImage);
		if (error != PGRERROR_OK) {
			PrintError16(error);
			cam.Disconnect();
		}
		vecImages[i].DeepCopy(&rawImage);
		test.set_file_name();
		get[i] = test.get_file_name();
		cout << get[i] << endl;
	}
	Image convertedImage;
	for (auto i = 0; i < k_numImages; i++) {
		error = vecImages[i].Convert(&convertedImage);
		error = convertedImage.Save((get[i] + ".png").c_str());
	}
	//system("python ..\\PythonApplication1\\ResponseAnalysis.py");
	return 0;
}
*/
int main(int argc, char** argv){

	// LIGHTS
	//GUI gui(argc, argv);
	WSADATA wsaData;
	// Initialize Winsock
	
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	while (true) {
		stage(8, 5)->set_rgb2(10, 10, 10, 10, 10, 10);
		stage.go(7);
		stage(8, 5)->set_rgb2(turn_off);
		stage.go(7);
		Sleep(20);
		//stage[5][5]->set_rgb2(turn_off);
		//Sleep(20);
	}
	stage.adjustAll(turn_off);
	cam.performFunc(cam.getBusManager()->GetCameraFromIndex(0, cam.getGuid()));
	int count = 0;
	
	while (true) {
		RunSingleCamera16(count);
		ofstream configuration;
		string name("..\\image_folder\\take_num.config");
		configuration.open(name.c_str());
		configuration << count;
		configuration.close();
		//system("..\\x64\\Debug\\PNGReader.exe");
		if (check_matching() == false) {
			count += 1;
		}
		else {
			break;
		}
	}
	
	/*
	while (true) {
		stage.rotation(1);
		Sleep(100);
	}
	*/
	//fine step increase
	/*
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			//stage.adjustAll(0, 0, 0, j, 0, 0);
			//stage.adjustAll(i, 0, 0, 0, 0, 0);
		}
	}
	*/

	//control one light
	/*
	while (true){
		cin >> temp >> temp2;
		stage(temp, temp2)->set_rgb2(turn_on);
		stage.go();
	}
	*/
	/*
	PrintBuildInfo16();

	Error error;
	BusManager busMgr;
	unsigned int numCameras;
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK)
	{
		PrintError16(error);
		return -1;
	}

	cout << "Number of cameras detected: " << numCameras << endl;
	for (unsigned int i = 0; i < numCameras; i++)
	{
		PGRGuid guid;
		error = busMgr.GetCameraFromIndex(i, &guid);
		if (error != PGRERROR_OK)
		{
			PrintError16(error);
			return -1;
		}

		InterfaceTest(guid);
	}
	*/
	cout << "Done! Press Enter to exit..." << endl;
	cin.ignore();
	return 0;

}