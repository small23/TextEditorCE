#include "stdafx.h"
#include "GraphicsFunctions.h"
#include "TextOperations.h"


#define ChkAndHideCaret() \
	if (_caretIsShown)\
	{\
		_caretIsShown=false;\
		HideCaret(hWnd);\
	}

bool cursorHighLighted=false;
bool _caretIsShown=false; 
GFDRAWEDLINES drawedLines = {0,0,0,0};
int caretHeight=16;
HWND hWnd=NULL;
RECT rect;
RECT rectGlobal;


RECT GF_GetRectGlobal()
{
	return rectGlobal;
}

void GF_SaveGlobalRect(RECT rectangle)
{
	rectGlobal=rectangle;
}

RECT GF_GetRect()
{
	return rect;
}

GFDRAWEDLINES* GF_GetDrawedLines()
{
	return &drawedLines;	
}

void GF_Init(HWND hwnd, RECT rectangle)
{
	rect=rectangle;
	hWnd=hwnd;
	CreateCaret(hWnd, (HBITMAP) NULL, 2,16);
}

void GF_RectChange(RECT rectangle)
{
	rect=rectangle;
}

void GF_DrawTextByLine(SEGMENT* segments, TOCURSORPOS* carrage, int newLine)
{
	int scrolledLines=newLine-drawedLines.totalCount;
	int inSegmentLine=0;
	int segmentPointer=0;
	int scrollDistance;
	int borderTop=rect.top;


	if (scrolledLines==0)
		return;

	_GF_LocateTextCoords(segments, newLine, &segmentPointer, &inSegmentLine);

	scrollDistance = _GF_CalcScrollDistanceAndBorder(segments, &borderTop, scrolledLines, &inSegmentLine, &segmentPointer);

	ChkAndHideCaret();

	HDC hdc = GetDC(hWnd);

	if (scrollDistance>rect.bottom-rect.top)
	{
		inSegmentLine=drawedLines.lineBegin;
		segmentPointer=drawedLines.segment;
		borderTop=rect.top;
		scrolledLines=500;
		BitBlt(hdc, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
			NULL,0,0,WHITENESS);
	}
	else
	{		
		if (scrollDistance<0)
		{
			BitBlt(hdc,rect.left, rect.top, rect.right-rect.left, borderTop-rect.top,
				hdc, rect.left, rect.top-scrollDistance,SRCCOPY);
			BitBlt(hdc, rect.left, borderTop, rect.right-rect.left, rect.bottom-borderTop,
				NULL,0,0,WHITENESS);
			scrolledLines=abs(scrolledLines)+1;
		}
		else
		{
			BitBlt(hdc,rect.left, rect.top+scrollDistance, rect.right-rect.left, rect.bottom-scrollDistance-rect.top,
				hdc, rect.left, rect.top,SRCCOPY);
			BitBlt(hdc, rect.left, borderTop, rect.right-rect.left, abs(scrollDistance),
				NULL,0,0,WHITENESS);
			scrolledLines=abs(scrolledLines);
		}
	}

	_GF_DrawText(segments, hdc, segmentPointer, inSegmentLine, scrolledLines, borderTop);

	ReleaseDC(hWnd, hdc);
	GF_SetCursorPos(segments, carrage);
}

void GF_DrawTextAll(SEGMENT* segments, TOCURSORPOS* carrage, int newLine)
{
	PAINTSTRUCT ps;
    HDC hdc;
	int inSegmentLine=0;
	int segmentPointer=0;

	
	ChkAndHideCaret();
	
	_GF_LocateTextCoords(segments, newLine, &segmentPointer, &inSegmentLine);
	
	hdc = BeginPaint (hWnd, &ps); 
	
	_GF_DrawText(segments, hdc, segmentPointer, inSegmentLine, 999, rect.top);

	EndPaint (hWnd, &ps); 
	GF_SetCursorPos(segments, carrage);
}

void GF_SetCursorPos(SEGMENT* segments, TOCURSORPOS* caret)
{
	ChkAndHideCaret();

	int inSegmentLine=drawedLines.lineBegin;
	int segmentPointer=drawedLines.segment;
	int rectTop=rect.top;
	
	
	if (caret->segment<drawedLines.segment)
	{	
		if (caret->lineLocation< drawedLines.lineBegin)
			return;
	}

	while (rectTop<rect.bottom) //Находим строку, которая рисуется самой последней на новом экране
	{
		if (inSegmentLine==caret->lineLocation && segmentPointer==caret->segment)
		{
			SetCaretPos(caret->x, rectTop);
			if (!_caretIsShown)
			{
				_caretIsShown=true;
				ShowCaret(hWnd);
				return;
			}
		}
		rectTop+=segments[segmentPointer].linesHeight[inSegmentLine];
		inSegmentLine++;

		if (!TO_CheckSegmentPointers(segments,&segmentPointer, &inSegmentLine)) {return;}
	}
}

