#include "stdafx.h"
#include "GraphicsFunctions.h"
#include "TextOperations.h"

bool cursorHighLighted=false;
bool carretIsShown=false; 

GFDRAWEDLINES drawedLines = {0,0,0};
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
		return;
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
int GF_CalcScrollDistance(SEGMENT* segments, int segmentsCount, int scrolledLines, int inSegmentLine, int segmentPointer)
{
	int scrollDistance=0;
	if(scrolledLines<0)
	{
		while (scrolledLines<0)
		{
			scrollDistance+=segments[segmentPointer].linesHeight[inSegmentLine];
			scrolledLines++;
			inSegmentLine++;
			if (inSegmentLine>=segments[segmentPointer].linesCounter)
			{
				inSegmentLine=0;
				segmentPointer++;
				if (segmentPointer>=segmentsCount)
					return scrollDistance;
			}
		}
	}
	else
	{
		if (inSegmentLine<0)
		{
			inSegmentLine=segments[segmentPointer].linesCounter-1;
			segmentPointer--;
			if(segmentPointer<0)
				return scrollDistance;
		}
		while (scrolledLines>0)
		{
			scrollDistance-=segments[segmentPointer].linesHeight[inSegmentLine];
			scrolledLines--;
			inSegmentLine--;
			if (inSegmentLine<0)
			{
				inSegmentLine=segments[segmentPointer].linesCounter-1;
				segmentPointer--;
				if(segmentPointer<0)
					return scrollDistance;
			}
		}
	}

	return scrollDistance;
}

void GF_DrawTextByLine(SEGMENT* segments, TOCURSORPOS* carrage,int segmentsCount, int newLine, int prevLine , int reserverd, int pageSize)
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
	
	int scrollDistance = GF_CalcScrollDistance(segments, segmentsCount, scrolledLines, inSegmentLine, segmentPointer);

	RECT fillingRect = rect;
	fillingRect.right+=2;
	
	int lastHeight=rect.top;
	
	if (carretIsShown)
	{
		carretIsShown=false;
		HideCaret(hWnd);
	}

	HDC hdc = GetWindowDC(hWnd);
	
	int rectTop=rect.top;
	int arraySize=0;
	HFONT* fonts=TO_GetFonts(&arraySize);
	SelectObject(hdc, fonts[0]);
	
	drawedLines.segment = segmentPointer;
	drawedLines.lineBegin = inSegmentLine;
	drawedLines.lineCount = 0;
	
	//RECT scrolledRect = rect;
	//RECT clippedRect = rect;
	
	if (scrollDistance < rect.bottom-rect.top)
	{	
		
		if (scrolledLines>0) //GoDown - render bottom strings
		{
			//scrolledRect.top+=scrollDistance;
			//clippedRect.bottom-=scrollDistance;
			//ScrollDC(hdc, 0, -scrollDistance, &scrolledRect,&clippedRect, NULL,NULL); 
			ScrollDC(hdc, 0, scrollDistance, &rect,&rect, NULL,NULL);
			int lastDrawedHeight=0;
			int lastDrawedLineInSegment = 0;
	        int lastDrawedSegment = 0;

			while (rectTop<rect.bottom && segmentPointer<segmentsCount) //Находим строку, которая рисуется самой последней на новом экране
			{
				if (rectTop - scrollDistance < rect.bottom)
				{
					lastDrawedSegment = segmentPointer;
					lastDrawedLineInSegment=inSegmentLine;
					lastDrawedHeight = rectTop;
				}
				rectTop+=segments[segmentPointer].linesHeight[inSegmentLine];
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
			//scrolledRect.bottom-=-scrolledLines*FONTHEIGHT;
			//clippedRect.top+=scrollDistance;
			//ScrollDC(hdc, 0, -scrolledLines*FONTHEIGHT, &scrolledRect,&clippedRect, NULL,NULL);
			ScrollDC(hdc, 0, scrollDistance, &rect,&rect, NULL,NULL);
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
