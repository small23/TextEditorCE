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
	int lastWidth;
}TOCURSORPOS;

void TO_GetTextSegments(SEGMENT* segments, HWND hWnd,  RECT rect);
void TO_RecheckSpacesAndLines(SEGMENT* segments, int i, HDC hdc,RECT rect);
void TO_InsertSymbol(SEGMENT* segments, TOCURSORPOS* carrage, wchar_t simbol, HDC hdc, RECT rect);
void TO_DeleteSymbol(SEGMENT* segments, TOCURSORPOS* carrage, HDC hdc, RECT rect);
void TO_CalcCarragePos(SEGMENT* segments,TOCURSORPOS* carrage,HDC hdc, RECT rect);
void TO_CreateFont(unsigned short* font, int size, unsigned char italic, unsigned char underline);
bool TO_CheckSegmentPointers(SEGMENT* segments,int* segmentPointer, int* inSegmentLine);
void TO_Arrows(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, WPARAM arrow);
void TO_ArrowUpDownPosCalc(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect);
int TO_GetSegmentCount();
HFONT* TO_GetFonts(int *ArrayCounter);