//Calculating coordinates of string, needed to draw, ScrollDC Y data and new insegment and segment pointers 
int _GF_CalcScrollDistanceAndBorder(SEGMENT* segments, int* borderTop, int scrolledLines, int* inSegmentLine, int* segmentPointer)
{
	int tempInSegmentLine=*inSegmentLine;
	int tempSegmentPointer=*segmentPointer;
	int scrollDistance=0;


	//If scrolls down - need to draw only first strings, inSegment and segment pointers didn`t changed
	if(scrolledLines<0)
	{
		while (scrolledLines<0)
		{
			scrollDistance+=segments[tempSegmentPointer].linesHeight[tempInSegmentLine];
			scrolledLines++;
			tempInSegmentLine++;
			if (!TO_CheckSegmentPointers(segments, &tempSegmentPointer, &tempInSegmentLine)) {return scrollDistance;}
		}
	}
	else //If scrolls up - need to draw bottom strings -> new inSegment and segment pointers, new border
	{
		//Calculating hided strings, step backword
		tempInSegmentLine--;
		if (!TO_CheckSegmentPointers(segments, &tempSegmentPointer, &tempInSegmentLine)) {return scrollDistance;}

		while (scrolledLines>0)
		{
			scrollDistance-=segments[tempSegmentPointer].linesHeight[tempInSegmentLine];
			scrolledLines--;
			tempInSegmentLine--;

			if (!TO_CheckSegmentPointers(segments, &tempSegmentPointer, &tempInSegmentLine)) {break;}
		}

		tempInSegmentLine = *inSegmentLine;
		tempSegmentPointer = *segmentPointer;

		int lastDrawedHeight=rect.top;
		
		//Scroll array to calc position of new srting
		while (lastDrawedHeight < rect.bottom + scrollDistance)
		{
			if (lastDrawedHeight < rect.bottom + scrollDistance)
			{
				*segmentPointer = tempSegmentPointer;
				*inSegmentLine = tempInSegmentLine;
				*borderTop = lastDrawedHeight;
				drawedLines.linesDrawed++;
			}
			lastDrawedHeight+=segments[tempSegmentPointer].linesHeight[tempInSegmentLine];
			tempInSegmentLine++;

			if (!TO_CheckSegmentPointers(segments, &tempSegmentPointer, &tempInSegmentLine)) {return scrollDistance;}

		}
	}

	return scrollDistance;
}

void _GF_DrawText(SEGMENT* segments, HDC hdc, int segmentPointer, int inSegmentLine, int scrolledLines, int borderTop)
{
	int arraySize=0;
	HFONT* fonts;


	fonts=FS_GetFonts(&arraySize);
	SelectObject(hdc, fonts[0]);

	while (scrolledLines>0 && borderTop<rect.bottom)
	{
		ExtTextOut(hdc, rect.left, borderTop, 0, NULL, segments[segmentPointer].text+(segments[segmentPointer].lineEnds[inSegmentLine]),
			segments[segmentPointer].linesLength[inSegmentLine], NULL);
		
		borderTop+=segments[segmentPointer].linesHeight[inSegmentLine];
		inSegmentLine++;
		scrolledLines--;
		drawedLines.linesDrawed++;

		if (!TO_CheckSegmentPointers(segments,&segmentPointer, &inSegmentLine)) {return;}
	}
}

void _GF_LocateTextCoords(SEGMENT* segments, int newLine, int* segmentPointer, int* inSegmentLine)
{
	drawedLines.totalCount = newLine;
	while (newLine>0)
	{
		(*inSegmentLine)++;
		newLine--;
		if (!TO_CheckSegmentPointers(segments,segmentPointer, inSegmentLine)) 
		{	
			drawedLines.totalCount-=newLine;
			(*inSegmentLine)--;
			break;
		}
	}
	drawedLines.segment = *segmentPointer;
	drawedLines.lineBegin = *inSegmentLine;
	drawedLines.linesDrawed = 0;
}