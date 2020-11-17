#include <Afxwin.h>
#include <stdlib.h>
#include <Commdlg.h>
#include <Commctrl.h>
#include "GraphicsFunctions.h"
#include "GlobalDefinitions.h"

void SpeedTest(HWND hWnd, int lineScrolls, SEGMENT* segments, int segmentsCount);
void MemOperationsTest();
void StringSizeTest(HWND hWnd);
__int64 Milliseconds_now();