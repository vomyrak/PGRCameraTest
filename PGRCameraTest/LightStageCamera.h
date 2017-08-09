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
	void PrintCameraInfo16(CameraInfo* pCamInfo);
	void PrintError16(Error error);

private:
	FC2Version fc2Version;
	Error error;
	BusManager busMgr;
	unsigned int numCameras;
	PGRGuid guid;
	Camera cam;
	CameraInfo camInfo;
	Property* prop = new Property[10];
};

