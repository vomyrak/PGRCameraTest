#include "GUI.h"

extern HWND * Texts;
extern HWND * LightCtrl;
extern wchar_t buf[4];
extern int pos;
extern int activeButton;
extern HWND * display;
extern UINT8 currentConfig[6];
extern HWND * Label;
extern HWND hwnd;
extern HWND hchild;
extern HWND * Button;
extern LightStage stage;
extern LightStageCamera cam;

int selectedPowerSupply = 0;
int selectedLightPos = 0;
bool whiteSelected = FALSE;
POINT cursor = { 535, 5 };
HBRUSH brush;
HPEN pen;
RECT * lightMatrix = new RECT[336];
PAINTSTRUCT ps;
using namespace std;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		addMenu(hwnd);
		Texts[0] = addTextBox(hwnd, 775, 703, 30, 20, ID_EC_1);
		Texts[1] = addTextBox(hwnd, 1160, 703, 30, 20, ID_EC_2);
		Texts[2] = addTextBox(hwnd, 1535, 703, 30, 20, ID_EC_3);
		Texts[3] = addTextBox(hwnd, 775, 763, 30, 20, ID_EC_4);
		Texts[4] = addTextBox(hwnd, 1160, 763, 30, 20, ID_EC_5);
		Texts[5] = addTextBox(hwnd, 1535, 763, 30, 20, ID_EC_6);
		Label[0] = addLabel(hwnd, 455, 704, 60, 20, L"Ch1_H");
		Label[1] = addLabel(hwnd, 455, 764, 60, 20, L"Ch1_L");
		Label[2] = addLabel(hwnd, 850, 704, 60, 20, L"Ch2_H");
		Label[3] = addLabel(hwnd, 850, 764, 60, 20, L"Ch2_L");
		Label[4] = addLabel(hwnd, 1225, 704, 60, 20, L"Ch3_H");
		Label[5] = addLabel(hwnd, 1225, 764, 60, 20, L"Ch3_L");
		addLabel(hwnd, 965, 660, 60, 20, L"Arc 0");
		addLabel(hwnd, 873, 660, 60, 20, L"Arc 1");
		addLabel(hwnd, 781, 660, 60, 20, L"Arc 2");
		addLabel(hwnd, 689, 660, 60, 20, L"Arc 3");
		addLabel(hwnd, 597, 660, 60, 20, L"Arc 4");
		addLabel(hwnd, 505, 660, 60, 20, L"Arc 5");
		addLabel(hwnd, 1517, 660, 60, 20, L"Arc 6");
		addLabel(hwnd, 1425, 660, 60, 20, L"Arc 7");
		addLabel(hwnd, 1333, 660, 60, 20, L"Arc 8");
		addLabel(hwnd, 1241, 660, 60, 20, L"Arc 9");
		addLabel(hwnd, 1149, 660, 60, 20, L"Arc 10");
		addLabel(hwnd, 1057, 660, 60, 20, L"Arc 11");
		LightCtrl[0] = CreateTrackbar(hwnd, 0, 255, ID_TB_1, 8, 515, 700);
		LightCtrl[1] = CreateTrackbar(hwnd, 0, 255, ID_TB_2, 8, 900, 700);
		LightCtrl[2] = CreateTrackbar(hwnd, 0, 255, ID_TB_3, 8, 1275, 700);
		LightCtrl[3] = CreateTrackbar(hwnd, 0, 255, ID_TB_4, 8, 515, 760);
		LightCtrl[4] = CreateTrackbar(hwnd, 0, 255, ID_TB_5, 8, 900, 760);
		LightCtrl[5] = CreateTrackbar(hwnd, 0, 255, ID_TB_6, 8, 1275, 760);
		Button[0] = CreateButton(hwnd, 440, 155, 100, 25, ID_BN_1, L"Load Default");
		Button[1] = CreateButton(hwnd, 550, 155, 150, 25, ID_BN_2, L"Load Global Default");
		Button[2] = CreateButton(hwnd, 710, 155, 50, 25, ID_BN_3, L"Reset");
		Button[3] = CreateButton(hwnd, 770, 155, 70, 25, ID_BN_4, L"Reset All");
		Button[4] = CreateButton(hwnd, 850, 155, 120, 25, ID_BN_5, L"Save As Default");
		Button[5] = CreateButton(hwnd, 980, 155, 120, 25, ID_BN_6, L"Save Value Map");
		Button[6] = CreateButton(hwnd, 1110, 155, 120, 25, ID_BN_7, L"Load Value Map");
		CreateComboEx(hwnd);
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
		BeginPaint(hchild, &ps);
		brush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[1], currentConfig[2]));
		FillRect(GetDC(hchild), lightMatrix + activeButton, brush);
		EndPaint(hchild, &ps);
		ReleaseDC(hchild, GetDC(hchild));
		DeleteObject(brush);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			if (LOWORD(wParam) == ID_BN_1) {
				uint8_t * temp = stage.getDefault();
				for (auto i = 0; i < 6; i++) {
					currentConfig[i] = temp[i];
					SendMessageW(LightCtrl[i], TBM_SETPOS, true, (LPARAM)currentConfig[i]);
					pos = currentConfig[i];
					wsprintfW(buf, L"%d", pos);
					SendMessageW(Texts[i], WM_SETTEXT, NULL, (LPARAM)buf);
				}
				updateLight();
				BeginPaint(hchild, &ps);
				brush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[1], currentConfig[2]));
				FillRect(GetDC(hchild), lightMatrix + activeButton, brush);
				EndPaint(hchild, &ps);
				ReleaseDC(hchild, GetDC(hchild));
				DeleteObject(brush);
			}
			else if (LOWORD(wParam) == ID_BN_2) {
				uint8_t * temp = stage.getDefault();
				for (auto i = 0; i < 6; i++) {
					currentConfig[i] = temp[i];
					SendMessageW(LightCtrl[i], TBM_SETPOS, true, (LPARAM)currentConfig[i]);
					pos = currentConfig[i];
					wsprintfW(buf, L"%d", pos);
					SendMessageW(Texts[i], WM_SETTEXT, NULL, (LPARAM)buf);
				}
				stage.adjustAll(temp);
				BeginPaint(hchild, &ps);
				brush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[1], currentConfig[2]));
				for (int i = 0; i < 336; i++) {
					FillRect(GetDC(hchild), lightMatrix + i, brush);
				}
				EndPaint(hchild, &ps);
				ReleaseDC(hchild, GetDC(hchild));
				DeleteObject(brush);
			}
			else if (LOWORD(wParam) == ID_BN_3) {
				for (auto i = 0; i < 6; i++) {
					currentConfig[i] = 0;
					SendMessageW(LightCtrl[i], TBM_SETPOS, true, (LPARAM)currentConfig[i]);
					pos = currentConfig[i];
					wsprintfW(buf, L"%d", pos);
					SendMessageW(Texts[i], WM_SETTEXT, NULL, (LPARAM)buf);
				}
				updateLight();
				BeginPaint(hchild, &ps);
				brush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[1], currentConfig[2]));
				FillRect(GetDC(hchild), lightMatrix + activeButton, brush);
				EndPaint(hchild, &ps);
				ReleaseDC(hchild, GetDC(hchild));
				DeleteObject(brush);
			}
			else if (LOWORD(wParam) == ID_BN_4) {
				for (auto i = 0; i < 6; i++) {
					currentConfig[i] = 0;
					SendMessageW(LightCtrl[i], TBM_SETPOS, true, (LPARAM)currentConfig[i]);
					pos = currentConfig[i];
					wsprintfW(buf, L"%d", pos);
					SendMessageW(Texts[i], WM_SETTEXT, NULL, (LPARAM)buf);
				}
				stage.adjustAll(turn_off);
				BeginPaint(hchild, &ps);
				brush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[1], currentConfig[2]));
				for (int i = 0; i < 336; i++) {
					FillRect(GetDC(hchild), lightMatrix + i, brush);
				}
				EndPaint(hchild, &ps);
				ReleaseDC(hchild, GetDC(hchild));
				DeleteObject(brush);
			}
			else if (LOWORD(wParam) == ID_BN_5) {
				stage.setDefault(currentConfig);
			}
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
	switch (uMsg)
	{
	case WM_COMMAND:
		break;
	case WM_CREATE:
		break;
	case WM_LBUTTONDOWN:
		cursor.x = LOWORD(lParam);
		cursor.y = HIWORD(lParam);
		for (auto j = 0; j < 336; j++) {
			if (PtInRect(lightMatrix + j, cursor)) {
				activeButton = j;
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
				BeginPaint(hwnd, &ps);
				brush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[1], currentConfig[2]));
				FillRect(GetDC(hwnd), lightMatrix + activeButton, brush);
				EndPaint(hwnd, &ps);
				ReleaseDC(hwnd, GetDC(hwnd));
				DeleteObject(brush);
				break;
			}
		}
		break;
	case WM_PAINT:
		pen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, pen);
		for (auto i = 0; i < 24; i++) {
			if (i % 4 == 0) {
				for (auto j = 0; j < 7; j++) {
					if (j == 0) {
						Rectangle(hdc, 27 + 46 * ((35 - i) % 24), 3, 39 + 46 * ((35 - i) % 24), 15);
						Rectangle(hdc, 27 + 46 * ((35 - i) % 24), 14, 39 + 46 * ((35 - i) % 24), 26);
						SetRect(lightMatrix + i * 14 + 2 * j, 28 + 46 * ((35 - i) % 24), 4, 38 + 46 * ((35 - i) % 24), 14);
						FillRect(hdc, lightMatrix + i * 14 + 2 * j, (HBRUSH)GetStockObject(BLACK_BRUSH));
						SetRect(lightMatrix + i * 14 + 2 * j + 1, 28 + 46 * ((35 - i) % 24), 15, 38 + 46 * ((35 - i) % 24), 25);
						FillRect(hdc, lightMatrix + i * 14 + 2 * j + 1, (HBRUSH)GetStockObject(BLACK_BRUSH));
					}
					else {
						Rectangle(hdc, 27 + 46 * ((35 - i) % 24), 33 + j * 60, 39 + 46 * ((35 - i) % 24), 45 + j * 60);
						Rectangle(hdc, 27 + 46 * ((35 - i) % 24), 44 + j * 60, 39 + 46 * ((35 - i) % 24), 56 + j * 60);
						SetRect(lightMatrix + i * 14 + 2 * j, 28 + 46 * ((35 - i) % 24), 34 + j * 60, 38 + 46 * ((35 - i) % 24), 44 + j * 60);
						FillRect(hdc, lightMatrix + i * 14 + 2 * j, (HBRUSH)GetStockObject(BLACK_BRUSH));
						SetRect(lightMatrix + i * 14 + 2 * j + 1, 28 + 46 * ((35 - i) % 24), 45 + j * 60, 38 + 46 * ((35 - i) % 24), 55 + j * 60);
						FillRect(hdc, lightMatrix + i * 14 + 2 * j + 1, (HBRUSH)GetStockObject(BLACK_BRUSH));
					}
				}
			}
			else if (i % 2 == 0) {
				for (auto j = 0; j < 7; j++) {
					Rectangle(hdc, 27 + 46 * ((35 - i) % 24), 33 + j * 60, 39 + 46 * ((35 - i) % 24), 45 + j * 60);
					Rectangle(hdc, 27 + 46 * ((35 - i) % 24), j * 60 + 44, 39 + 46 * ((35 - i) % 24), j * 60 + 56);
					SetRect(lightMatrix + i * 14 + 2 * j, 28 + 46 * ((35 - i) % 24), 34 + j * 60, 38 + 46 * ((35 - i) % 24), 44 + j * 60);
					FillRect(hdc, lightMatrix + i * 14 + 2 * j, (HBRUSH)GetStockObject(BLACK_BRUSH));
					SetRect(lightMatrix + i * 14 + 2 * j + 1, 28 + 46 * ((35 - i) % 24), j * 60 + 45, 38 + 46 * ((35 - i) % 24), j * 60 + 55);
					FillRect(hdc, lightMatrix + i * 14 + 2 * j + 1, (HBRUSH)GetStockObject(BLACK_BRUSH));
				}
			}
			else {
				for (auto j = 0; j < 7; j++) {
					Rectangle(hdc, 32 + 46 * ((35 - i) % 24), 69 + j * 60, 44 + 46 * ((35 - i) % 24), 81 + j * 60);
					Rectangle(hdc, 21 + 46 * ((35 - i) % 24), j * 60 + 69, 33 + 46 * ((35 - i) % 24), j * 60 + 81);
					SetRect(lightMatrix + i * 14 + 2 * j, 33 + 46 * ((35 - i) % 24), 70 + j * 60, 43 + 46 * ((35 - i) % 24), 80 + j * 60);
					FillRect(hdc, lightMatrix + i * 14 + 2 * j, (HBRUSH)GetStockObject(BLACK_BRUSH));
					SetRect(lightMatrix + i * 14 + 2 * j + 1, 22 + 46 * ((35 - i) % 24), j * 60 + 70, 32 + 46 * ((35 - i) % 24), j * 60 + 80);
					FillRect(hdc, lightMatrix + i * 14 + 2 * j + 1, (HBRUSH)GetStockObject(BLACK_BRUSH));

				}
			}
		}
		EndPaint(hwnd, &ps);
		DeleteObject(pen);
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

