#include "GUI.h"

extern HWND * Texts;
extern HWND * LightCtrl;
extern wchar_t buf[4];
extern int pos;
extern int activeButton;
extern HWND * display;
extern UINT8 currentConfig[6];
extern HWND * Label;

int selectedPowerSupply = 0;
int selectedLightPos = 0;
bool whiteSelected = FALSE;

using namespace std;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg)
	{
	case WM_CREATE:
		addMenu(hwnd);
		LightCtrl[0] = CreateTrackbar(hwnd, 0, 255, ID_TB_1, 8, 515, 700);
		LightCtrl[3] = CreateTrackbar(hwnd, 0, 255, ID_TB_4, 8, 515, 760);
		LightCtrl[1] = CreateTrackbar(hwnd, 0, 255, ID_TB_2, 8, 900, 700);
		LightCtrl[4] = CreateTrackbar(hwnd, 0, 255, ID_TB_5, 8, 900, 760);
		LightCtrl[2] = CreateTrackbar(hwnd, 0, 255, ID_TB_3, 8, 1275, 700);
		LightCtrl[5] = CreateTrackbar(hwnd, 0, 255, ID_TB_6, 8, 1275, 760);
		CreateComboEx(hwnd);
		addRadioButton(hwnd);
		Texts[0] = addTextBox(hwnd, 775, 703, 30, 20, ID_EC_1);
		Texts[3] = addTextBox(hwnd, 775, 763, 30, 20, ID_EC_4);
		Texts[1] = addTextBox(hwnd, 1160, 703, 30, 20, ID_EC_2);
		Texts[4] = addTextBox(hwnd, 1160, 763, 30, 20, ID_EC_5);
		Texts[2] = addTextBox(hwnd, 1535, 703, 30, 20, ID_EC_3);
		Texts[5] = addTextBox(hwnd, 1535, 763, 30, 20, ID_EC_6);
		Label[0] = addLabel(hwnd, 455, 704, 60, 20);
		Label[1] = addLabel(hwnd, 455, 764, 60, 20);
		Label[2] = addLabel(hwnd, 850, 704, 60, 20);
		Label[3] = addLabel(hwnd, 850, 764, 60, 20);
		Label[4] = addLabel(hwnd, 1225, 704, 60, 20);
		Label[5] = addLabel(hwnd, 1225, 764, 60, 20);
		UpdateWindow(hwnd);
		break;
	case WM_HSCROLL:
		for (int i = 0; i < 6; i++) {
			pos = SendMessageW(LightCtrl[i], TBM_GETPOS, 0, 0);
			currentConfig[i] = pos;
			wsprintfW(buf, L"%d", pos);
			SendMessageW(Texts[i], WM_SETTEXT, NULL, (LPARAM)buf);
			UpdateWindow(Texts[i]);
			UpdateWindow(LightCtrl[i]);
		}
		updateLight();
		break;
	case WM_COMMAND:
		switch (wParam) {
		case 1:
			MessageBeep(MB_OK);
			break;
		}
		break;
	case WM_ACTIVATE:
		UpdateWindow(hwnd);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

LRESULT CALLBACK WindowProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (uMsg)
	{
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			activeButton = LOWORD(wParam);
			selectedPowerSupply = activeButton / 28;
			selectedLightPos = activeButton % 28;
			whiteSelected = activeButton % 2;
			stage[selectedPowerSupply][selectedLightPos]->getConfig(currentConfig);
			for (int i = 0; i < 6; i++) {
				pos = currentConfig[i];
				wsprintfW(buf, L"%d", pos);
				SendMessageW(Texts[i], WM_SETTEXT, NULL, (LPARAM)buf);
				SendMessageW(LightCtrl[i], TBM_SETPOS, true, (LPARAM)pos);
				UpdateWindow(Texts[i]);
				UpdateWindow(LightCtrl[i]);
			}
		}
		break;
	case WM_CREATE:
		for (auto i = 0; i < 24; i++)
		{
			if (i % 4 == 0) {
				for (auto j = 0; j < 7; j++)
				{
					if (j == 0) {
						display[i * 14 + 2 * j] = CreateWindowW(L"BUTTON", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_DEFPUSHBUTTON, 28 + 46 * ((35 - i) % 24), 5, 10, 10, hwnd, (HMENU)(i * 14 + 2 * j), NULL, NULL);
						display[i * 14 + 2 * j + 1] = CreateWindowW(L"BUTTON", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_DEFPUSHBUTTON, 28 + 46 * ((35 - i) % 24), 15, 10, 10, hwnd, (HMENU)(i * 14 + 2 * j + 1), NULL, NULL);
					}
					else {
						display[i * 14 + 2 * j] = CreateWindowW(L"BUTTON", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_DEFPUSHBUTTON, 28 + 46 * ((35 - i) % 24), 35 + j * 60, 10, 10, hwnd, (HMENU)(i * 14 + 2 * j), NULL, NULL);
						display[i * 14 + 2 * j + 1] = CreateWindowW(L"BUTTON", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_DEFPUSHBUTTON, 28 + 46 * ((35 - i) % 24), j * 60 + 45, 10, 10, hwnd, (HMENU)(i * 14 + 2 * j + 1), NULL, NULL);
					}
				}
			}
			else if (i % 2 == 0)
			{
				for (auto j = 0; j < 7; j++) {
					display[i * 14 + 2 * j] = CreateWindowW(L"BUTTON", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_DEFPUSHBUTTON, 28 + 46 * ((35 - i) % 24), 35 + j * 60, 10, 10, hwnd, (HMENU)(i * 14 + 2 * j), NULL, NULL);
					display[i * 14 + 2 * j + 1] = CreateWindowW(L"BUTTON", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_DEFPUSHBUTTON, 28 + 46 * ((35 - i) % 24), j * 60 + 45, 10, 10, hwnd, (HMENU)(i * 14 + 2 * j + 1), NULL, NULL);
				}
			}
			else
			{
				for (auto j = 0; j < 7; j++) {
					display[i * 14 + 2 * j] = CreateWindowW(L"BUTTON", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_DEFPUSHBUTTON, 23 + 46 * ((35 - i) % 24), 70 + j * 60, 10, 10, hwnd, (HMENU)(i * 14 + 2 * j), NULL, NULL);
					display[i * 14 + 2 * j + 1] = CreateWindowW(L"BUTTON", NULL, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_DEFPUSHBUTTON, 33 + 46 * ((35 - i) % 24), j * 60 + 70, 10, 10, hwnd, (HMENU)(i * 14 + 2 * j + 1), NULL, NULL);
				}
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

HMENU WINAPI addMenu(HWND hwnd)
{
	HMENU hmenu = CreateMenu();
	HMENU submenu = CreateMenu();
	HMENU hmenu2 = CreatePopupMenu();
	AppendMenuW(hmenu, MF_POPUP, (UINT_PTR)submenu, L"File");
	AppendMenuW(submenu, MF_STRING, NULL, L"File2");
	SetMenu(hwnd, hmenu);
	AppendMenuW(hmenu, MF_POPUP, NULL, L"File3");
	return hmenu;
}

HWND WINAPI CreateTrackbar(HWND hwnd, UINT iMin, UINT iMax, UINT iSelMin, UINT iSelMax, UINT posx, UINT posy, UINT width, UINT height)
{
	HWND hwndTrack = CreateWindowEx(0, TRACKBAR_CLASS,L"Trackbar Control",WS_CHILD |	WS_VISIBLE |	TBS_NOTICKS |TBS_ENABLESELRANGE |TBS_HORZ, posx, posy, 	width, height,	hwnd,(HMENU)iSelMin,NULL, NULL);
	SendMessage(hwndTrack, TBM_SETRANGE,	(WPARAM)TRUE, (LPARAM)MAKELONG(iMin, iMax));  
	SendMessage(hwndTrack, TBM_SETPAGESIZE,0, (LPARAM)4);                  
	SetFocus(hwndTrack);
	return hwndTrack;
}

HWND WINAPI addRadioButton(HWND hwnd)
{
	HWND button1 = CreateWindowExW(NULL, L"BUTTON", L"Radiobox", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_GROUP, 440, 160, 10, 10, hwnd, NULL, NULL, NULL);
	HWND button2 = CreateWindowExW(NULL, L"BUTTON", L"Radiobox", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_GROUP, 640, 160, 10, 10, hwnd, NULL, NULL, NULL);
	CheckRadioButton(hwnd, (int)button1, (int)button2, (int)button1);
	return button1;
}

HWND WINAPI addLabel(HWND hwnd, int x, int y, int width, int height)
{
	HWND label = CreateWindowExW(NULL, L"STATIC", L"Ch1_H", WS_CHILDWINDOW | WS_VISIBLE, x, y, width, height, hwnd, NULL, NULL, NULL);
	return label;
}

HWND WINAPI addTextBox(HWND hwnd, int xpos, int ypos, int width, int height, int handle)
{
	HWND temp = CreateWindowExW(NULL, L"EDIT", NULL, ES_NUMBER | WS_VISIBLE | WS_CHILDWINDOW | WS_BORDER, xpos, ypos, width, height, hwnd, (HMENU)handle, NULL, NULL);

	SendMessageW(temp, WM_SETTEXT, NULL, (LPARAM)L"0");
	SetWindowSubclass(temp, subEditProc, 0, 0);
	return temp;
}

HWND WINAPI CreateComboEx(HWND hwnd)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_USEREX_CLASSES;
	InitCommonControlsEx(&icex);
	HWND temp = CreateWindowEx(0, WC_COMBOBOXEX, NULL,WS_BORDER | WS_VISIBLE |WS_CHILD | CBS_DROPDOWN,1265,150,300,100,hwnd,NULL,NULL,NULL);
	return temp;
}

LRESULT CALLBACK subEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RETURN:
			for (auto i = 0; i < 6; i++) {
				SendMessageW(Texts[i], WM_GETTEXT, 4, (LPARAM)buf);
				pos = stoi(buf);
				currentConfig[i] = pos;
				SendMessageW(LightCtrl[i], TBM_SETPOS, true, (LPARAM)pos);
				updateLight();
			}
			break;
		}
	case WM_PAINT:
		break;
	default:
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}
	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void updateLight()
{
	stage[selectedPowerSupply][selectedLightPos]->setConfig(currentConfig);
	if (!whiteSelected) {
		stage.go(selectedPowerSupply * 2);
	}
	else {
		stage.go(selectedPowerSupply * 2 + 1);
	}
	Sleep(20);
}
