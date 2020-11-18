#pragma once

//#include <windows.h>
#include <stdlib.h>
#include "GlobalDefinitions.h"
//#include "DemoText.h"
#include "textData.h"

typedef struct
{ 
	int segment;
	int position;
}TOCURSORPOS;

void GetTextSegments(HWND hWnd, SEGMENT* segments,int* segmentsCount);
void RecheckSpacesAndLines(SEGMENT* segments, int i, HDC hdc,RECT rect);
void InsertSymbol(SEGMENT* segments, int segmentNum, int position, wchar_t simbol, HDC hdc, RECT rect);
void CreateFont();
HFONT* GetFonts(int *ArrayCounter);