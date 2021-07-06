#include "stdafx.h"
#include "GraphicsFunctions.h"
#include "TextOperations.h"

bool cursorHighLighted=false;
bool carretIsShown=false; 

GFDRAWEDLINES drawedLines = {0,0,0,0};
HWND hWnd=NULL;
RECT rect;

void GF_Init(HWND hwnd, RECT rectangle)
{
	rect=rectangle;
	hWnd=hwnd;
	CreateCaret(hWnd, (HBITMAP) NULL, 2,16);
}

void GF_SetCursorPos(SEGMENT* segments, TOCURSORPOS* caret, int segmentsCount)
{
	if (caret->segment<drawedLines.segment)
	{	
		if (caret->lineLocation< drawedLines.lineBegin)
			return;
	}
	int inSegmentLine=drawedLines.lineBegin;
	int segmentPointer=drawedLines.segment;
	int rectTop=rect.top;
	while (rectTop<rect.bottom && segmentPointer<segmentsCount) //Находим строку, которая рисуется самой последней на новом экране
	{
		if (inSegmentLine==caret->lineLocation && segmentPointer==caret->segment)
		{
			SetCaretPos(caret->x, rectTop);
			if (!carretIsShown)
			{
				carretIsShown=true;
				ShowCaret(hWnd);
				return;
			}
		}
		rectTop+=segments[segmentPointer].linesHeight[inSegmentLine];
		inSegmentLine++;
		if (inSegmentLine>=segments[segmentPointer].linesCounter)
		{
			inSegmentLine=0;
			segmentPointer++;
		}
	}
}

//Calculating coordinates of string, needed to draw, ScrollDC Y data and new insegment and segment pointers 
int GF_CalcScrollDistanceAndBorder(SEGMENT* segments, int* borderTop, int segmentsCount, int scrolledLines, int* inSegmentLine, int* segmentPointer)
{
	int tempInSegmentLine=*inSegmentLine;
	int tempSegmentPointer=*segmentPointer;
	int scrollDistance=0;
	*borderTop=rect.top;

	//If scrolls down - need to draw only first strings, inSegment and segment pointers didn`t changed
	if(scrolledLines<0)
	{
		while (scrolledLines<0)
		{
			scrollDistance+=segments[tempSegmentPointer].linesHeight[tempInSegmentLine];
			scrolledLines++;
			tempInSegmentLine++;
			if (tempInSegmentLine>=segments[tempSegmentPointer].linesCounter)
			{
				tempInSegmentLine=0;
				tempSegmentPointer++;
				if (tempSegmentPointer>=segmentsCount)
					return scrollDistance;
			}
		}
	}
	else //If scrolls up - need to draw bottom strings -> new inSegment and segment pointers, new border
	{
		//Calculating hided strings, step backword
		tempInSegmentLine--;
		if (tempInSegmentLine<0)
		{
			tempSegmentPointer--;
			if(tempSegmentPointer<0)
				return scrollDistance;
			tempInSegmentLine=segments[tempSegmentPointer].linesCounter-1;
		}

		while (scrolledLines>0)
		{
			scrollDistance-=segments[tempSegmentPointer].linesHeight[tempInSegmentLine];
			scrolledLines--;
			tempInSegmentLine--;
			if (tempInSegmentLine<0)
			{
				tempSegmentPointer--;
				if(tempSegmentPointer<0)
					break;
				tempInSegmentLine=segments[tempSegmentPointer].linesCounter-1;
			}
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
			}
			lastDrawedHeight+=segments[tempSegmentPointer].linesHeight[tempInSegmentLine];
			tempInSegmentLine++;
			if (tempInSegmentLine>=segments[tempSegmentPointer].linesCounter)
			{
				tempInSegmentLine=0;
				tempSegmentPointer++;
				if (tempSegmentPointer>=segmentsCount)
					return scrollDistance;
			}
		}
	}

	return scrollDistance;
}

