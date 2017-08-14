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
#include "FlyCapture2.h"
#include "GUI.h"

#define turn_on 255,255,255,255,255,255
#define turn_half_lit 128,128,128,128,128,128
#define turn_off 0,0,0,0,0,0

#define ROI 900, 720, 250, 180

#define brightness 0
#define auto_exposure 1
#define sharpness 2
#define hue 3
#define saturation 4
#define gamma 5
#define shutter 6
#define gain 7
#define frame_rate 8
#define white_balance 9

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace FlyCapture2;
using namespace std;

void PrintBuildInfo16()
{
	FC2Version fc2Version;
	Utilities::GetLibraryVersion(&fc2Version);

	ostringstream version;
	version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
	cout << version.str() << endl;

	ostringstream timeStamp;
	timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
	cout << timeStamp.str() << endl << endl;
}

/**< AUTO_EXPOSURE, SHARPNESS, HUE, SATURATION, GAMMA CONTROLS SEEMS TO BE INEFFECTIVE TO THE CAMERA*/


void PrintCameraInfo16(CameraInfo* pCamInfo)
{
	cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number -" << pCamInfo->serialNumber << endl;
	cout << "Camera model - " << pCamInfo->modelName << endl;
	cout << "Camera vendor - " << pCamInfo->vendorName << endl;
	cout << "Sensor - " << pCamInfo->sensorInfo << endl;
	cout << "Resolution - " << pCamInfo->sensorResolution << endl;
	cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
	cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl << endl;

}

void PrintError16(Error error)
{
	error.PrintErrorTrace();
}

//initialise camera control

Error init_control(Property(&prop)[10], Camera &cam, Error &error) {
	prop[0].type = BRIGHTNESS;
	prop[1].type = AUTO_EXPOSURE;
	prop[2].type = SHARPNESS;
	prop[3].type = HUE;
	prop[4].type = SATURATION;
	prop[5].type = GAMMA;
	prop[6].type = SHUTTER;
	prop[7].type = GAIN;
	prop[8].type = FRAME_RATE;
	prop[9].type = WHITE_BALANCE;
	for (size_t i = 0; i < 10; i++) {
		prop[i].absControl = true;
		prop[i].autoManualMode = false;
		prop[i].present = true;
		prop[i].onOff = true;
	}
	prop[brightness].absValue = 0;
	prop[auto_exposure].absValue = 0;
	prop[sharpness].absControl = false;
	//sharpness must have absControl set to false
	prop[sharpness].valueA = 1024;
	prop[hue].absValue = 0;
	prop[saturation].absValue = 100;
	prop[gamma].absValue = 1;
	prop[shutter].absValue = 18.058;//30;
	prop[gain].absValue = 0;
	prop[frame_rate].absValue = 55.161;//80;
									   //white balance must have absControl set to false
	prop[white_balance].absControl = false;
	prop[white_balance].valueA = 482;
	prop[white_balance].valueB = 762;
	for (size_t i = 0; i < 10; i++) {
		error = cam.SetProperty(prop + i);
		if (error != PGRERROR_OK) {
			PrintError16(error);
			cout << "Failure at property" << i << endl;
		}
	}
	return error;
}

