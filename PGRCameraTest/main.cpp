#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <vector>

#include "kinet.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "FlyCapture2.h"

#define turn_on_w 255,255,0
#define turn_on_rgb 255,255,255
#define turn_off 0,0,0

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace FlyCapture2;
using namespace std;

void PrintBuildInfo()
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

void PrintCameraInfo(CameraInfo* pCamInfo)
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

void PrintError(Error error)
{
	error.PrintErrorTrace();
}


int RunSingleCamera(PGRGuid guid)
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
	Old_PowerSupply *arc_0_c = new Old_PowerSupply("10.33.157.55");
	Old_PowerSupply *arc_0_w = new Old_PowerSupply("10.33.157.51");

	Old_FixtureRGB *fix_w_1 = new Old_FixtureRGB(0, 0, 0, 0);
	Old_FixtureRGB *fix_rgb_1 = new Old_FixtureRGB(6, 0, 0, 0);
	Old_FixtureRGB *fix_w_2 = new Old_FixtureRGB(12, 0, 0, 0);
	Old_FixtureRGB *fix_rgb_2 = new Old_FixtureRGB(18, 0, 0, 0);
	Old_FixtureRGB *fix_w_3 = new Old_FixtureRGB(0, 0, 0, 0);
	Old_FixtureRGB *fix_rgb_3 = new Old_FixtureRGB(6, 0, 0, 0);
	Old_FixtureRGB *fix_w_4 = new Old_FixtureRGB(12, 0, 0, 0);
	Old_FixtureRGB *fix_rgb_4 = new Old_FixtureRGB(18, 0, 0, 0);

	arc_0_c->addOld_Fixture(fix_w_1);
	arc_0_c->addOld_Fixture(fix_rgb_1);
	arc_0_c->addOld_Fixture(fix_w_2);
	arc_0_c->addOld_Fixture(fix_rgb_2);

	arc_0_w->addOld_Fixture(fix_w_3);
	arc_0_w->addOld_Fixture(fix_rgb_3);
	arc_0_w->addOld_Fixture(fix_w_4);
	arc_0_w->addOld_Fixture(fix_rgb_4);

	arc_0_c->go();
	arc_0_w->go();

	// CAMERA
	const int k_numImages = 2;

	Error error;
	Camera cam;

	// Connect to a camera
	error = cam.Connect(&guid);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Get the camera information
	CameraInfo camInfo;
	error = cam.GetCameraInfo(&camInfo);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	PrintCameraInfo(&camInfo);

	// Start capturing images
	error = cam.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	Image rawImage;
	std::vector<Image> vecImages;
	vecImages.resize(k_numImages);


	// Capture first image (all white LEDs on)
	fix_w_1->set_rgb(turn_on_w);
	fix_w_2->set_rgb(turn_on_w);
	fix_w_3->set_rgb(turn_on_w);
	fix_w_4->set_rgb(turn_on_w);
	arc_0_c->go();
	arc_0_w->go();

	Sleep(100); // wait for the lights to turn on
	error = cam.RetrieveBuffer(&rawImage);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
	}
	cout << "Grabbed first image" << endl;
	vecImages[0].DeepCopy(&rawImage);

	Sleep(1000);
	// Capture second image (all RGB LEDs on)
	fix_w_1->set_rgb(turn_off);
	fix_w_2->set_rgb(turn_off);
	fix_w_3->set_rgb(turn_off);
	fix_w_4->set_rgb(turn_off);
	fix_rgb_1->set_rgb(turn_on_rgb);
	fix_rgb_2->set_rgb(turn_on_rgb);
	fix_rgb_3->set_rgb(turn_on_rgb);
	fix_rgb_4->set_rgb(turn_on_rgb);
	arc_0_c->go();
	arc_0_w->go();

	Sleep(100); // wait for the lights to turn on

	error = cam.RetrieveBuffer(&rawImage);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
	}
	cout << "Grabbed second image" << endl;
	vecImages[1].DeepCopy(&rawImage);

	Sleep(1000);
	
	// turn off all lights
	fix_w_1->set_rgb(turn_off);
	fix_w_2->set_rgb(turn_off);
	fix_w_3->set_rgb(turn_off);
	fix_w_4->set_rgb(turn_off);

	fix_rgb_1->set_rgb(turn_off);
	fix_rgb_2->set_rgb(turn_off);
	fix_rgb_3->set_rgb(turn_off);
	fix_rgb_4->set_rgb(turn_off);
	arc_0_c->go();
	arc_0_w->go();


	// Stop capturing images
	error = cam.StopCapture();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Disconnect the camera
	error = cam.Disconnect();
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	// Save images to disk
	for (int imageCnt = 0; imageCnt < k_numImages; imageCnt++)
	{
		// Create a converted image
		Image convertedImage;

		// Convert the raw image
		error = vecImages[imageCnt].Convert(PIXEL_FORMAT_RGB, &convertedImage);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return -1;
		}

		// Create a unique filename
		ostringstream filename;
		filename << "image_" << imageCnt + 1 << ".png";

		// Save the image. If a file format is not passed in, then the file
		// extension is parsed to attempt to determine the file format.
		error = convertedImage.Save(filename.str().c_str());
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return -1;
		}
	}

	return 0;
}

int main8(int argc, char** argv)
{
	PrintBuildInfo();

	Error error;
	BusManager busMgr;
	unsigned int numCameras;
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}

	cout << "Number of cameras detected: " << numCameras << endl;

	for (unsigned int i = 0; i < numCameras; i++)
	{
		PGRGuid guid;
		error = busMgr.GetCameraFromIndex(i, &guid);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
			return -1;
		}

		RunSingleCamera(guid);
	}

	cout << "Done! Press Enter to exit..." << endl;
	cin.ignore();

	return 0;

}