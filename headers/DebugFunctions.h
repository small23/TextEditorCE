#include <Afxwin.h>
#include <stdlib.h>
#include <Commdlg.h>
#include <Commctrl.h>
#include "GraphicsFunctions.h"
#include "GlobalDefinitions.h"

void DF_SpeedTest(HWND hWnd, TOCURSORPOS* carrage, int lineScrolls, SEGMENT* segments, int segmentsCount);
void DF_MemOperationsTest();
void DF_StringSizeTest(HWND hWnd);
__int64 DF_Milliseconds_now();