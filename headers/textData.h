#pragma once

#include <stdlib.h>


typedef struct
{
	int linesCounter;
	int length;
	int* lineEnds;
	int* spacesPointer;
	int spacesCounter;
	int*  linesLength;
	wchar_t* text;
	int textArraySize;

}SEGMENT;


