#include "stdafx.h"
#include <winsock2.h>

#include <fstream>
#include <cmath>
#include <tuple>
#include <vector>
#include <ws2tcpip.h>
#include "FlyCapture2.h"
#include "GUI.h"





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
UINT8 currentConfig[6] = { 0, 0, 0, 0, 0, 0 };
HWND * Label = new HWND[6];
HWND hwnd;
HWND hchild;
HWND * Button = new HWND[7];

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

	hwnd = CreateWindowExW(
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