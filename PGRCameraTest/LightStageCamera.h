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
	void initCurrentCamera();
	void connect();
	void getCameraInfo();
	void PrintCameraInfo16();
	void PrintError16();
	void StatusQuery();
	int getNumImages();
	void setNumImages(int num);
	void StartCapture();
	void performFunc(Error &error);
	void RetrieveBuffer(std::vector<Image> & vecImages, Image * rawImage, int index);
	void StopCapture();
	void Disconnect();
	int getNumCamera();
	void getCameraFromIndex(int index = 0);
	Camera * getCurrentCamera();
	PGRGuid * getGuid();
	BusManager * getBusManager();
	Property * getProp();
	

private:
	Error status;
	BusManager busMgr;
	FC2Version fc2Version;
	unsigned int numCameras;
	PGRGuid guid;
	std::vector<Camera> camera;
	CameraInfo camInfo;
	int numImages;
	int currentCam;
	std::vector<Property[10]> prop;
};

