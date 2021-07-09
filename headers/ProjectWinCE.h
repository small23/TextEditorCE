#pragma once

#include <Afxwin.h>
#include <windows.h>
#include <stdlib.h>
#include "resource.h"
#include <Commdlg.h>
#include <commctrl.h>  
#include "GlobalDefinitions.h"
#include "textData.h"
#include "TextOperations.h"
#include "GraphicsFunctions.h"
#include "DebugFunctions.h"


LRESULT CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT KeydownHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT CharHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT CommandHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT VsScrollHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT PaintHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);



void Setup(HWND lParam);
int GetFileName (HWND hWnd, LPTSTR szFileName, int nMax);
int CreateCommandBand (HWND hWnd, BOOL fFirst);
int InitInstance(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPWSTR lpCmdLine, int nCmdShow);

COMMANDBANDSRESTOREINFO cbr[NUMBANDS];
INT nBandOrder[NUMBANDS];

SEGMENT *segments;
TOCURSORPOS carrage={0,0,0,0,16,0};


HINSTANCE		hInst;
HWND			hwndCB;			// The command bar handle
HWND			hwndCBND;
HWND			hwndBand;
HWND			hwndSB;
HWND			hwndMW;
HWND			hwndFONT;
HWND			hwndFONTSIZE;
HFONT			hfont;

const TBBUTTON tbCBStdBtns[] = {
	//  BitmapIndex      Command  State            Style    UserData  String
    {STD_FILEOPEN,   211,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,    0},
    {STD_FILESAVE,   212,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,    0},
    {0,              0,       TBSTATE_ENABLED, TBSTYLE_SEP,    0,    0},
    {STD_CUT,        213,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,    0},
    {STD_COPY,       214,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,    0},
    {STD_PASTE,      215,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,    0},
    {0,              0,       TBSTATE_ENABLED, TBSTYLE_SEP,    0,    0},
	{STD_UNDO,		 216,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,    0},
	{VIEW_LIST,		 217,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,    0},
    {STD_PROPERTIES, 218,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0,    0},
	};

