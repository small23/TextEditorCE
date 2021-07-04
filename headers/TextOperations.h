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
	int lineLocation;
	int x;
	int height;
}TOCURSORPOS;

void TO_GetTextSegments(SEGMENT* segments,int* segmentsCount, HWND hWnd,  RECT rect);
void TO_RecheckSpacesAndLines(SEGMENT* segments, int i, HDC hdc,RECT rect);
void TO_InsertSymbol(SEGMENT* segments, TOCURSORPOS* carrage, wchar_t simbol, HDC hdc, RECT rect);
void TO_DeleteSymbol(SEGMENT* segments, TOCURSORPOS* carrage, HDC hdc, RECT rect);
void TO_CalcCarragePos(SEGMENT* segments,TOCURSORPOS* carrage,HDC hdc, RECT rect);
void TO_CreateFont();
HFONT* TO_GetFonts(int *ArrayCounter);