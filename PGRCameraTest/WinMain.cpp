#include "stdafx.h"

#include <fstream>
#include <cmath>
#include <tuple>
#include <vector>
#include "GUI.h"
#include "FlyCapture2.h"


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace FlyCapture2;
using namespace std;

LightStage stage;
LightStageCamera cam;
HWND hmaster;
HWND hchild;
HWND hpopup;
HWND hcamera;

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	lightStageInit();
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

	WNDCLASSW wc3 = {};
	wc3.cbClsExtra = 0;
	wc3.cbWndExtra = 0;
	wc3.hIcon = 0;
	wc3.lpfnWndProc = WindowProc3;
	wc3.hInstance = hinstance;
	wc3.style = CS_HREDRAW | CS_VREDRAW;
	wc3.lpszMenuName = 0;
	wc3.lpszClassName = L"Popup";
	wc3.hCursor = LoadCursor(0, IDC_ARROW);
	wc3.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc3.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	RegisterClassW(&wc3);

	WNDCLASSW wc4 = {};
	wc4.cbClsExtra = 0;
	wc4.cbWndExtra = 0;
	wc4.hIcon = 0;
	wc4.lpfnWndProc = WindowProc4;
	wc4.hInstance = hinstance;
	wc4.style = CS_HREDRAW | CS_VREDRAW;
	wc4.lpszMenuName = 0;
	wc4.lpszClassName = L"Camera";
	wc4.hCursor = LoadCursor(0, IDC_ARROW);
	wc4.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc4.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	RegisterClassW(&wc4);

	hmaster = CreateWindowExW(
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

	hchild = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"Child",
		NULL,
		WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS,
		25, 70, 1125, 450,
		hmaster,
		NULL,
		NULL,
		NULL
	);
	
	hpopup = CreateWindowExW(
		NULL,
		L"Popup",
		L"Mode selection",
		WS_OVERLAPPEDWINDOW  ^ WS_THICKFRAME  | WS_POPUPWINDOW,
		600, 150, 460, 300,
		NULL,
		NULL,
		NULL,
		NULL
	);

	hcamera = CreateWindowExW(
		NULL,
		L"Camera",
		L"",
		WS_CHILDWINDOW | WS_VISIBLE,
		1165, 25, 400, 650,
		hmaster,
		NULL,
		NULL,
		NULL
	);
	
	HFONT defaultFont;
	defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessageW(hmaster, WM_SETFONT, WPARAM(defaultFont), (LPARAM)L"Times New Roman");

	if ((hmaster == NULL) || (hchild == NULL) || (hpopup == NULL))
	{
		MessageBoxW(0, L"Create Window Failed", L"Error Message", 0);
	}
	ShowWindow(hmaster, nCmdShow);
	UpdateWindow(hmaster);

	MSG msg = {};
	SecureZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		//if (!(IsDialogMessageW(hmaster, &msg))) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		//}

	}
	return (int)msg.wParam;
}

void lightStageInit() {
	ifstream infile;
	infile.open("default.val");
	if (!infile.is_open()) {
		stage.adjustAll(stage.getDefault());
	}
	else {
		int r, r2, g, g2, b, b2;
		infile >> r >> r2 >> g >> g2 >> b >> b2;
		stage.adjustAll(r, r2, g, g2, b, b2);
	}
	infile.close();
}