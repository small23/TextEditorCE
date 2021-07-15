#pragma once

#include <stdlib.h>
#include "GlobalDefinitions.h"
#include "FontsStorage.h"

#define		TO_RESERVED		0x0001
#define		TO_HEADING_1	0x0002
#define     TO_HEADING_2	0x0004
#define		TO_HEADING_3	0x0006
#define		TO_TAB			0x0008
#define		TO_INDEX_1		0x0010
#define		TO_INDEX_2		0x0020
#define		TO_INDEX_3		0x0030  


/////////////////////////////////////////////////////
// Structs definitions
// TOCURSORPOS - contains information of the position 
// of the carriage on the screen and in the text
typedef struct
{ 
	int segment;		//segment number
	int position;		//position in segment - by text
	int lineLocation;	//position in segment - by drawed line
	int x;				//ofset 
	int height;			//height of the cursor
	int lastWidth;		//??? something
}TOCURSORPOS;


typedef struct 
{
	int reasonCode;
	int stopPoint;
	LOGFONT font;
}TOSTOPPOINTS;

typedef struct
{
	int  segmentMarks;		//Marks about segment
	int  linesCounter;		//Number of drawed lines
	int  length;			//Total length of segment (in char)
	int* lineEnds;			//Array with pointers to end lines in text
	int  lineEndsSize;		//Array size
	int  lineEndsCounter;	//Array last used cell
	TOSTOPPOINTS* renderStopPoints;	//Points for render stop (end of line or font change)
	int  renderStopSize;
	int  renderStopCounter;
	int* spacesPointer;		//Array that contains adres of spaces
	int	 spacesCounter;		
	int  spacesSize;
	int* linesLength;		//Array that contains length of lines
	int* linesHeight;		//Array thar contains height of lines
	int  linesSize;
	wchar_t* text;			//Text array
	int  textSize;
	LOGFONT* fonts;
}SEGMENT;


///////////////////////
//Main functions
void TO_GetTextSegments(SEGMENT* segments, HWND hWnd,  RECT rect);
void TO_RecheckSpacesAndLines(SEGMENT* segments, int i, HDC hdc,RECT rect);
void TO_CalcCarragePos(SEGMENT* segments,TOCURSORPOS* carrage,HDC hdc, RECT rect);
bool TO_CheckSegmentPointers(SEGMENT* segments,int* segmentPointer, int* inSegmentLine);
int TO_GetSegmentCount();
int TO_GetLinesTotal(SEGMENT* segments);
////////////////////////
//Text manipulations
void TO_InsertSymbol(SEGMENT* segments, TOCURSORPOS* carrage, wchar_t simbol, HDC hdc, RECT rect);
void TO_DeleteSymbol(SEGMENT* segments, TOCURSORPOS* carrage, HDC hdc, RECT rect);

////////////////////////
//Hotkeys controls
void TO_Arrows(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, WPARAM arrow);
void TO_HomeEnd(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, WPARAM arrow);

////////////////////////
//Internal functions
void _TO_ArrowUpDownPosCalc(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, int counter);