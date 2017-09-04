#include "GUI.h"
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
using namespace std;
using namespace cv;

HWND * Texts = new HWND[6];
HWND * LightCtrl = new HWND[6];
wchar_t buf[4];
int pos = 0;
int activeButton = 0;
HWND * display = new HWND[336];
UINT8 currentConfig[6] = { 0, 0, 0, 0, 0, 0 };
HWND * Label = new HWND[9];
HWND * Button = new HWND[7];
HMENU * Menu = new HMENU[3];
HWND * RadioButton = new HWND[4];
HWND ComboBox;
int selectedPowerSupply = 0;
int selectedLightPos = 0;
bool whiteSelected = FALSE;
POINT cursor = { 535, 5 };
HBRUSH rgbbrush;
HBRUSH wbrush;
HPEN pen;
HDC globalHDC;
RECT * lightMatrix = new RECT[336];
PAINTSTRUCT ps;
int grayval;
int selectedCBItem = 0;
int rotationIndex = 0;
wchar_t * environment = new wchar_t[260];
bool rotationInterrupted = 0;
bool multipleActive = false;
vector<int> activeList(1, 0);



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		addMenu(hwnd);
		Texts[0] = addTextBox(hwnd, 775, 703, 30, 20, ID_EC_1);
		Texts[1] = addTextBox(hwnd, 775, 763, 30, 20, ID_EC_2);
		Texts[2] = addTextBox(hwnd, 1160, 703, 30, 20, ID_EC_3);
		Texts[3] = addTextBox(hwnd, 1160, 763, 30, 20, ID_EC_4);
		Texts[4] = addTextBox(hwnd, 1535, 703, 30, 20, ID_EC_5);
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
		LightCtrl[1] = CreateTrackbar(hwnd, 0, 255, ID_TB_2, 8, 515, 760);
		LightCtrl[2] = CreateTrackbar(hwnd, 0, 255, ID_TB_3, 8, 900, 700);
		LightCtrl[3] = CreateTrackbar(hwnd, 0, 255, ID_TB_4, 8, 900, 760);
		LightCtrl[4] = CreateTrackbar(hwnd, 0, 255, ID_TB_5, 8, 1275, 700);
		LightCtrl[5] = CreateTrackbar(hwnd, 0, 255, ID_TB_6, 8, 1275, 760);
		Button[0] = CreateButton(hwnd, 440, 155, 100, 25, ID_BN_1, L"Load Default");
		Button[1] = CreateButton(hwnd, 550, 155, 150, 25, ID_BN_2, L"Load Global Default");
		Button[2] = CreateButton(hwnd, 710, 155, 50, 25, ID_BN_3, L"Reset");
		Button[3] = CreateButton(hwnd, 770, 155, 70, 25, ID_BN_4, L"Reset All");
		Button[4] = CreateButton(hwnd, 850, 155, 120, 25, ID_BN_5, L"Save As Default");
		//Button[5] = CreateButton(hwnd, 980, 155, 120, 25, ID_BN_6, L"Save Value Map");
		//Button[6] = CreateButton(hwnd, 1110, 155, 120, 25, ID_BN_7, L"Load Value Map");
		ComboBox = CreateComboEx(hwnd);
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"No Map Loaded");
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"Ennis");
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"Glacier");
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"Grace");
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"Pisa");
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"Uffizi");
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"Rotated Pisa");
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"Z-gradient");
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"Y-gradient");
		SendMessageW(ComboBox, CB_ADDSTRING, NULL, (LPARAM)L"X-gradient");
		SendMessageW(ComboBox, CB_SETCURSEL, 0, NULL);
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
		globalHDC = GetDC(hchild);
		BeginPaint(hchild, &ps);
		if (activeButton % 2) {
			grayval = (currentConfig[0] + currentConfig[2] + currentConfig[4]) / 3;
			wbrush = CreateSolidBrush(RGB(grayval, grayval, grayval));
			FillRect(globalHDC, lightMatrix + activeButton, wbrush);
			DeleteObject(wbrush);
		}
		else {
			rgbbrush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[2], currentConfig[4]));
			FillRect(globalHDC, lightMatrix + activeButton, rgbbrush);
			DeleteObject(rgbbrush);
		}

		ReleaseDC(hchild, globalHDC);
		EndPaint(hchild, &ps);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == CBN_SELCHANGE) {
			selectedCBItem = SendMessageW(ComboBox, CB_GETCURSEL, NULL, NULL);
			ShowWindow(hpopup, SW_SHOWNORMAL);
			UpdateWindow(hpopup);
		}
		else if (HIWORD(wParam) == BN_CLICKED) {
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
				globalHDC = GetDC(hchild);
				BeginPaint(hchild, &ps);
				if (activeButton % 2) {
					grayval = (currentConfig[0] + currentConfig[2] + currentConfig[4]) / 3;
					wbrush = CreateSolidBrush(RGB(grayval, grayval, grayval));
					FillRect(globalHDC, lightMatrix + activeButton, wbrush);
					DeleteObject(wbrush);
				}
				else {
					rgbbrush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[2], currentConfig[4]));
					FillRect(globalHDC, lightMatrix + activeButton, rgbbrush);
					DeleteObject(rgbbrush);
				}

				ReleaseDC(hchild, globalHDC);
				EndPaint(hchild, &ps);

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
				globalHDC = GetDC(hchild);
				BeginPaint(hchild, &ps);
				rgbbrush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[2], currentConfig[4]));
				grayval = (currentConfig[0] + currentConfig[2] + currentConfig[4]) / 3;
				wbrush = CreateSolidBrush(RGB(grayval, grayval, grayval));
				for (int i = 0; i < 336; i++) {
					if (i % 2) {
						FillRect(globalHDC, lightMatrix + i, wbrush);
					}
					else {
						FillRect(globalHDC, lightMatrix + i, rgbbrush);
					}
				}

				ReleaseDC(hchild, globalHDC);
				DeleteObject(rgbbrush);
				DeleteObject(wbrush);
				EndPaint(hchild, &ps);
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
				globalHDC = GetDC(hchild);
				BeginPaint(hchild, &ps);
				if (activeButton % 2) {
					grayval = (currentConfig[0] + currentConfig[2] + currentConfig[4]) / 3;
					wbrush = CreateSolidBrush(RGB(grayval, grayval, grayval));
					FillRect(globalHDC, lightMatrix + activeButton, wbrush);
					DeleteObject(wbrush);
				}
				else {
					rgbbrush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[2], currentConfig[4]));
					FillRect(globalHDC, lightMatrix + activeButton, rgbbrush);
					DeleteObject(rgbbrush);
				}

				ReleaseDC(hchild, globalHDC);
				EndPaint(hchild, &ps);
			}
			else if (LOWORD(wParam) == ID_BN_4) {
				for (auto i = 0; i < 6; i++) {
					currentConfig[i] = 0;
					SendMessageW(LightCtrl[i], TBM_SETPOS, true, (LPARAM)currentConfig[i]);
					pos = currentConfig[i];
					wsprintfW(buf, L"%d", pos);
					SendMessageW(Texts[i], WM_SETTEXT, NULL, (LPARAM)buf);
					UpdateWindow(hmaster);
				}
				stage.adjustAll(turn_off);
				globalHDC = GetDC(hchild);
				BeginPaint(hchild, &ps);
				rgbbrush = CreateSolidBrush(RGB(0, 0, 0));
				for (int i = 0; i < 336; i++) {
					FillRect(globalHDC, lightMatrix + i, rgbbrush);
				}

				ReleaseDC(hchild, globalHDC);
				DeleteObject(rgbbrush);
				EndPaint(hchild, &ps);
			}
			else if (LOWORD(wParam) == ID_BN_5) {
				stage.setDefault(currentConfig);
			}
			else if (LOWORD(wParam) == ID_BN_6) {
				

				
			}
			else if (LOWORD(wParam) == ID_BN_7) {
			}
			else if (LOWORD(wParam) == ID_MN_1) {
				fileSave();
			}
			else if (LOWORD(wParam) == ID_MN_2) {
				fileOpen();
				stage.go();
				stage[selectedPowerSupply][selectedLightPos]->getValue(currentConfig);
				for (auto j = 0; j < 6; j++) {
					SendMessageW(LightCtrl[j], TBM_SETPOS, true, (LPARAM)currentConfig[j]);
					pos = currentConfig[j];
					wsprintfW(buf, L"%d", pos);
					SendMessageW(Texts[j], WM_SETTEXT, NULL, (LPARAM)buf);
					UpdateWindow(hmaster);
				}
				globalHDC = GetDC(hchild);
				BeginPaint(hchild, &ps);
				for (auto i = 0; i < 12; i++) {
					for (auto j = 0; j < 28; j++) {
						stage[i][j]->getValue(currentConfig);
						if (j % 2) {
							int grayval = (currentConfig[0] + currentConfig[2] + currentConfig[4]) / 3;
							wbrush = CreateSolidBrush(RGB(grayval, grayval, grayval));
							FillRect(globalHDC, lightMatrix + i * 28 + j, wbrush);
							DeleteObject(wbrush);
						}
						else {
							rgbbrush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[2], currentConfig[4]));
							FillRect(globalHDC, lightMatrix + i * 28 + j, rgbbrush);
							DeleteObject(rgbbrush);
						}
					}
				}
				ReleaseDC(hchild, globalHDC);
				EndPaint(hchild, &ps);
				UpdateWindow(hmaster);
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
				stage[selectedPowerSupply][selectedLightPos]->getValue(currentConfig);
				for (int i = 0; i < 6; i++) {
					pos = currentConfig[i];
					wsprintfW(buf, L"%d", pos);
					SendMessageW(Texts[i], WM_SETTEXT, NULL, (LPARAM)buf);
					SendMessageW(LightCtrl[i], TBM_SETPOS, true, (LPARAM)pos);
					UpdateWindow(Texts[i]);
					UpdateWindow(LightCtrl[i]);
				}
				hdc = GetDC(hchild);
				BeginPaint(hwnd, &ps);
				if (activeButton % 2) {
					int grayval = (currentConfig[0] + currentConfig[2] + currentConfig[4]) / 3;
					wbrush = CreateSolidBrush(RGB(grayval, grayval, grayval));
					FillRect(hdc, lightMatrix + activeButton, wbrush);
					DeleteObject(wbrush);
				}
				else {
					rgbbrush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[2], currentConfig[4]));
					FillRect(hdc, lightMatrix + activeButton, rgbbrush);
					DeleteObject(rgbbrush);
				}
				EndPaint(hwnd, &ps);
				ReleaseDC(hwnd, hdc);
				break;
			}
		}
		break;
	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		pen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
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

