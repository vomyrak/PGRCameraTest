#include "LightStageCamera.h"
using namespace std;


LightStageCamera::LightStageCamera()
{
	PrintBuildInfo16();
	status = busMgr.GetNumOfCameras(&numCameras);
	StatusQuery();
	cout << "Number of cameras detected: " << numCameras << endl;
	camera.resize(numCameras);
	prop.resize(numCameras);
	getCameraFromIndex();
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

void LightStageCamera::init_control()
{
	prop[currentCam][0].type = BRIGHTNESS;
	prop[currentCam][1].type = AUTO_EXPOSURE;
	prop[currentCam][2].type = SHARPNESS;
	prop[currentCam][3].type = HUE;
	prop[currentCam][4].type = SATURATION;
	prop[currentCam][5].type = GAMMA;
	prop[currentCam][6].type = SHUTTER;
	prop[currentCam][7].type = GAIN;
	prop[currentCam][8].type = FRAME_RATE;
	prop[currentCam][9].type = WHITE_BALANCE;
	for (size_t i = 0; i < 10; i++) {
		prop[currentCam][i].absControl = true;
		prop[currentCam][i].autoManualMode = false;
		prop[currentCam][i].present = true;
		prop[currentCam][i].onOff = true;
	}
	prop[currentCam][brightness].absValue = 0;
	prop[currentCam][auto_exposure].absValue = 0;
	prop[currentCam][sharpness].absControl = false;
	//sharpness must have absControl set to false
	prop[currentCam][sharpness].valueA = 1024;
	prop[currentCam][hue].absValue = 0;
	prop[currentCam][saturation].absValue = 100;
	prop[currentCam][gamma].absValue = 1;
	prop[currentCam][shutter].absValue = 18.058;//30;
	prop[currentCam][gain].absValue = 0;
	prop[currentCam][frame_rate].absValue = 82;//55.161;//80;
									   //white balance must have absControl set to false
	prop[currentCam][white_balance].absControl = false;
	prop[currentCam][white_balance].onOff = false;
	prop[currentCam][white_balance].valueA = 482;
	prop[currentCam][white_balance].valueB = 762;
	for (size_t i = 0; i < 10; i++) {
		status = camera[currentCam].SetProperty(prop[currentCam] + i);
		StatusQuery();
	}
}

void LightStageCamera::connect()
{
	status = camera[currentCam].Connect(&guid);
}

void LightStageCamera::getCameraInfo()
{
	status = camera[currentCam].GetCameraInfo(&camInfo);
}

void LightStageCamera::PrintCameraInfo16()
{

	cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number -" << camInfo.serialNumber << endl;
	cout << "Camera model - " << camInfo.modelName << endl;
	cout << "Camera vendor - " << camInfo.vendorName << endl;
	cout << "Sensor - " << camInfo.sensorInfo << endl;
	cout << "Resolution - " << camInfo.sensorResolution << endl;
	cout << "Firmware version - " << camInfo.firmwareVersion << endl;
	cout << "Firmware build time - " << camInfo.firmwareBuildTime << endl << endl;
}

void LightStageCamera::PrintError16()
{
	status.PrintErrorTrace();
}

void LightStageCamera::StatusQuery()
{
	if (status != PGRERROR_OK) {
		PrintError16();
		cerr << "Interrupted" << endl;
	}
}

int LightStageCamera::getNumImages()
{
	return numImages;
}

void LightStageCamera::setNumImages(int num)
{
	numImages = num;
}

void LightStageCamera::StartCapture()
{
	status = camera[currentCam].StartCapture();
	StatusQuery();
}

Camera * LightStageCamera::getCurrentCamera()
{
	return &camera[currentCam];
}

void LightStageCamera::performFunc(Error & error)
{
	status = error;
	StatusQuery();
}

void LightStageCamera::RetrieveBuffer(std::vector<Image> & vecImages, Image * rawImage, int index)
{
	status = camera[currentCam].RetrieveBuffer(rawImage);
	StatusQuery();
	printf("Grabbed %dth image\n", index + 1);
	vecImages[index].DeepCopy(rawImage);
}

void LightStageCamera::StopCapture()
{
	status = camera[currentCam].StopCapture();
	StatusQuery();
}

void LightStageCamera::Disconnect()
{
	status = camera[currentCam].Disconnect();
	StatusQuery();
}

int LightStageCamera::getNumCamera()
{
	return numCameras;
}

void LightStageCamera::getCameraFromIndex(int index)
{
	status = busMgr.GetCameraFromIndex(index, &guid);
	StatusQuery();
	currentCam = index;
}

PGRGuid * LightStageCamera::getGuid()
{
	return &guid;
}

BusManager * LightStageCamera::getBusManager()
{
	return &busMgr;
}

Property * LightStageCamera::getProp()
{
	return prop[currentCam];
}
