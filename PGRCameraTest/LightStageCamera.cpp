#include "LightStageCamera.h"
using namespace std;


LightStageCamera::LightStageCamera()
{
	PrintBuildInfo16();
	error = busMgr.GetNumOfCameras(&numCameras);
	if (error != PGRERROR_OK) {
		PrintError16(error);
	}
	cout << "Number of cameras detected: " << numCameras << endl;
}


LightStageCamera::~LightStageCamera()
{
}

void LightStageCamera::PrintBuildInfo16()
{
	Utilities::GetLibraryVersion(&fc2Version);
	ostringstream version;
	version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
	cout << version.str() << endl;

	ostringstream timeStamp;
	timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
	cout << timeStamp.str() << endl << endl;
}

Error LightStageCamera::init_control(Property(&prop)[10], Camera & cam, Error & error)
{
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

void LightStageCamera::PrintCameraInfo16(CameraInfo * pCamInfo)
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

void LightStageCamera::PrintError16(Error error)
{
	error.PrintErrorTrace();
}
