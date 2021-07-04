#pragma once

#include <windows.h>
#include <stdlib.h>
#include "GlobalDefinitions.h"
#include "TextOperations.h"
#include "textData.h"

typedef struct
{
	int segment;
	int lineBegin;
	int lineCount;

}GFDRAWEDLINES;

void GF_DrawTextAll(SEGMENT* segments, TOCURSORPOS* carrage,int segmentCount, int newLinePointer, int oldLinePointer , HWND hWnd, RECT rect);
void GF_DrawTextByLine(SEGMENT* segments, TOCURSORPOS* carrage,int segmentCount, int newLinePointer, int oldLinePointer , int redrawAll, int pageSize, HWND hWnd, RECT rect);
void GF_DrawCursor(HDC hdc);
GFDRAWEDLINES* GetDrawedLines();