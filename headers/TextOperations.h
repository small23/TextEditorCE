#pragma once

#include <stdlib.h>
#include "GlobalDefinitions.h"
#include "FontsStorage.h"

typedef struct
{ 
	int segment;
	int position;
	int lineLocation;
	int x;
	int height;
	int lastWidth;
}TOCURSORPOS;

typedef struct
{
	int linesCounter;
	int length;
	int* lineEnds;
	int* spacesPointer;
	int spacesCounter;
	int*  linesLength;
	int*  linesHeight;
	wchar_t* text;
	int textArraySize;

}SEGMENT;

void TO_GetTextSegments(SEGMENT* segments, HWND hWnd,  RECT rect);
void TO_RecheckSpacesAndLines(SEGMENT* segments, int i, HDC hdc,RECT rect);
void TO_InsertSymbol(SEGMENT* segments, TOCURSORPOS* carrage, wchar_t simbol, HDC hdc, RECT rect);
void TO_DeleteSymbol(SEGMENT* segments, TOCURSORPOS* carrage, HDC hdc, RECT rect);
void TO_CalcCarragePos(SEGMENT* segments,TOCURSORPOS* carrage,HDC hdc, RECT rect);
bool TO_CheckSegmentPointers(SEGMENT* segments,int* segmentPointer, int* inSegmentLine);
void TO_Arrows(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, WPARAM arrow);
void TO_ArrowUpDownPosCalc(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, int counter);
void TO_HomeEnd(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, WPARAM arrow);
int TO_GetSegmentCount();