DeleteObject(pen);
EndPaint(hwnd, &ps);
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

LRESULT CALLBACK WindowProc3(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KILLFOCUS:
		break;
	case WM_CREATE:
		Label[6] = addLabel(hwnd, 10, 10, 200, 20, L"Choose a mode");
		CreateButton(hwnd, 160, 200, 50, 25, ID_P_BN_1, L"OK");
		CreateButton(hwnd, 230, 200, 50, 25, ID_P_BN_2, L"Cancel");
		RadioButton[0] = addRadioButton(hwnd, 10, 40, 70, 20, ID_P_RB_1, L"Demo", true);
		RadioButton[1] = addRadioButton(hwnd, 80, 40, 80, 20, ID_P_RB_2, L"Capture", false);
		RadioButton[2] = addRadioButton(hwnd, 160, 40, 130, 20, ID_P_RB_3, L"Precomputation", false);
		RadioButton[3] = addRadioButton(hwnd, 290, 40, 150, 20, ID_P_RB_4, L"Table Computation", false);
		SendMessageW(RadioButton[0], BM_SETCHECK, BST_CHECKED, NULL);
		Label[7] = addLabel(hwnd, 10, 70, 200, 20, L"Choose a lighting mode: ");
		RadioButton[4] = addRadioButton(hwnd, 10, 100, 90, 20, ID_P_RB_5, L"Rotation", true);
		RadioButton[5] = addRadioButton(hwnd, 100, 100, 70, 20, ID_P_RB_6, L"Static", false);
		SendMessageW(RadioButton[4], BM_SETCHECK, BST_CHECKED, NULL);
		Label[8] = addLabel(hwnd, 10, 130, 200, 20, L"Choose a display mode: ");
		RadioButton[6] = addRadioButton(hwnd, 10, 160, 70, 20, ID_P_RB_7, L"RGBW", true);
		RadioButton[7] = addRadioButton(hwnd, 80, 160, 65, 20, ID_P_RB_8, L"RGB", false);
		Label[9] = addLabel(hwnd, 10, 70, 200, 20, L"Choose a BRDF mode: ");
		RadioButton[8] = addRadioButton(hwnd, 10, 100, 110, 20, ID_P_RB_9, L"Aquisition", true);
		RadioButton[9] = addRadioButton(hwnd, 120, 100, 110, 20, ID_P_RB_10, L"Processing", false);
		Label[10] = addLabel(hwnd, 10, 70, 200, 20, L"Choose the table to compute: ");
		RadioButton[10] = addRadioButton(hwnd, 10, 100, 200, 20, ID_P_RB_11, L"Latitude Longitude Table", true);
		RadioButton[11] = addRadioButton(hwnd, 210, 100, 300, 20, ID_P_RB_12, L"Relighting Table", false);
		SendMessageW(RadioButton[6], BM_SETCHECK, BST_CHECKED, NULL);
		SendMessageW(RadioButton[8], BM_SETCHECK, BST_CHECKED, NULL);
		ShowWindow(Label[9], SW_HIDE);
		ShowWindow(RadioButton[8], SW_HIDE);
		ShowWindow(RadioButton[9], SW_HIDE);
		ShowWindow(Label[10], SW_HIDE);
		ShowWindow(RadioButton[10], SW_HIDE);
		ShowWindow(RadioButton[11], SW_HIDE);
		
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			if (LOWORD(wParam) == ID_P_BN_1) {
				SendMessageW(hwnd, WM_CLOSE, NULL, NULL);
				if (SendMessageW(RadioButton[0], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
					demoRoutine();
				}
				else if (SendMessageW(RadioButton[1], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
					captureRoutine();
				}
				else if (SendMessageW(RadioButton[2], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
					precomRoutine();
				}
				else if (SendMessageW(RadioButton[2], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
					tableRoutine();
				}

			}
			else if (LOWORD(wParam) == ID_P_BN_2) {
				SendMessageW(hwnd, WM_CLOSE, NULL, NULL);
			}
			else if (LOWORD(wParam) == ID_P_RB_1) {
				ShowWindow(Label[7], SW_SHOW);
				ShowWindow(Label[8], SW_SHOW);
				ShowWindow(RadioButton[4], SW_SHOW);
				ShowWindow(RadioButton[5], SW_SHOW);
				ShowWindow(RadioButton[6], SW_SHOW);
				ShowWindow(RadioButton[7], SW_SHOW);
				ShowWindow(Label[9], SW_HIDE);
				ShowWindow(RadioButton[8], SW_HIDE);
				ShowWindow(RadioButton[9], SW_HIDE);
				ShowWindow(Label[10], SW_HIDE);
				ShowWindow(RadioButton[10], SW_HIDE);
				ShowWindow(RadioButton[11], SW_HIDE);
			}
			else if (LOWORD(wParam) == ID_P_RB_2) {
				ShowWindow(Label[7], SW_HIDE);
				ShowWindow(Label[8], SW_HIDE);
				ShowWindow(RadioButton[4], SW_HIDE);
				ShowWindow(RadioButton[5], SW_HIDE);
				ShowWindow(RadioButton[6], SW_HIDE);
				ShowWindow(RadioButton[7], SW_HIDE);
				ShowWindow(Label[9], SW_SHOW);
				ShowWindow(RadioButton[8], SW_SHOW);
				ShowWindow(RadioButton[9], SW_SHOW);
				ShowWindow(Label[10], SW_HIDE);
				ShowWindow(RadioButton[10], SW_HIDE);
				ShowWindow(RadioButton[11], SW_HIDE);
			}
			else if (LOWORD(wParam) == ID_P_RB_3) {
				ShowWindow(Label[7], SW_HIDE);
				ShowWindow(Label[8], SW_HIDE);
				ShowWindow(RadioButton[4], SW_HIDE);
				ShowWindow(RadioButton[5], SW_HIDE);
				ShowWindow(RadioButton[6], SW_HIDE);
				ShowWindow(RadioButton[7], SW_HIDE);
				ShowWindow(Label[9], SW_HIDE);
				ShowWindow(RadioButton[8], SW_HIDE);
				ShowWindow(RadioButton[9], SW_HIDE);
				ShowWindow(Label[10], SW_HIDE);
				ShowWindow(RadioButton[10], SW_HIDE);
				ShowWindow(RadioButton[11], SW_HIDE);
			}
			else if (LOWORD(wParam) == ID_P_RB_4) {
				MessageBoxW(hwnd, L"!!WARNING!! ONLY USE THIS MODE IF YOU WISH TO CHANGE THE LIGHTS POSITIONS!", L"Warning", MB_OK);
				ShowWindow(Label[7], SW_HIDE);
				ShowWindow(Label[8], SW_HIDE);
				ShowWindow(RadioButton[4], SW_HIDE);
				ShowWindow(RadioButton[5], SW_HIDE);
				ShowWindow(RadioButton[6], SW_HIDE);
				ShowWindow(RadioButton[7], SW_HIDE);
				ShowWindow(Label[9], SW_SHOW);
				ShowWindow(RadioButton[8], SW_HIDE);
				ShowWindow(RadioButton[9], SW_HIDE);
				ShowWindow(Label[10], SW_SHOW);
				ShowWindow(RadioButton[10], SW_SHOW);
				ShowWindow(RadioButton[11], SW_SHOW);
			}
		}
		break;
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
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

HWND WINAPI addRadioButton(HWND hwnd, int x, int y, int width, int height, int handle, wchar_t * tag, bool isGrouped)
{
	HWND radioButton;
	if (isGrouped) {
		radioButton = CreateWindowExW(NULL, L"BUTTON", tag, BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_GROUP, x, y, width, height, hwnd, (HMENU)handle, NULL, NULL);
	}
	else {
		radioButton = CreateWindowExW(NULL, L"BUTTON", tag, BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, x, y, width, height, hwnd, (HMENU)handle, NULL, NULL);
	}
	return radioButton;
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

HWND WINAPI addCheckBox(HWND hwnd, int x, int y, int width, int height, int handle, wchar_t * tag, bool isGrouped)
{
	HWND temp;
	if (isGrouped) {
		temp = CreateWindowExW(NULL, L"BUTTON", tag, BS_CHECKBOX | WS_VISIBLE | WS_CHILDWINDOW | WS_GROUP, x, y, width, height, hwnd, (HMENU)handle, NULL, NULL);
	}
	else {
		temp = CreateWindowExW(NULL, L"BUTTON", tag, BS_CHECKBOX | WS_VISIBLE | WS_CHILDWINDOW, x, y, width, height, hwnd, (HMENU)handle, NULL, NULL);
	}
	return temp;
}

HWND WINAPI CreateComboEx(HWND hwnd)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_USEREX_CLASSES;
	InitCommonControlsEx(&icex);
	HWND temp = CreateWindowEx(0, WC_COMBOBOX, NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | CBS_DROPDOWN, 1265, 155, 300, 500, hwnd, NULL, NULL, NULL);
	return temp;
}

LRESULT CALLBACK subEditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	HDC hdc;
	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RETURN:
			for (auto i = 0; i < 6; i++) {
				SendMessageW(Texts[i], WM_GETTEXT, 4, (LPARAM)buf);
				pos = stoi(buf);
				if (pos > 255) {
					pos = 255;
				}
				else if (pos < 0) {
					pos = 0;
				}
				currentConfig[i] = pos;
				SendMessageW(LightCtrl[i], TBM_SETPOS, true, (LPARAM)pos);
			}
			updateLight();
			hdc = GetDC(hchild);
			BeginPaint(hchild, &ps);
			if (activeButton % 2) {
				int grayval = (currentConfig[0] + currentConfig[2] + currentConfig[4]) / 3;
				wbrush = CreateSolidBrush(RGB(grayval, grayval, grayval));
				FillRect(hdc, lightMatrix + activeButton, wbrush);
				DeleteObject(wbrush);
			}
			else {
				rgbbrush = CreateSolidBrush(RGB(currentConfig[0], currentConfig[2], currentConfig[4]));
				FillRect(hdc, lightMatrix + activeButton, rgbbrush);
				DeleteObject(rgbbrush);
			}
			EndPaint(hchild, &ps);
			ReleaseDC(hchild, hdc);
			break;
		}
	case WM_PAINT:
		break;
	default:
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}
	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK subCVProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	switch (uMsg)
	{
	case WM_CREATE:
		SetFocus(hwnd);
	case WM_LBUTTONDOWN:
		SetFocus(hwnd);
		rotationInterrupted = true;
		break;
	case WM_KEYDOWN:
		if (rotationInterrupted == false) {
			rotationInterrupted = true;
		}
		else {
			rotationInterrupted = false;
			demoRoutine();
		}
		break;
	case WM_DESTROY:
		ShowWindow(hwnd, SW_HIDE);
		break;
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		break;
	default:
		return DefSubclassProc(hwnd, uMsg, wParam, lParam);
	}
	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void addMenu(HWND hwnd)
{
	Menu[0] = CreateMenu();
	Menu[1] = CreateMenu();
	AppendMenuW(Menu[0], MF_POPUP, (UINT_PTR)Menu[1], L"File");
	AppendMenuW(Menu[1], MF_STRING, ID_MN_1, L"Save Value Map");
	AppendMenuW(Menu[1], MF_STRING, ID_MN_2, L"Load Value Map");
	AppendMenuW(Menu[0], MF_POPUP, NULL, L"File3");
	SetMenu(hwnd, Menu[0]);
}

void updateLight()
{
	stage[selectedPowerSupply][selectedLightPos]->setValue(currentConfig);
	if (!whiteSelected) {
		stage.go(selectedPowerSupply * 2);
	}
	else {
		stage.go(selectedPowerSupply * 2 + 1);
	}
	Sleep(20);
}

void fileOpen() {
	OPENFILENAMEW ofn;
	wchar_t szFile[260];
	wchar_t dirBuffer[260];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hmaster;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Light Stage Value Map (.valmap)\0*.valmap*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	GetCurrentDirectoryW(260, dirBuffer);
	ofn.lpstrInitialDir = dirBuffer;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn) == true) {
		char temp[260];
		ZeroMemory(temp, sizeof(temp));
		wcstombs(temp, ofn.lpstrFile, lstrlenW(ofn.lpstrFile));
		if (temp[0] != '\0') {
			stage.loadMap(temp);
		}
	}
}

