#pragma once
#ifndef UNICODE
#define UNICODE
#endif // !UNICODE
#define _CRT_SECURE_NO_WARNINGS

#include "relighting.h"
#include <ShObjIdl.h>
#include <CommCtrl.h>
#include <stdlib.h>
#include <cwchar>
#include <cstring>
#include <string>
#include <conio.h>
#include "interface.h"
#include "convert.h"
#include <set>



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
#define ID_MN_1 20
#define ID_MN_2 21

#define ID_P_BN_1 22
#define ID_P_BN_2 23
#define ID_P_RB_1 24
#define ID_P_RB_2 25
#define ID_P_RB_3 26
#define ID_P_RB_4 27
#define ID_P_RB_5 28
#define ID_P_RB_6 29
#define ID_P_RB_7 30
#define ID_P_RB_8 31
#define ID_P_RB_9 32
#define ID_P_RB_10 33
#define ID_P_RB_11 34
#define ID_P_RB_12 35
#define ID_P_RB_13 36
#define ID_P_RB_14 37
#define ID_P_RB_15 38

#define ID_CB_1 39
#define ID_CB_2 40
#define ID_CB_3 41

#define ID_LB_1 42
#define ID_LB_2 43
#define ID_LB_3 44
#define ID_LB_4 45
#define ID_LB_5 46
#define ID_LB_6 47
#define ID_LB_7 48
#define ID_LB_8 49
#define ID_LB_9 50
#define ID_LB_10 51
#define ID_LB_11 52
#define ID_LB_12 53
#define ID_LB_13 54
#define ID_LB_14 55
#define ID_LB_15 56
#define ID_LB_16 57
#define ID_LB_17 58
#define ID_LB_18 59
#define ID_LB_19 60
#define ID_LB_20 61
#define ID_LB_21 62
#define ID_LB_22 63
#define ID_LB_23 64

#define ID_CHB_1 65
#define ID_CHB_2 66
#define ID_CHB_3 67
#define ID_CHB_4 68
#define ID_CHB_5 69
#define ID_CHB_6 70
#define ID_CHB_7 71
#define ID_CHB_8 72
#define ID_CHB_9 73
#define ID_CHB_10 74
#define ID_CHB_11 75
#define ID_CHB_12 76
#define ID_CHB_13 77
#define ID_CHB_14 78

#define turn_on 255,255,255,255,255,255
#define turn_half_lit 128,128,128,128,128,128
#define turn_off 0,0,0,0,0,0

extern LightStage stage;
extern LightStageCamera cam;
extern HWND hmaster;
extern HWND hchild;
extern HWND hpopup;

extern LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK WindowProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK WindowProc3(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK WindowProc4(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern void addMenu(HWND);
extern HWND WINAPI CreateTrackbar(HWND hwnd, UINT iMin, UINT iMax, UINT iSelMin, UINT iSelMax, UINT posx, UINT posy, UINT width = 250, UINT height = 50);
extern HWND WINAPI addRadioButton(HWND hwnd, int x, int y, int width, int height, int handle, wchar_t * tag, bool isGrouped);
extern HWND WINAPI addLabel(HWND hwnd, int x, int y, int width, int height, int handle, wchar_t * tag);
extern HWND WINAPI addTextBox(HWND hwnd, int xpos, int ypos, int width, int height, int handle, bool isGrouped);
extern HWND WINAPI addCheckBox(HWND hwnd, int x, int y, int width, int height, int handle, wchar_t * tag, bool isGrouped);
extern HWND WINAPI CreateComboEx(HWND hwnd, int x, int y, int width, int height, int handle);
extern LRESULT CALLBACK subEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
extern HWND WINAPI CreateButton(HWND hwnd, int x, int y, int width, int height, int handle, wchar_t * label);
extern LRESULT CALLBACK subCVProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
extern void fileOpen();
extern void fileSave();
extern void updateLight();
extern void demoRoutine();
extern void captureRoutine();
extern void precomRoutine();
extern void tableRoutine();
extern void loadVal(uint8_t * config);
extern void loadValMap();
extern void loadVoronMap();
extern void lightStageInit();
extern void scale(uint8_t * config);