HWND WINAPI CreateButton(HWND hwnd, int x, int y, int width, int height, int handle, wchar_t * label)
{
	HWND button = CreateWindowW(L"BUTTON", label, WS_TABSTOP | WS_VISIBLE |BS_FLAT| WS_CHILD | BS_DEFPUSHBUTTON, x, y, width, height, hwnd, (HMENU)handle, NULL, NULL);
	return button;
}

HWND WINAPI CreateTrackbar(HWND hwnd, UINT iMin, UINT iMax, UINT iSelMin, UINT iSelMax, UINT posx, UINT posy, UINT width, UINT height)
{
	HWND hwndTrack = CreateWindowEx(0, TRACKBAR_CLASS, L"Trackbar Control", WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_ENABLESELRANGE | TBS_HORZ, posx, posy, width, height, hwnd, (HMENU)iSelMin, NULL, NULL);
	SendMessage(hwndTrack, TBM_SETRANGE,	(WPARAM)TRUE, (LPARAM)MAKELONG(iMin, iMax));  
	SendMessage(hwndTrack, TBM_SETPAGESIZE,0, (LPARAM)4);                  
	return hwndTrack;
}

HWND WINAPI addRadioButton(HWND hwnd)
{
	HWND button1 = CreateWindowExW(NULL, L"BUTTON", L"Radiobox", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_GROUP, 440, 160, 10, 10, hwnd, NULL, NULL, NULL);
	HWND button2 = CreateWindowExW(NULL, L"BUTTON", L"Radiobox", BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_GROUP, 640, 160, 10, 10, hwnd, NULL, NULL, NULL);
	CheckRadioButton(hwnd, (int)button1, (int)button2, (int)button1);
	return button1;
}

HWND WINAPI addLabel(HWND hwnd, int x, int y, int width, int height, wchar_t* tag)
{
	HWND label = CreateWindowExW(NULL, L"STATIC", tag, WS_CHILDWINDOW | WS_VISIBLE, x, y, width, height, hwnd, NULL, NULL, NULL);
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
	HWND temp = CreateWindowEx(0, WC_COMBOBOXEX, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | CBS_DROPDOWN, 1265, 150, 300, 100, hwnd, NULL, NULL, NULL);
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
