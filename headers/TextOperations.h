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

void TO_GetTextSegments(HWND hWnd, SEGMENT* segments,int* segmentsCount);
void TO_RecheckSpacesAndLines(SEGMENT* segments, int i, HDC hdc,RECT rect);
void TO_InsertSymbol(SEGMENT* segments, TOCURSORPOS* carrage, wchar_t simbol, HDC hdc, RECT rect);
void TO_DeleteSymbol(SEGMENT* segments, TOCURSORPOS* carrage, HDC hdc, RECT rect);
void TO_CreateFont();
HFONT* TO_GetFonts(int *ArrayCounter);