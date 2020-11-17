#pragma once

#include <Afxwin.h>
#include <windows.h>
#include <stdlib.h>
#include "resource.h"
#include <Commdlg.h>
#include <Commctrl.h>
#include "GlobalDefinitions.h"
#include "textData.h"
#include "TextOperations.h"
#include "GraphicsFunctions.h"
#include "DebugFunctions.h"

LRESULT CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void Setup(HWND lParam);
int GetFileName (HWND hWnd, LPTSTR szFileName, int nMax);

int oldPos;
int segmentsCount=0;

SEGMENT *segments;

HINSTANCE			hInst;
HWND				hwndCB;			// The command bar handle
HWND				hwndCBND;
HWND				hwndSB;
HWND				hwndMW;
HFONT				hfont;