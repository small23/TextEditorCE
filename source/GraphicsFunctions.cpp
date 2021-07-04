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

void GF_DrawTextByLine(SEGMENT* segments, int segmentsCount, int newLine, int prevLine , int reserverd, int pageSize, HWND hWnd, RECT rect)
{
	HDC hdc = GetDC(hWnd);
	
	int currentLine=0;
	int inSegmentLine=0;
	int segmentPointer=0;
	//HideCaret(hwndMW);
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
	
	RECT fillingRect = rect;
	fillingRect.right+=2;
	
	int lastHeight=rect.top;
	//int lastCursorOnText=0;
	
	//SIZE textSize;
	
	int scrolledLines=newLine-prevLine;
	int rectTop=rect.top;
	int counter=0;
	int arraySize=0;
	HFONT* fonts=TO_GetFonts(&arraySize);
	SelectObject(hdc, fonts[0]);
	
	drawedLines.segment = segmentPointer;
	drawedLines.lineBegin = inSegmentLine;
	drawedLines.lineCount = 0;
				
				
	if (abs(scrolledLines)<pageSize)
	{	
		
		if (scrolledLines>0) //GoDown - render bottom strings
		{
			ScrollDC(hdc, 0, -scrolledLines*FONTHEIGHT, &rect,&rect, NULL,NULL);
			
			while (rectTop<rect.bottom-FONTHEIGHT)
			{
				rectTop+=FONTHEIGHT;
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
			
			

			while (scrolledLines>0)
			{
				rectTop-=FONTHEIGHT;
				scrolledLines--;
				inSegmentLine--;
				currentLine--;
				if (inSegmentLine<0)
				{
					segmentPointer--;
					inSegmentLine=segments[segmentPointer].linesCounter-1;
					
				}
			}
		
			lastHeight=rectTop;
			fillingRect.top=lastHeight;
			fillingRect.bottom=fillingRect.top+FONTHEIGHT;
			while (lastHeight<rect.bottom)
			{
				if (segmentPointer < segmentsCount)
				{
					int lengthText=segments[segmentPointer].linesLength[inSegmentLine];
					if (lengthText)
						ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,segments[segmentPointer].text+(segments[segmentPointer].lineEnds[inSegmentLine]),lengthText,NULL);
					else
						ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,L" ",1,NULL);
					inSegmentLine++;
					if (inSegmentLine>=segments[segmentPointer].linesCounter)
					{
						inSegmentLine=0;
						
					}
					drawedLines.lineCount++;
				}
				else
					ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,L" ",1,NULL);
				fillingRect.top=fillingRect.bottom;
				fillingRect.bottom=fillingRect.top+FONTHEIGHT;
				lastHeight+=FONTHEIGHT;
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
	tagPOINT coords;
	GetCaretPos(&coords);
	coords.x+=5;
	SetCaretPos(coords.x, coords.y);
	//ShowCaret(hWnd);
}

void GF_DrawTextAll(SEGMENT* segments, int segmentsCount, int newLine, int prevLine , HWND hWnd, RECT rect)
{
	PAINTSTRUCT ps;
    HDC hdc;

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
			lastHeight+=FONTHEIGHT;
			fillingRect.top=fillingRect.bottom;
			fillingRect.bottom=fillingRect.top+FONTHEIGHT;
		}
		else
			ExtTextOut(hdc,3,lastHeight,ETO_OPAQUE,&fillingRect,L" ",1,NULL);
	}
	
	EndPaint (hWnd, &ps); 
	
}
