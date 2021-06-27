// ProjectWinCE.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "ProjectWinCE.h"

int carrage=0;

//======================================================================
// Program entry point
//
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc;
    HWND hWnd;
    MSG msg;
	HICON hIcon;
	
	hIcon=LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	hInst = hInstance;
    // Register application main window class.
    wc.style = 0;                             // Window style
    wc.lpfnWndProc = MainWndProc;             // Callback function
    wc.cbClsExtra = 0;                        // Extra class data
    wc.cbWndExtra = 0;                        // Extra window data
    wc.hInstance = hInstance;                 // Owner handle
    wc.hIcon = NULL,                          // Application icon
		wc.hCursor = 0;// Default cursor
    wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wc.lpszMenuName =  NULL;                  // Menu name
    wc.lpszClassName = TEXT("MyClass");       // Window class name
	
    if (RegisterClass (&wc) == 0) return -1;
	
    // Create main window.
    hWnd = CreateWindowEx(WS_EX_NODRAG,       // Ex style flags
		TEXT("MyClass"),    // Window class
		TEXT("WinCE Project"),      // Window title
		// Style flags
		WS_VISIBLE,
		CW_USEDEFAULT,      // x position
		CW_USEDEFAULT,      // y position
		CW_USEDEFAULT,      // Initial width
		CW_USEDEFAULT,      // Initial height
		NULL,               // Parent
		NULL,               // Menu, must be null
		hInstance,          // Application instance
		NULL);              // Pointer to create parameters
    if (!IsWindow (hWnd)) return -2;  // Fail code if not created.
	
	SendMessage(hWnd, WM_SETICON, FALSE, (LPARAM)hIcon);
	hwndMW=hWnd;

	RECT rect;
	GetClientRect (hWnd, &rect);
	
	hwndSB = CreateWindowEx( 
		0,                      // no extended styles 
		L"SCROLLBAR",           // scroll bar control class 
		(PTSTR) NULL,           // no window text 
		WS_CHILD | WS_VISIBLE   // window styles  
		| SBS_VERT,         // horizontal scroll bar style 
		rect.right-SBWIDTH,              // horizontal position 
		TOPHEADERDEADZONE,				// vertical position 
		rect.right,             // width of the scroll bar 
		rect.bottom-TOPHEADERDEADZONE,               // height of the scroll bar
		hWnd,					// handle to main window 
		(HMENU) NULL,           // no menu 
		hInstance,              // instance owning this window 
		(PVOID) NULL            // pointer not needed 
        );
	
	segments = new SEGMENT[50];
	CreateFont();
	GetTextSegments(hWnd, segments, &segmentsCount);
	Setup(hwndSB);
	
    // Standard show and update calls
    ShowWindow (hWnd, nCmdShow);
    UpdateWindow (hWnd);
	
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

    // Application message loop
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
    // Instance cleanup
    return msg.wParam;
}

