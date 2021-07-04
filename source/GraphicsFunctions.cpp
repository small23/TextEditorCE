#include "stdafx.h"
#include "GraphicsFunctions.h"
#include "TextOperations.h"

bool cursorHighLighted=false;


GFDRAWEDLINES drawedLines = {0,0,0};

void GF_DrawCursor(HDC hdc)
{
	
}

GFDRAWEDLINES* GF_GetDrawedLines()
{
	return &drawedLines;
}

void GF_DrawTextByLine(SEGMENT* segments, TOCURSORPOS* carrage,int segmentsCount, int newLine, int prevLine , int reserverd, int pageSize, HWND hWnd, RECT rect)
{
	int scrolledLines=newLine-prevLine;
	if (scrolledLines==0)
		return;

	int currentLine=0;
	int inSegmentLine=0;
	int segmentPointer=0;
	int scrollDistance=0;

	while (currentLine<newLine)
	{
		inSegmentLine++;
		currentLine++;
		if (inSegmentLine>=segments[segmentPointer].linesCounter)
		{
			inSegmentLine=0;
			segmentPointer++;
		}
		if (currentLine>prevLine)
			scrollDistance+=segments[segmentPointer].linesHeight[inSegmentLine];
	}
	
	RECT fillingRect = rect;
	fillingRect.right+=2;
	
	int lastHeight=rect.top;
	
	HideCaret(hWnd);

	HDC hdc = GetDC(hWnd);
	
	int rectTop=rect.top;
	int counter=0;
	int arraySize=0;
	int lastDrawedLineInSegment = 0;
	int lastDrawedSegment = 0;
	HFONT* fonts=TO_GetFonts(&arraySize);
	SelectObject(hdc, fonts[0]);
	
	drawedLines.segment = segmentPointer;
	drawedLines.lineBegin = inSegmentLine;
	drawedLines.lineCount = 0;
			
	if (abs(scrolledLines)<pageSize)
	{	
		
		if (scrolledLines>0) //GoDown - render bottom strings
		{
			ScrollDC(hdc, 0, -scrollDistance, &rect,&rect, NULL,NULL); 
			int lastDrawedHeight=0;
			
			while (rectTop<rect.bottom && segmentPointer<segmentsCount) //Находим строку, которая рисуется самой последней на новом экране
			{
				if (rectTop + scrollDistance < rect.bottom)
				{
					lastDrawedSegment = segmentPointer;
					lastDrawedLineInSegment=inSegmentLine;
					lastDrawedHeight = rectTop;
				}
				rectTop+=segments[segmentPointer].linesHeight[inSegmentLine];
				counter++;
				inSegmentLine++;
				currentLine++;
				if (inSegmentLine>=segments[segmentPointer].linesCounter)
				{
					inSegmentLine=0;
					segmentPointer++;
				}
				drawedLines.lineCount++;
			}
			segmentPointer = lastDrawedSegment;
			inSegmentLine = lastDrawedLineInSegment;
			lastHeight = lastDrawedHeight;
			fillingRect.top=lastHeight;
			fillingRect.bottom=fillingRect.top+segments[segmentPointer].linesHeight[inSegmentLine];
			while (segmentPointer<segmentsCount && lastHeight<rect.bottom)
			{
					int lengthText=segments[segmentPointer].linesLength[inSegmentLine];
					if (lengthText)
						ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,segments[segmentPointer].text+(segments[segmentPointer].lineEnds[inSegmentLine]),lengthText,NULL);
					else
						ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,L" ",1,NULL);
					
					fillingRect.top=fillingRect.bottom;
					fillingRect.bottom=fillingRect.top+segments[segmentPointer].linesHeight[inSegmentLine];
					lastHeight+=segments[segmentPointer].linesHeight[inSegmentLine];
					inSegmentLine++;
					if (inSegmentLine>=segments[segmentPointer].linesCounter)
					{
						inSegmentLine=0;
						segmentPointer++;
					}
					drawedLines.lineCount++;
			}
			if (!lastHeight<rect.bottom)
			{
				fillingRect.bottom=rect.bottom;
				ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,L" ",1,NULL);
			}
		}
		else if (scrolledLines<0) //GoUp - render uppper strings
		{
			fillingRect.bottom=fillingRect.top+FONTHEIGHT;
			ScrollDC(hdc, 0, -scrolledLines*FONTHEIGHT, &rect,&rect, NULL,NULL);
			while (scrolledLines<0)
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
				scrolledLines++;
			}
			
		}
		else return;
	}
	else
	{
		fillingRect.top=lastHeight;
		fillingRect.bottom=fillingRect.top+segments[segmentPointer].linesHeight[inSegmentLine];
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
			}
			else
				ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,L" ",1,NULL);
			lastHeight+=FONTHEIGHT;
			fillingRect.top=fillingRect.bottom;
			fillingRect.bottom=fillingRect.top+FONTHEIGHT;
		}
	}
	ReleaseDC(hWnd, hdc);
}

void GF_DrawTextAll(SEGMENT* segments, TOCURSORPOS* carrage, int segmentsCount, int newLine, int prevLine , HWND hWnd, RECT rect)
{
	PAINTSTRUCT ps;
    HDC hdc;
	HideCaret(hWnd);
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
	int lastCursorOnText=0;
	
	fillingRect.bottom=fillingRect.top+FONTHEIGHT;

	//SIZE textSize;
	
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

			if (newLine==carrage->lineLocation)
				SetCaretPos(carrage->x, lastHeight);
			
			lastHeight+=FONTHEIGHT;
			fillingRect.top=fillingRect.bottom;
			fillingRect.bottom=fillingRect.top+FONTHEIGHT;
		}
		else
			ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,L" ",1,NULL);
		newLine++;
	}
	
	EndPaint (hWnd, &ps); 
	
	ShowCaret(hWnd);
}