void fileSave() {
	OPENFILENAMEW ofn;
	wchar_t szFile[260];
	wchar_t dirBuffer[260];
	HANDLE hf;
	DWORD dw;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hmaster;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Light Stage Value Map (.valmap)\0*.valmap*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	GetCurrentDirectoryW(260, dirBuffer);
	ofn.lpstrInitialDir = dirBuffer;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetSaveFileNameW(&ofn);
	char temp[260];
	ZeroMemory(temp, sizeof(temp));
	wcstombs(temp, ofn.lpstrFile, lstrlenW(ofn.lpstrFile));
	if (temp[0] != '\0') {
		stage.saveMap(temp);
	}
}

void demoRoutine() {
	char * envName = new char[260];
	ZeroMemory(envName, sizeof(envName));
	SendMessageW(ComboBox, CB_GETLBTEXT, selectedCBItem, (LPARAM)environment);
	wcstombs(envName, environment, 30);
	string show_path_0 = "C:\\CO417-HW1\\voronoi_";
	show_path_0 += envName;
	show_path_0 += "\\original.png";

	Mat fr_0;
	fr_0 = imread(show_path_0, CV_LOAD_IMAGE_COLOR);
	imshow("Original", fr_0);
	waitKey(1000);

	namedWindow("Demo", WINDOW_NORMAL);
	HWND newWND = (HWND)cvGetWindowHandle("Demo");
	SetFocus(newWND);
	SetWindowSubclass(newWND, subCVProc, 0, 0);
	string show_path_1;
	show_path_1 = "C:\\CO417-HW1\\voronoi_";
	show_path_1 += envName;
	show_path_1 += "\\centers.png";

	Mat fr_1;
	fr_1 = imread(show_path_1, CV_LOAD_IMAGE_COLOR);
	imshow("Demo", fr_1);
	waitKey(1000);

	string show_path_2;
	show_path_2 = "C:\\CO417-HW1\\voronoi_";
	show_path_2 += envName;
	show_path_2 += "\\centers_lines.png";

	Mat fr_2;
	fr_2 = imread(show_path_2, CV_LOAD_IMAGE_COLOR);
	imshow("Demo", fr_2);
	waitKey(1000);

	string show_path_3;
	show_path_3 = "C:\\CO417-HW1\\voronoi_";
	show_path_3 += envName;
	show_path_3 += "\\rotation_x12\\rotated0.0.png";

	Mat fr_3;
	fr_3 = imread(show_path_3, CV_LOAD_IMAGE_COLOR);
	imshow("Demo", fr_3);
	waitKey(1000);
	if (SendMessageW(RadioButton[4], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
		if (SendMessageW(RadioButton[6], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
			while (!rotationInterrupted) {
				rotationIndex = 0;
				//LAUNCH LIGHT STAGE 1 file
				string in_path;
				in_path = "C:\\CO417-HW1\\voronoi_";
				in_path += envName;
				in_path += "\\txt\\global_converted.txt";

				ifstream ifs(in_path);

				int no, arc, index;
				float r, g, b;
				while (ifs >> no >> arc >> index >> r >> g >> b)
				{
					stage(arc, index)->setValue(r, 0, g, 0, b, 0);

					if (no == 174)
					{
						stage.go();
						string animation_path;
						animation_path = "C:\\CO417-HW1\\voronoi_";
						animation_path += envName;
						animation_path += "\\rotation\\rotated";
						animation_path += to_string(rotationIndex);
						animation_path += ".png";
						play_one_rotation(animation_path, rotationIndex);
						rotationIndex++;
						waitKey(20);
					}
				}
			}
			rotationInterrupted = false;
			cin.ignore();
			string show_path_0;
			show_path_0 = "C:\\CO417-HW1\\voronoi_";
			show_path_0 += envName;
			show_path_0 += "\\original.png";

			Mat fr_0;
			fr_0 = imread(show_path_0, CV_LOAD_IMAGE_COLOR);
			imshow("Original", fr_0);
			waitKey(1000);
		}
		else if (SendMessageW(RadioButton[6], BM_GETCHECK, NULL, NULL) == BST_UNCHECKED) {
			while (!rotationInterrupted)
			{
				rotationIndex = 0;
				//LAUNCH LIGHT STAGE 1 file
				string in_path;
				in_path = "C:\\CO417-HW1\\voronoi_";
				in_path += envName;
				in_path += "\\txt\\global_converted.txt";

				ifstream ifs(in_path);

				int no, arc, index;
				float r, g, b;
				while (ifs >> no >> arc >> index >> r >> g >> b)
				{
					stage(arc, index)->setValue(r, 0, g, 0, b, 0);

					if (no == 174)
					{
						stage.go();
						string animation_path;
						animation_path = "C:\\CO417-HW1\\voronoi_";
						animation_path += envName;
						animation_path += "\\rotation\\rotated";
						animation_path += to_string(rotationIndex);
						animation_path += ".png";
						play_one_rotation(animation_path, rotationIndex);
						rotationIndex++;
						waitKey(20);
					}
				}
			}
			cin.ignore();
			rotationInterrupted;

			string show_path_0;
			show_path_0 = "C:\\CO417-HW1\\voronoi_";
			show_path_0 += envName;
			show_path_0 += "\\original.png";

			Mat fr_0;
			fr_0 = imread(show_path_0, CV_LOAD_IMAGE_COLOR);
			imshow("Original", fr_0);
			waitKey(1000);

		}
	
	}
	else if (SendMessageW(RadioButton[4], BM_GETCHECK, NULL, NULL) == BST_UNCHECKED) {
		if (SendMessageW(RadioButton[6], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
			string in_path;
			in_path = "C:\\CO417-HW1\\voronoi_";
			in_path += envName;
			in_path += "\\txt\\global_rgb_w.txt";


			ifstream ifs(in_path);

			int no, arc, index;
			float r, g, b;
			while (ifs >> no >> arc >> index >> r >> g >> b && !(no == 174 && index == 13))
			{
				stage(arc, index)->setValue(r, 0, g, 0, b, 0);
			}

			stage.go();
			Sleep(1000);
		}
		else if (SendMessageW(RadioButton[6], BM_GETCHECK, NULL, NULL) == BST_UNCHECKED) {

			//NO ROTATION ONLY RGB:
			string in_path;
			in_path = "C:\\CO417-HW1\\voronoi_";
			in_path += envName;
			in_path += "\\txt\\global_converted.txt";


			ifstream ifs(in_path);

			int no, arc, index;
			float r, g, b;
			while (ifs >> no >> arc >> index >> r >> g >> b && !(no == 174 && index == 13))
			{
				stage(arc, index)->setValue(r, 0, g, 0, b, 0);
			}

			stage.go();
			Sleep(1000);
		}
	}
	delete[] envName;
}

void captureRoutine() {
	if (SendMessageW(RadioButton[8], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
		//FOLDERS CREATION:
		wstring dir_IBRL = L"IBRL";
		CreateDirectoryW(dir_IBRL.c_str(), NULL);


		wstring dir_imgs(dir_IBRL);
		dir_imgs += L"\\images";
		CreateDirectoryW(dir_imgs.c_str(), NULL);


		wstring dir_imgs_multiplied(dir_IBRL);
		dir_imgs_multiplied += L"\\images_multiplied";
		CreateDirectoryW(dir_imgs_multiplied.c_str(), NULL);
		//END FOLDERS CREATION
		RunSingleCamera16(stage, cam, "C:\\CO417-HW1\\IBRL\\images\\");
	}
	else if (SendMessageW(RadioButton[9], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
		char * envName = new char[260];
		ZeroMemory(envName, sizeof(envName));
		SendMessageW(ComboBox, CB_GETLBTEXT, selectedCBItem, (LPARAM)environment);
		wcstombs(envName, environment, 30);

		for (unsigned int i = 0; i < 168; i++) //image i
		{
			multiply_image_i("C:\\CO417-HW1\\IBRL\\images\\", "C:\\CO417-HW1\\IBRL\\images_multiplied\\", i, envName);
		}



		sum_images("C:\\CO417-HW1\\IBRL\\images_multiplied\\", "C:\\CO417-HW1\\IBRL\\");
		delete[] envName;
	}
}

void precomRoutine() {

	//FOLDERS CREATION:
	wstring dir_env = L"C:\\CO417-HW1\\voronoi_";
	dir_env += environment;
	CreateDirectory(dir_env.c_str(), NULL);


	wstring dir_txt(dir_env);
	dir_txt += L"\\txt";
	CreateDirectory(dir_txt.c_str(), NULL);


	wstring dir_rot_conv(dir_env);
	dir_rot_conv += L"\\txt\\rotations_converted";
	CreateDirectory(dir_rot_conv.c_str(), NULL);


	wstring dir_rot_x12(dir_env);
	dir_rot_x12 += L"\\rotation_x12";
	CreateDirectory(dir_rot_x12.c_str(), NULL);


	wstring dir_rot(dir_env);
	dir_rot += L"\\rotation";
	CreateDirectory(dir_rot.c_str(), NULL);
	//END FOLDERS CREATION







	cout << "Creating voronoi diagram" << endl;

	//VORONOI DIAGRAM CREATION
	wstring PFM_path = L"C:\\CO417-HW1\\GraceCathedral\\";
	PFM_path += environment;
	PFM_path += L".pfm";

	char * char_path = new char[260];
	ZeroMemory(char_path, sizeof(char_path));
	wcstombs(char_path, PFM_path.c_str(), 30);
	Mat img = loadPFM(char_path);
	int width(img.cols), height(img.rows);




	wstring voro_path;
	voro_path = L"C:\\CO417-HW1\\voronoi_";
	voro_path += environment;
	wstring no_gamma_path(voro_path);

	voro_path += L"\\gamma_corrected.png";
	no_gamma_path += L"\\weighted.pfm";

	char * envName = new char[260];
	ZeroMemory(envName, sizeof(envName));
	SendMessageW(ComboBox, CB_GETLBTEXT, selectedCBItem, (LPARAM)environment);
	wcstombs(envName, environment, 30);

	ZeroMemory(char_path, sizeof(char_path));
	wcstombs(char_path, voro_path.c_str(), 30);
	Mat img_vo = apply_voronoi(img, envName, "C:\\CO417-HW1\\latlon.txt", char_path, true);

	ZeroMemory(char_path, sizeof(char_path));
	wcstombs(char_path, no_gamma_path.c_str(), 30);
	delete_gamma(img_vo);
	savePFM(img_vo, char_path);
	//END VORONOI DIAGRAM CREATION


	cout << "Renaming the rotated images" << endl;

	//RENAME ROTATED IMAGES
	string in;
	in = "C:\\CO417-HW1\\voronoi_";
	in += envName;
	in += "\\rotation_x12\\rotated";

	string out;
	out = "C:\\CO417-HW1\\voronoi_";
	out += envName;
	out += "\\rotation\\rotated";

	rename_rotation(in, out);
	//END RENAME ROTATED IMAGES

	cout << "Storing data" << endl;

	//STORE DATA
	for (unsigned int i = 0; i < get_rot_complete().size(); i++) //image i
	{
		string out_path;
		out_path = "C:\\CO417-HW1\\voronoi_";
		out_path += envName;
		out_path += "\\txt\\rotations_converted\\rotation";
		out_path += to_string(i);
		out_path += ".txt";

		save_vec(out_path, get_rot_complete()[i]);

		cout << "rotation no:" << i << endl;
	}
	//END STORE DATA


	cout << "Compressing data" << endl;

	//COMPRESS DATA 1 file
	for (unsigned int i = 0; i < 361; i++) //image i
	{


		string in_path;
		in_path = "C:\\CO417-HW1\\voronoi_";
		in_path += envName;
		in_path += "\\txt\\rotations_converted\\rotation";
		in_path += to_string(i);
		in_path += ".txt";

		ifstream ifs(in_path);


		cout << "rotation no:" << i << endl;



		string out_path;
		out_path = "C:\\CO417-HW1\\voronoi_";
		out_path += envName;
		out_path += "\\txt\\global_converted.txt";

		ofstream ofs;
		ofs.open(out_path, std::ios_base::app);

		int no, arc, index;
		float r, g, b;
		while (ifs >> no >> arc >> index >> r >> g >> b)
		{
			ofs << no << ' ' << arc << ' ' << index << ' ' << r << ' ' << g << ' ' << b << endl;
		}
	}
	//END COMPRESS DATA 1 file

	cout << "Converting to HSV" << endl;

	//CONVERSION RGB TO HSV
	string rgb_path;
	rgb_path = "C:\\CO417-HW1\\voronoi_";
	rgb_path += envName;
	rgb_path += "\\txt\\global_converted.txt";

	string hsv_path;
	hsv_path = "C:\\CO417-HW1\\voronoi_";
	hsv_path += envName;
	hsv_path += "\\txt\\global_rgb_hsv.txt";

	rgb_to_hsv_file(rgb_path, hsv_path);
	//END CONVERSION RGB TO HSV

	cout << "Converting to RGB and W" << endl;

	//CONVERSION TO WEIGHTED RGB AND W*3
	string rgb_hsv_path;
	rgb_hsv_path = "C:\\CO417-HW1\\voronoi_";
	rgb_hsv_path += envName;
	rgb_hsv_path += "\\txt\\global_rgb_hsv.txt";

	string rgb_w_path;
	rgb_w_path = "C:\\CO417-HW1\\voronoi_";
	rgb_w_path += envName;
	rgb_w_path += "\\txt\\global_rgb_w.txt";

	compute_ratio_file(rgb_hsv_path, rgb_w_path);
	//END CONVERSION TO WEIGHTED RGB AND W*3



	//re_compute_file("C:\\CO417-HW1\\voronoi_Y-gradient\\txt\\global_rgb_w.txt", "C:\\CO417-HW1\\voronoi_Y-gradient\\txt\\global_rgb_w_plus.txt");

}

void tableRoutine() {
	if (SendMessageW(RadioButton[10], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
		//CONVERT XYZ COORDINATES INTO LATITUDE-LONGITUDE COORDINATES
		int width = 1024;
		int height = 512;
		convert_data("C:\\CO417-HW1\\points.txt", "C:\\CO417-HW1\\latlon.txt", width, height);

		//CREATE CONVERSION TABLE BETWEEN (ARC, INDEX) (Huseng's) AND NUMBER (Alix's) LIGHT ADDRESSING WAYS
		vector<Point2f> points;
		sort_data("C:\\CO417-HW1\\latlon.txt", points);
		build_table("C:\\CO417-HW1\\table.txt", points, width, height);
		invert_table("C:\\CO417-HW1\\table.txt", "C:\\CO417-HW1\\table_inv_minus.txt");
		fix_table("C:\\CO417-HW1\\table_inv_minus.txt", "C:\\CO417-HW1\\table_inv.txt");
	}
	else if (SendMessageW(RadioButton[11], BM_GETCHECK, NULL, NULL) == BST_CHECKED) {
		string table_path;
		table_path = "C:\\CO417-HW1\\relighting_table.txt";
		compute_relighting_table(table_path);
	}
}