//======================================================================
// MainWndProc - Callback function for application window
//
LRESULT CALLBACK MainWndProc (HWND hWnd, UINT wMsg, WPARAM wParam, 
                              LPARAM lParam) {
    RECT rect;
    HDC hdc; 
	SCROLLINFO si;
	bool reRender=false;
	int wmId, wmEvent;
    switch (wMsg) 
	{
	case WM_CHAR:
		{
			wchar_t charCode =wParam;
			hdc=GetDC(hWnd);
			GetClientRect(hWnd, &rect);
			rect.top=TOPHEADERDEADZONE;
			rect.left=3;
			rect.right-=SBWIDTH+3;
			switch (charCode)
			{
			case '\b':
				
				break;
			case L'\r':
				InsertSymbol(segments,0,9,charCode,hdc, rect);
				break;
			default:
				InsertSymbol(segments,0,carrage,L'L',hdc, rect);
				carrage++;
				break;
			}
			
			int b=0; 
			ReleaseDC(hWnd,hdc);
			InvalidateRect(hWnd,NULL,TRUE);
			return 0;
		}
	case WM_KEYDOWN:
		{
			
			int a=1;
			return 0;
		}
	case WM_COMMAND:
		{
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
			case ID_HELP_ABOUT:
				{
					DialogBox(hInst, (LPCTSTR)IDD_DIALOG1, hWnd, (DLGPROC)About);
					return 0;
				}
			case ID_FILE_EXIT:
				{
					DestroyWindow(hWnd);
					return 0;
				}
			case ID_TOOLS_DEBUG_SPEEDTEST2:
				{
					SpeedTest(hWnd, 2, segments, segmentsCount);
					return 0;
				}
			case ID_TOOLS_DEBUG_SPEEDTEST10:
				{
					SpeedTest(hWnd, 10, segments, segmentsCount);
					return 0;
				}
			case ID_FILE_OPEN:
				{
					TCHAR szFileName[MAX_PATH];
					int length = GetFileName (hWnd, szFileName, MAX_PATH);
					return 0;
				}
			case ID_TOOLS_DEBUG_MEMORYOPERATIONS:
				{
					MemOperationsTest();
					return 0;
				}
			case ID_TOOLS_DEBUG_STRINGCHECKLENGTHTEST:
				{
					StringSizeTest(hWnd);
					return 0;
				}				
			}
			
		}
	case WM_CREATE:
		{
			hwndCB = CommandBar_Create(hInst, hWnd, 1);			
			BOOL success=CommandBar_InsertMenubar(hwndCB, hInst, IDR_MENU1 , 0);
			CommandBar_InsertComboBox(hwndCB, hInst, 100, CBS_DROPDOWNLIST | WS_VSCROLL, 0, 1);
			
			success=CommandBar_AddAdornments(hwndCB, 0, 0);

			return 0;
		}
	case WM_VSCROLL:
		{
			int sPos, sPosOrg;
			
			// Get scroll bar position.
			si.cbSize = sizeof (si);
			si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
			
			GetScrollInfo ((HWND)lParam, SB_CTL, &si);
			sPos = si.nPos;
			sPosOrg=si.nPos;
			// Act on the scroll code.
			switch (LOWORD (wParam)) {
			case SB_LINEUP:      // Also SB_LINELEFT
				sPos -= 1;
				reRender=true;		
				break;

			case SB_LINEDOWN:    // Also SB_LINERIGHT
				sPos += 1;
				reRender=true;
				break;
				
			case SB_PAGEUP:      // Also SB_PAGELEFT
				sPos -= si.nPage;
				reRender=true;
				break;
				
			case SB_PAGEDOWN:    // Also SB_PAGERIGHT
				sPos += si.nPage;
				reRender=true;
				//si.nMax=100;
				break;
				
			case SB_THUMBPOSITION:
				sPos = HIWORD (wParam);
				reRender=true;
				break;
				
			case SB_THUMBTRACK:
				sPos = HIWORD (wParam);
				reRender=true;
				break;	
			}
			// Check range.
			if (sPos < 0)
				sPos = 0;
			if (sPos >= si.nMax-si.nPage+1)
				sPos = si.nMax-si.nPage+1;
			
			// Update scroll bar position.
			si.cbSize = sizeof (si);
			si.nPos = sPos;
		
			si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
			
			SetScrollInfo ((HWND)lParam, SB_CTL, &si, TRUE);
			if (reRender==true)
			{
				hdc = GetDC(hWnd);
				GetClientRect(hWnd, &rect);
				rect.top=TOPHEADERDEADZONE;
				rect.left=3;
				rect.right-=SBWIDTH;
				DrawTextByLineChoosenHDC(segments, segmentsCount, si.nPos, sPosOrg , 0, si.nPage, hdc, rect);
				ReleaseDC(hWnd,hdc);
			}
			return 0;
		}
	case WM_PAINT:
		{
			si.cbSize = sizeof (si);
			si.fMask = SIF_POS;			
			GetScrollInfo ((HWND)hwndSB, SB_CTL, &si);
			oldPos=si.nPos;
			
			GetClientRect(hWnd, &rect);
			
			rect.top=TOPHEADERDEADZONE;
			RECT fillingRect=rect;
			
			fillingRect.bottom=FONTHEIGHT+TOPHEADERDEADZONE;
			fillingRect.top=TOPHEADERDEADZONE;
			fillingRect.left=3;
			rect.right-=SBWIDTH;
			
			DrawTextByLine(segments, segmentsCount, si.nPos, oldPos , 1, hWnd,rect);
			return 0;
		}
	case WM_DESTROY:
		{
			PostQuitMessage (0);
			break;
		}
    }
    return DefWindowProc (hWnd, wMsg, wParam, lParam);
}


void Setup(HWND lParam)
{	
	RECT rect;
	int counterTotal=0;
	for (int j=0; j<segmentsCount; j++)
	{
		counterTotal+=segments[j].linesCounter;
	}
	GetClientRect(lParam, &rect);
	rect.top=TOPHEADERDEADZONE;
	SCROLLINFO si;
	
	// Get scroll bar position.
	si.cbSize = sizeof (si);
	si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
	
	GetScrollInfo ((HWND)lParam, SB_CTL, &si);
	
	// Update scroll bar position.
	si.cbSize = sizeof (si);
	
	si.nPage=0;
	while (rect.top<rect.bottom-FONTHEIGHT)
	{
		si.nPage++;
		rect.top+=FONTHEIGHT;
	}
	si.nMax=counterTotal-1;
	
	si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
	
	SetScrollInfo ((HWND)lParam, SB_CTL, &si, TRUE);
}


// Mesage handler for the About box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

int GetFileName (HWND hWnd, LPTSTR szFileName, int nMax) 
{
	OPENFILENAME of;
	const LPTSTR pszOpenFilter = TEXT ("All Documents (*.*)\0*.*\0\0");

	szFileName[0] = '\0';				  // Initial filename
	memset (&of, 0, sizeof (of));		  // Initial file open structure

	of.lStructSize = sizeof (of);
	of.hwndOwner = hWnd;
	of.lpstrFile = szFileName;
	of.nMaxFile = nMax;
	of.lpstrFilter = pszOpenFilter;
	of.Flags = 0;

	if (GetOpenFileName (&of))
		return lstrlen (szFileName);
	else
		return 0;
}

