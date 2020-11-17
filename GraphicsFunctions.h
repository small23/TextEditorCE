#pragma once

#include <windows.h>
#include <stdlib.h>
#include "GlobalDefinitions.h"
#include "textData.h"
//#include "FontsStorage.h"

void DrawTextByLine(SEGMENT* segments,int segmentCount, int a, int b , int c, HWND hWnd, RECT rect);
void DrawTextByLineChoosenHDC(SEGMENT* segments,int segmentCount, int newLinePointer, int oldLinePointer , int redrawAll, int pageSize, HDC hdc, RECT rect);