int RunSingleCamera16(PGRGuid guid)
{
	// LIGHTS
	WSADATA wsaData;
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	// initialize lights
	PowerSupply *arc_0_c = new PowerSupply("10.33.157.55");
	PowerSupply *arc_0_w = new PowerSupply("10.33.157.51");

	FixtureRGB16 *fix_w_1 = new FixtureRGB16(0, 0, 0, 0, 0, 0, 0);
	FixtureRGB16 *fix_rgb_1 = new FixtureRGB16(6, 0, 0, 0, 0, 0, 0);
	FixtureRGB16 *fix_w_2 = new FixtureRGB16(12, 0, 0, 0, 0, 0, 0);
	FixtureRGB16 *fix_rgb_2 = new FixtureRGB16(18, 0, 0, 0, 0, 0, 0);
	FixtureRGB16 *fix_w_3 = new FixtureRGB16(0, 0, 0, 0, 0, 0, 0);
	FixtureRGB16 *fix_rgb_3 = new FixtureRGB16(6, 0, 0, 0, 0, 0, 0);
	FixtureRGB16 *fix_w_4 = new FixtureRGB16(12, 0, 0, 0, 0, 0, 0);
	FixtureRGB16 *fix_rgb_4 = new FixtureRGB16(18, 0, 0, 0, 0, 0, 0);

	arc_0_c->addFixture(fix_w_1);
	arc_0_c->addFixture(fix_rgb_1);
	arc_0_c->addFixture(fix_w_2);
	arc_0_c->addFixture(fix_rgb_2);

	arc_0_w->addFixture(fix_w_3);
	arc_0_w->addFixture(fix_rgb_3);
	arc_0_w->addFixture(fix_w_4);
	arc_0_w->addFixture(fix_rgb_4);

	arc_0_c->go();
	arc_0_w->go();

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

	// Set the camera control



	// Start capturing images
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

	fix_w_1->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_w_2->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_w_3->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_w_4->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_rgb_1->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_rgb_2->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_rgb_3->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_rgb_4->set_rgb2(0, 0, 0, 0, 0, 0);
	arc_0_c->go();
	arc_0_w->go();
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
	for (int i = 0; i < k_numImages;) {
		
		fix_w_1->set_rgb2(i, 0, 0, 0, 0, 0);
		fix_w_2->set_rgb2(i, 0, 0, 0, 0, 0);
		fix_w_3->set_rgb2(i, 0, 0, 0, 0, 0);
		fix_w_4->set_rgb2(i, 0, 0, 0, 0, 0);
		/*
		fix_rgb_1->set_rgb2(i, 0, 0, 0, 0, 0);
		fix_rgb_2->set_rgb2(i, 0, 0, 0, 0, 0);
		fix_rgb_3->set_rgb2(i, 0, 0, 0, 0, 0);
		fix_rgb_4->set_rgb2(i, 0, 0, 0, 0, 0);
		*/
		arc_0_c->go();
		arc_0_w->go();
		Sleep(150);
		error = cam.RetrieveBuffer(&rawImage);
		if (error != PGRERROR_OK)
		{
			PrintError16(error);
			error = cam.Disconnect();
		}
		printf("Grabbed %dth image\n", i + 1);
		vecImages[i].DeepCopy(&rawImage);
		Sleep(200);
		i++;
	}

	// turn off all lights
	fix_w_1->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_w_2->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_w_3->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_w_4->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_rgb_1->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_rgb_2->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_rgb_3->set_rgb2(0, 0, 0, 0, 0, 0);
	fix_rgb_4->set_rgb2(0, 0, 0, 0, 0, 0);
	arc_0_c->go();
	arc_0_w->go();


	// Stop capturing images
	error = cam.StopCapture();
	if (error != PGRERROR_OK)
	{
		PrintError16(error);
		return -1;
	}

	// Disconnect the camera
	error = cam.Disconnect();
	if (error != PGRERROR_OK)
	{
		PrintError16(error);
		return -1;
	}

	// Save images to disk
	for (int imageCnt = 0; imageCnt < k_numImages; imageCnt++)
	{
		// Create a converted image
		Image convertedImage;
		// Convert the raw image
		ImageMetadata data = vecImages[imageCnt].GetMetadata();
		error = vecImages[imageCnt].Convert(PIXEL_FORMAT_RGB, &convertedImage);
		if (error != PGRERROR_OK)
		{
			PrintError16(error);
			return -1;
		}
		ImageStatistics stats;
		stats.EnableAll();
		convertedImage.CalculateStatistics(&stats);
		stats.EnableAll();
		float mean[3];
		error = stats.GetMean(stats.RED, mean);
		error = stats.GetMean(stats.RED, mean+1);
		error = stats.GetMean(stats.RED, mean+2);
		printf("%dth picture: \n", imageCnt + 1);
		cout << "Exposure " << data.embeddedExposure << endl;
		cout << "Brightness " << data.embeddedBrightness << endl;
		cout << "White Balance " << data.embeddedWhiteBalance << endl;
		cout << "Mean " << mean << endl;
		// Create a unique filename
		ostringstream filename;
		filename << "image_" << imageCnt + 1 << "_first_light" << ".png";

		// Save the image. If a file format is not passed in, then the file
		// extension is parsed to attempt to determine the file format.
		error = convertedImage.Save(filename.str().c_str());
		if (error != PGRERROR_OK)
		{
			PrintError16(error);
			return -1;
		}
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

	test_interface test(cam, fixture, prop);
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
	config[0] = 40;
	config[1] = 5;
	config[2] = 5;
	config[3] = 5;
	config[4] = 5;
	config[5] = 5;

	stage.go();
	
	while (true) {
		stage.rotation(1);
		Sleep(500);
	}
	
	/*
	for (int i = 0; i < 256; i++) {
		//stage.adjustAll(0, 0, 0, i, 0, 0);
		//stage.adjustAll(i, 0, 0, 0, 0, 0);
	}
	*/
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