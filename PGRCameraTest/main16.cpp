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


int RunSingleCamera16(LightStageCamera &cam, LightStage &stage)
{
	// CAMERA
	cam.setNumImages(32);

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

	//for testing fine control
	/*
	for (int i = 0; counter < k_numImages;)
	{
		error = cam.RetrieveBuffer(&rawImage);
		if (error != PGRERROR_OK)
		{
			PrintError16(error);
			error = cam.Disconnect();
		}
		printf("Grabbed %dth image\n", counter+1);
		vecImages[counter].DeepCopy(&rawImage);
		Sleep(200);

		fix_w_1->set_rgb2(128, 128, 128, i, i, i);
		fix_w_2->set_rgb2(128, 128, 128, i, i, i);
		fix_w_3->set_rgb2(128, 128, 128, i, i, i);
		fix_w_4->set_rgb2(128, 128, 128, i, i, i);
		arc_0_c->go();
		arc_0_w->go();
		Sleep(200);
		i += 255 / k_numImages;
		counter += 1;
	}
	*/
	for (int i = 0; i < cam.getNumImages();) {
		
		stage.adjustAll(i, 0, 0, 0, 0, 0);
		/*
		fix_rgb_1->set_rgb2(i, 0, 0, 0, 0, 0);
		fix_rgb_2->set_rgb2(i, 0, 0, 0, 0, 0);
		fix_rgb_3->set_rgb2(i, 0, 0, 0, 0, 0);
		fix_rgb_4->set_rgb2(i, 0, 0, 0, 0, 0);
		*/
		Sleep(150);
		cam.RetrieveBuffer(&rawImage);
		printf("Grabbed %dth image\n", i + 1);
		vecImages[i].DeepCopy(&rawImage);
		Sleep(200);
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
		ImageMetadata data = vecImages[imageCnt].GetMetadata();
		cam.updateStatus(vecImages[imageCnt].Convert(PIXEL_FORMAT_RGB, &convertedImage));
		cam.StatusQuery();
		ImageStatistics stats;
		stats.EnableAll();
		convertedImage.CalculateStatistics(&stats);
		stats.EnableAll();
		float mean[3];
		cam.updateStatus(stats.GetMean(stats.RED, mean));
		cam.updateStatus(stats.GetMean(stats.RED, mean+1));
		cam.updateStatus(stats.GetMean(stats.RED, mean+2));
		printf("%dth picture: \n", imageCnt + 1);
		cout << "Exposure " << data.embeddedExposure << endl;
		cout << "Brightness " << data.embeddedBrightness << endl;
		cout << "White Balance " << data.embeddedWhiteBalance << endl;
		cout << "Mean " << mean << endl;
		// Create a unique filename
		ostringstream filename;
		filename << "..\\image_folder\\image_" << imageCnt + 1 << "_first_light" << ".png";

		// Save the image. If a file format is not passed in, then the file
		// extension is parsed to attempt to determine the file format.
		cam.updateStatus(convertedImage.Save(filename.str().c_str()));
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
	LightStage stage;
	int temp, temp2;
	uint8_t config[6];
	config[0] = 50;
	config[1] = 50;
	config[2] = 50;
	config[3] = 0;
	config[4] = 128;
	config[5] = 0;
	stage(2, 5)->set_config(config);
	stage(5, 2)->set_config(config);
	stage(7, 3)->set_config(config);
	stage.go();
	int a = 1;
	LightStageCamera cam;
	for (unsigned int i = 0; i < cam.getNumCamera(); i++) {
		cam.updateStatus(cam.getBusManager()->GetCameraFromIndex(i, cam.getGuid()));

		RunSingleCamera16(cam, stage);
		
	}
	while (true) {
		stage.rotation(1);
		Sleep(100);
	}
	
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