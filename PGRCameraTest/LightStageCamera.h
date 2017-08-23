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

	/*Print software build information*/
	void PrintBuildInfo16();

	/*Initialise currently selected camera*/
	void initCurrentCamera();

	/*Connect selected camera*/
	void connect();

	/*Get the information of the selected camera*/
	void getCameraInfo();

	/*Print the infromation of the selected camera*/
	void PrintCameraInfo16();

	/*Print status error*/
	void PrintError16();

	/*Check current status*/
	void StatusQuery();

	/*Get the number of images to capture*/
	int getNumImages();

	/*Set the number of images to capture*/
	void setNumImages(int num);

	/*Get the camera ready for capturing*/
	void StartCapture();

	/*Interface with original Flycap SDK*/
	void performFunc(Error &error);

	/*Retrieve buffered image from camera*/
	void RetrieveBuffer(std::vector<Image> & vecImages, Image * rawImage, int index);
	
	/*Stop capturing image*/
	void StopCapture();

	/*Disconnect current camera*/
	void Disconnect();

	/*Get the number of cameras currently cunnected*/
	int getNumCamera();

	/*Select one of connected camera by index*/
	void getCameraFromIndex(int index = 0);

	/*Return the pointer to currently selected camera*/
	Camera * getCurrentCamera();

	/*Return the pointer to GUID*/
	PGRGuid * getGuid();

	/*Return the pointer to camera bus manager*/
	BusManager * getBusManager();

	/*Return the pointer to the property array of currently selected camera*/
	Property * getProp();

	/*Return the index of currently selected camera*/
	int getCurrentCamIndex();

private:
	Error status;
	BusManager busMgr;
	FC2Version fc2Version;
	unsigned int numCameras;
	PGRGuid guid;
	std::vector<Camera*> camera;
	CameraInfo camInfo;
	int numImages;
	int currentCam;
	std::vector<Property*> prop;
};

