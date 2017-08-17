#pragma once
#include "FlyCapture2.h"
#include <sstream>
#include <iostream>
#include <vector>

using namespace FlyCapture2;
using namespace std;

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

class LightStageCamera
{
	
public:
	LightStageCamera();
	~LightStageCamera();
	void PrintBuildInfo16();
	void init_control();
	void connect();
	void getCameraInfo();
	void PrintCameraInfo16();
	void PrintError16(Error error);
	void StatusQuery();
	int getNumImages();
	void setNumImages(int num);
	void StartCapture();
	Camera * getCamera();
	void performFunc(Error &error);
	void LightStageCamera::RetrieveBuffer(std::vector<Image> & vecImages, Image * rawImage, int index);
	void StopCapture();
	void Disconnect();
	int getNumCamera();
	PGRGuid * getGuid();
	BusManager * getBusManager();
	

private:
	Error status;
	BusManager busMgr;
	FC2Version fc2Version;
	unsigned int numCameras;
	PGRGuid guid;
	Camera camera;
	CameraInfo camInfo;
	int numImages;
	Property* prop = new Property[10];
};

