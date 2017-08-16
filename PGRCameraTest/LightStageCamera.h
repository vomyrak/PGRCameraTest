#pragma once
#include "FlyCapture2.h"
#include <sstream>
#include <iostream>

using namespace FlyCapture2;
using namespace std;

class LightStageCamera
{
	enum {
		brightness,
		auto_exposure,
		sharpness,
		hue,
		saturation,
		gamma,
		shutter,
		gain,
		frame_rate,
		white_balance
	};
public:
	LightStageCamera();
	~LightStageCamera();
	void PrintBuildInfo16();
	Error init_control(Property(&prop)[10], Camera &cam, Error &error);
	void connect();
	void getCameraInfo();
	void PrintCameraInfo16();
	void PrintError16(Error error);
	void StatusQuery();
	int getNumImages();
	void setNumImages(int num);
	void StartCapture();
	Camera * getCamera();
	void setStatus(Error &error);
	void RetrieveBuffer(Image * rawImage);
	void StopCapture();
	void Disconnect();
	int getNumCamera();
	PGRGuid * getGuid();
	BusManager * getBusManager();
	Property* prop = new Property[10];

private:
	Error status;
	BusManager busMgr;
	FC2Version fc2Version;
	unsigned int numCameras;
	PGRGuid guid;
	Camera camera;
	CameraInfo camInfo;
	int numImages;
	int numCamera;
};

