#include "stdafx.h"
#include <winsock2.h>

#include <fstream>
#include <cmath>
#include <tuple>
#include <vector>
#include <ws2tcpip.h>
#include "FlyCapture2.h"
#include "GUI.h"


#define turn_on 255,255,255,255,255,255
#define turn_half_lit 128,128,128,128,128,128
#define turn_off 0,0,0,0,0,0



// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace FlyCapture2;
using namespace std;

LightStage stage;
LightStageCamera cam;
HWND * Texts = new HWND[6];
HWND * LightCtrl = new HWND[6];
wchar_t buf[4];
int pos = 0;
int activeButton = 0;
HWND * display = new HWND[336];
UINT8 currentConfig[6];
HWND * Label = new HWND[6];



int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	const wchar_t CLASS_NAME[] = L"Master";
	WNDCLASSW wc = {};
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = 0;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hinstance;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszMenuName = 0;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	RegisterClassW(&wc);

	WNDCLASSW wc2 = {};
	wc2.cbClsExtra = 0;
	wc2.cbWndExtra = 0;
	wc2.hIcon = 0;
	wc2.lpfnWndProc = WindowProc2;
	wc2.hInstance = hinstance;
	wc2.style = CS_HREDRAW | CS_VREDRAW;
	wc2.lpszMenuName = 0;
	wc2.lpszClassName = L"Child";
	wc2.hCursor = LoadCursor(0, IDC_ARROW);
	wc2.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc2.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	RegisterClassW(&wc2);

	HWND hwnd = CreateWindowExW(
		NULL,
		CLASS_NAME,
		L"Light Stage Controller",
		WS_OVERLAPPEDWINDOW  ^ WS_THICKFRAME | WS_VISIBLE | WS_CLIPCHILDREN | WS_TABSTOP,
		CW_USEDEFAULT, CW_USEDEFAULT, 1600, 900,
		NULL,
		NULL,
		NULL,
		NULL
	);

	HWND hchild = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"Child",
		NULL,
		WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS,
		440, 200, 1125, 450,
		hwnd,
		NULL,
		NULL,
		NULL
	);

	HFONT defaultFont;
	defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessageW(hwnd, WM_SETFONT, WPARAM(defaultFont), (LPARAM)L"Times New Roman");

	if ((hwnd == NULL) || (hchild == NULL))
	{
		MessageBoxW(0, L"Create Window Failed", L"Error Message", 0);
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg = {};
	SecureZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}




















/*
bool checkMatching() {
	return true;
}
void testCases(int i) {
	switch (i) {
		case 0:
			stage.adjustAll(turn_off);
			stage.adjustRGB(20, 20, 20, 0, 0, 0);
			//stage.adjustRGB(40, 40, 40, 0, 0, 0);
			break;
		case 1:
			stage.adjustAll(turn_off);

			stage.adjustWhite(20, 20, 20, 0, 0, 0);
			//stage.adjustWhite(40, 40, 40, 0, 0, 0);
			break;
		case 2:
			stage.adjustAll(turn_off);
			stage.adjustRGB(10, 10, 10, 0, 0, 0);
			stage.adjustWhite(10, 10, 10, 0, 0, 0);
			//stage.adjustRGB(40, 40, 40, 0, 0, 0);
			break;
		case 3:
			stage.adjustAll(turn_off);
			stage.adjustRGB(60, 0, 0, 0, 0, 0);
			break;
		case 4:
			stage.adjustAll(turn_off);
			stage.adjustRGB(0, 60, 0, 0, 0, 0);
			break;
		case 5:
			stage.adjustAll(turn_off);
			stage.adjustRGB(0, 0, 60, 0, 0, 0);
			break;
		case 6:
			stage.adjustAll(turn_off);
			stage.adjustWhite(60, 0, 0, 0, 0, 0);
			break;
		case 7:
			stage.adjustAll(turn_off);
			stage.adjustWhite(0, 60, 0, 0, 0, 0);
			break;
		case 8:
			stage.adjustAll(turn_off);
			stage.adjustWhite(0, 0, 60, 0, 0, 0);
			break;
	}
	stage.adjustWhite(20 + i * 2, 0, 60 - i * 2, 0, 0, 0);
}

int RunSingleCamera16(int &count)
{
	cam.getCameraFromIndex();
	cam.connect();
	cam.getCameraInfo();
	cam.PrintCameraInfo16();
	cam.setNumImages(336);


	// Start capturing images
	cam.StartCapture();
	Image rawImage;
	std::vector<Image> vecImages;
	vecImages.resize(cam.getNumImages());
	int k = 0;
	stage.adjustAll(turn_off);
	Sleep(100); // wait for the lights to turn on
	for (int i = 0; i < 1;) {//cam.getNumImages()
		for (int j = 0; j < 1; j++) {
			if (j % 2 == 0) {
				stage(i, j)->setValue(255, 255, 255, 0, 0, 0);
			}
			else {
				stage(i, j)->setValue(255, 255, 255, 0, 0, 0);
			}
			stage.go();
			Sleep(100);
			
			cam.RetrieveBuffer(vecImages, &rawImage, k);
			k++;
			stage.adjustAll(turn_off);
		}
		//testCases(i);
		
		//cam.RetrieveBuffer(vecImages, &rawImage, i);
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
		cam.performFunc(vecImages[imageCnt].Convert(PIXEL_FORMAT_RGB, &convertedImage));
		cam.StatusQuery();
		printf("%dth picture, take %d\n", imageCnt + 1, count);

		// Create a unique filename
		ostringstream filename;
		filename << "..\\image_folder\\image_" << imageCnt + 1 << "_iter_" << count << ".png";

		// Save the image. If a file format is not passed in, then the file
		// extension is parsed to attempt to determine the file format.
		cam.performFunc(convertedImage.Save(filename.str().c_str()));
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
	initCurrentCamera(prop, cam, error);

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
*//*
int main(int argc, char** argv){

	// LIGHTS
	//GUI gui(argc, argv);
	WSADATA wsaData;
	// Initialize Winsock
	uint8_t sample[6] = { 8, 8, 8, 0, 0, 0 };
	//stage.adjustAll(sample);
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	stage(8, 5)->setValue(10, 10, 10, 10, 10, 10);
	stage.go(7);
	int count = 0;
	RunSingleCamera16(count);
		//stage[5][5]->setValue(turn_off);
		//Sleep(20);
	/*
	stage.adjustAll(turn_off);
	cam.performFunc(cam.getBusManager()->GetCameraFromIndex(0, cam.getGuid()));
	int count = 0;
	*/
	/*
	while (true) {
		RunSingleCamera16(count);
		ofstream configuration;
		string name("..\\image_folder\\take_num.config");
		configuration.open(name.c_str());
		configuration << count;
		configuration.close();
		//system("..\\x64\\Debug\\PNGReader.exe");
		if (checkMatching() == false) {
			count += 1;
		}
		else {
			break;
		}
	}
	*/
	/*
	while (true) {
		stage.rotation(1);
		Sleep(100);
	}
	*/
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
		stage(temp, temp2)->setValue(turn_on);
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
	
	cout << "Done! Press Enter to exit..." << endl;
	cin.ignore();
	return 0;

}
*/