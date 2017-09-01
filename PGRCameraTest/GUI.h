#pragma once
#ifndef UNICODE
#define UNICODE
#endif // !UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <CommCtrl.h>
#include <stdlib.h>
#include <cwchar>
#include <cstring>
#include <string>
#include "LightStage.h"
#include "LightStageCamera.h"
#define ID_TB_1 1
#define ID_TB_2 2
#define ID_TB_3 3
#define ID_TB_4 4
#define ID_TB_5 5
#define ID_TB_6 6
#define ID_EC_1 7
#define ID_EC_2 8
#define ID_EC_3 9
#define ID_EC_4 10
#define ID_EC_5 11
#define ID_EC_6 12
#define ID_BN_1 13
#define ID_BN_2 14
#define ID_BN_3 15
#define ID_BN_4 16
#define ID_BN_5 17
#define ID_BN_6 18
#define ID_BN_7 19

#define turn_on 255,255,255,255,255,255
#define turn_half_lit 128,128,128,128,128,128
#define turn_off 0,0,0,0,0,0

extern LightStage stage;
extern LightStageCamera cam;
extern LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK WindowProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern HMENU WINAPI addMenu(HWND);
extern HWND WINAPI CreateTrackbar(HWND hwnd, UINT iMin, UINT iMax, UINT iSelMin, UINT iSelMax, UINT posx, UINT posy, UINT width = 250, UINT height = 50);
extern HWND WINAPI addRadioButton(HWND);
extern HWND WINAPI addLabel(HWND hwnd, int x, int y, int width, int height, wchar_t * tag);
extern HWND WINAPI addTextBox(HWND hwnd, int xpos, int ypos, int width, int height, int handle);
extern HWND WINAPI CreateComboEx(HWND hwnd);
extern LRESULT CALLBACK subEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
extern HWND WINAPI CreateButton(HWND hwnd, int x, int y, int width, int height, int handle, wchar_t * label);
extern HWND * Texts;
extern HWND * LightCtrl;
extern wchar_t buf[4];
extern int pos;
extern int activeButton;
extern HWND * display;
extern UINT8 currentConfig[6];
extern HWND * Label;
extern void updateLight();
extern HWND hwnd;
extern HWND hchild;
extern HWND * Button;