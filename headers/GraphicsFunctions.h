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
	int totalCount;
}GFDRAWEDLINES;

GFDRAWEDLINES* GetDrawedLines();

void GF_DrawTextAll(SEGMENT* segments, TOCURSORPOS* caretpos,int segmentCount, int newLinePointer, int oldLinePointer);
void GF_DrawTextByLine(SEGMENT* segments, TOCURSORPOS* caretpos,int segmentCount, int newLinePointer, int oldLinePointer);
void GF_Init(HWND hWnd, RECT rect);
void GF_SetCursorPos(SEGMENT* segments, TOCURSORPOS* caretpos, int segmentCount);
int GF_CalcScrollDistanceAndBorder(SEGMENT* segments, int* borderTop, int segmentsCount, int scrolledLines, int* inSegmentLine, int* segmentPointer);