void GF_DrawTextByLine(SEGMENT* segments, TOCURSORPOS* carrage,int segmentsCount, int newLine, int prevLine)
{
	int scrolledLines=newLine-prevLine;
	if (scrolledLines==0)
		return;

	int currentLine=0;
	int inSegmentLine=0;
	int segmentPointer=0;

	while (currentLine<newLine)
	{
		inSegmentLine++;
		currentLine++;
		if (inSegmentLine>=segments[segmentPointer].linesCounter)
		{
			inSegmentLine=0;
			segmentPointer++;
		}
	}

	drawedLines.segment = segmentPointer;
	drawedLines.lineBegin = inSegmentLine;
	drawedLines.lineCount = 0;
	drawedLines.totalCount = currentLine;

	int borderTop=0;
	int scrollDistance = GF_CalcScrollDistanceAndBorder(segments, &borderTop, segmentsCount, scrolledLines, &inSegmentLine, &segmentPointer);
	
	if (scrolledLines>0)
		scrolledLines=abs(scrolledLines)+1;
	else
		scrolledLines=abs(scrolledLines);

	RECT fillingRect = rect;
	fillingRect.right+=2;
	
	if (carretIsShown)
	{
		carretIsShown=false;
		HideCaret(hWnd);
	}

	HDC hdc = GetDC(hWnd);

	if (scrollDistance>rect.bottom-rect.top)
	{
		inSegmentLine=drawedLines.lineBegin;
		segmentPointer=drawedLines.segment;
		borderTop=rect.top;
		scrolledLines=500;
	}
	else
	{	
		//RECT scrolledRect = rect;
		if (scrollDistance<0)
		{
			//scrolledRect.top-=scrollDistance;
			BitBlt(hdc,rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top+scrollDistance,
				hdc, rect.left, rect.top-scrollDistance,SRCCOPY);
		}
		else
		{
			//scrolledRect.bottom-=scrollDistance;
			BitBlt(hdc,rect.left, rect.top+scrollDistance, rect.right-rect.left, rect.bottom-scrollDistance-rect.top,
				hdc, rect.left, rect.top,SRCCOPY);
		}
		//ScrollDC(hdc, 0, scrollDistance, &scrolledRect,&rect, NULL,NULL);
	}

	int rectTop=rect.top;
	int arraySize=0;
	HFONT* fonts=TO_GetFonts(&arraySize);
	SelectObject(hdc, fonts[0]);
	
	fillingRect.bottom=borderTop;
	while (scrolledLines>0 && fillingRect.top<rect.bottom)
	{
		fillingRect.top=fillingRect.bottom;
		fillingRect.bottom=fillingRect.top+segments[segmentPointer].linesHeight[inSegmentLine];
		int lengthText=segments[segmentPointer].linesLength[inSegmentLine];
		
		ExtTextOut(hdc,3,fillingRect.top,ETO_OPAQUE,&fillingRect,segments[segmentPointer].text+(segments[segmentPointer].lineEnds[inSegmentLine]),lengthText,NULL);

		inSegmentLine++;
		scrolledLines--;
		
		if (inSegmentLine>=segments[segmentPointer].linesCounter)
		{
			inSegmentLine=0;
			segmentPointer++;
			if (segmentPointer>=segmentsCount)
			{
				fillingRect.top=fillingRect.bottom;
				fillingRect.bottom=rect.bottom;
				ExtTextOut(hdc,3,fillingRect.top,ETO_OPAQUE,&fillingRect,L" ",1,NULL);
				break;
			}
		}
	}
	
	ReleaseDC(hWnd, hdc);
	GF_SetCursorPos(segments, carrage, segmentsCount);
}

void GF_DrawTextAll(SEGMENT* segments, TOCURSORPOS* carrage, int segmentsCount, int newLine, int prevLine)
{
	PAINTSTRUCT ps;
    HDC hdc;

	if (carretIsShown)
	{
		carretIsShown=false;
		HideCaret(hWnd);
	}

	RECT fillingRect = rect;
	fillingRect.right+=2;
	int currentLine=0;
	int inSegmentLine=0;
	int segmentPointer=0;
	
	while (currentLine<newLine)
	{
		inSegmentLine++;
		currentLine++;
		if (inSegmentLine>=segments[segmentPointer].linesCounter)
		{
			inSegmentLine=0;
			segmentPointer++;
		}
	}
	
	drawedLines.segment = segmentPointer;
	drawedLines.lineBegin = inSegmentLine;
	drawedLines.lineCount = 0;

	hdc = BeginPaint (hWnd, &ps); 
	
	int arraySize=0;
	HFONT* fonts=TO_GetFonts(&arraySize);
	SelectObject(hdc, fonts[0]);

	int lastHeight=rect.top;
	
	fillingRect.bottom=fillingRect.top+FONTHEIGHT;

	while (lastHeight<rect.bottom)
	{
		
		if (segmentPointer < segmentsCount)
		{
			int lengthText=segments[segmentPointer].linesLength[inSegmentLine];
			ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,segments[segmentPointer].text+(segments[segmentPointer].lineEnds[inSegmentLine]),lengthText,NULL);
			inSegmentLine++;
			if (inSegmentLine>=segments[segmentPointer].linesCounter)
			{
				inSegmentLine=0;
				segmentPointer++;
			}
			lastHeight+=FONTHEIGHT;
			fillingRect.top=fillingRect.bottom;
			fillingRect.bottom=fillingRect.top+FONTHEIGHT;
		}
		else
			ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,L" ",1,NULL);
		newLine++;
	}
	
	EndPaint (hWnd, &ps); 
	GF_SetCursorPos(segments, carrage, segmentsCount);
}
