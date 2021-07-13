#pragma once

#include <windows.h>
#include <stdlib.h>
#include "GlobalDefinitions.h"
#include "TextOperations.h"
#include "FontsStorage.h"

typedef struct
{
	int segment;
	int lineBegin;
	int linesDrawed;
	int totalCount;
}GFDRAWEDLINES;

GFDRAWEDLINES* GF_GetDrawedLines();
void GF_DrawTextAll(SEGMENT* segments, TOCURSORPOS* caretpos, int newLinePointer);
void GF_DrawTextByLine(SEGMENT* segments, TOCURSORPOS* caretpos,int newLinePointer);
void GF_Init(HWND hWnd, RECT rect);
RECT GF_GetRect();
void GF_RectChange(RECT rectangle);
void GF_SetCursorPos(SEGMENT* segments, TOCURSORPOS* caretpos);

int _GF_CalcScrollDistanceAndBorder(SEGMENT* segments, int* borderTop, int scrolledLines, int* inSegmentLine, int* segmentPointer);
void _GF_LocateTextCoords(SEGMENT* segments, int newLine, int* segmentPointer, int* inSegmentLine);
void _GF_DrawText(SEGMENT* segments, HDC hdc,int segmentPointer, int inSegmentLine, int scrolledLines, int borderTop);
