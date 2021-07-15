#pragma once

#include "stdafx.h"
#include "windows.h"

typedef struct
{
	int nNumFonts;
	TCHAR szFontFamily[LF_FACESIZE];
	TCHAR szShortName[LF_FACESIZE];
} FSFONTFAMILY;


HFONT* FS_GetFonts(int *ArrayCounter);
void FS_CreateFont(unsigned short* font, int size, unsigned char italic, unsigned char underline);
FSFONTFAMILY* FS_GetFontFamily(int i);
int FS_GetFontsCounter();
void FS_InitFonts(HWND hWnd);


int CALLBACK _FS_FontsCallback(CONST LOGFONT *lplf, CONST TEXTMETRIC *lpntm, DWORD nFontType, LPARAM lParam);
int CALLBACK _FS_EnumSingleFontFamily(CONST LOGFONT *lplf, CONST TEXTMETRIC *lpntm, DWORD nFontType, LPARAM lParam);