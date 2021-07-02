#pragma once

#include <windows.h>
#include <stdlib.h>
#include "GlobalDefinitions.h"
#include "textData.h"

typedef struct
{
	int segment;
	int lineBegin;
	int lineCount;

}GFDRAWEDLINES;

void GF_DrawTextByLine(SEGMENT* segments,int segmentCount, int a, int b , int c, HWND hWnd, RECT rect);
void GF_DrawTextByLineChoosenHDC(SEGMENT* segments,int segmentCount, int newLinePointer, int oldLinePointer , int redrawAll, int pageSize, HDC hdc, RECT rect);
void GF_DrawCursor(HDC hdc);
GFDRAWEDLINES* GetDrawedLines();