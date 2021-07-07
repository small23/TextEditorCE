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
	int linesDrawed;
	int totalCount;
}GFDRAWEDLINES;

GFDRAWEDLINES* GF_GetDrawedLines();
void GF_DrawTextAll(SEGMENT* segments, TOCURSORPOS* caretpos,int segmentCount, int newLinePointer);
void GF_DrawTextByLine(SEGMENT* segments, TOCURSORPOS* caretpos,int segmentCount, int newLinePointer);
void GF_Init(HWND hWnd, RECT rect);
RECT GF_GetRect();
void GF_RectChange(RECT rectangle);

void _GF_SetCursorPos(SEGMENT* segments, TOCURSORPOS* caretpos, int segmentCount);
int _GF_CalcScrollDistanceAndBorder(SEGMENT* segments, int* borderTop, int segmentsCount, int scrolledLines, int* inSegmentLine, int* segmentPointer);
void _GF_LocateTextCoords(SEGMENT* segments, int segmentsCount, int newLine, int* segmentPointer, int* inSegmentLine);
void _GF_DrawText(SEGMENT* segments, int segmentsCount, HDC hdc,int segmentPointer, int inSegmentLine, int scrolledLines, int borderTop);
_inline bool _GF_CheckSegmentPointers(SEGMENT* segments, int* segmentPointer, int* inSegmentLine, int segmentsCount);