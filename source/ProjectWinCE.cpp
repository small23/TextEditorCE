// ProjectWinCE.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "ProjectWinCE.h"

//======================================================================
// Program entry point
//
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nCmdShow) 
	{
    MSG msg;
	
	if(InitInstance(hInstance, hPrevInstance, lpCmdLine, nCmdShow)!=0)
		return -1;
	
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
                              LPARAM lParam) 
	{
    switch (wMsg) 
		{
		case WM_CHAR:
			{
			return (CharHandler(hWnd, wMsg, wParam,lParam));
			}
		case WM_KEYDOWN:
			{
			return (KeydownHandler(hWnd, wMsg, wParam,lParam));
			}
		case WM_COMMAND:
			{
			return (CommandHandler(hWnd, wMsg, wParam,lParam));
			}
		case WM_CREATE:
			{
			
			return 0;
			}
		case WM_VSCROLL:
			{
			return (VsScrollHandler(hWnd, wMsg, wParam,lParam));
			}
		case WM_PAINT:
			{
			return (PaintHandler(hWnd, wMsg, wParam,lParam));
			}
		case WM_DESTROY:
			{
			PostQuitMessage (0);
			break;
			}
		}
    return DefWindowProc (hWnd, wMsg, wParam, lParam);
	}

LRESULT KeydownHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
	switch (wParam) 
		{
		case VK_HOME:       // Home 
			
			break; 
		case VK_END:        // End 
			
			break; 
		case VK_PRIOR:      // Page Up 
			
			break; 
		case VK_NEXT:       // Page Down 
			
			break; 
		case VK_LEFT:       // Left arrow 
			
			break; 
		case VK_RIGHT:      // Right arrow 
			
			break; 
		case VK_UP:         // Up arrow 
			
			break; 
		case VK_DOWN:       // Down arrow 
			
			break; 
		case VK_DELETE:     // Delete 
			
			break; 
		} 	
	return 0; 
	}

LRESULT CharHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
	HDC hdc; 
	RECT rect;
	wchar_t charCode =wParam;
	hdc=GetDC(hwndMW);
	GetClientRect(hwndMW, &rect);
	rect.top=TOPHEADERDEADZONE;
	rect.left=3;
	rect.right-=SBWIDTH+3;
	switch (charCode)
		{
		case '\b':
			TO_DeleteSymbol(segments,&carrage,hdc, rect);
			break;
		case '\t':
			
			break;
		case L'\r':
			//TO_InsertSymbol(segments,0,9,charCode,hdc, rect);
			break;
		case L'\n':
			//TO_InsertSymbol(segments,0,9,charCode,hdc, rect);
			break;
		default:
			TO_InsertSymbol(segments,&carrage,charCode,hdc, rect);
			break;
		}
	tagPOINT coords;
	GetCaretPos(&coords);
	int b=0; 
	coords.x+=5;
	SetCaretPos(coords.x, coords.y);
	ReleaseDC(hwndMW,hdc);
	InvalidateRect(hwndMW,NULL,TRUE);
	return 0;
	}

LRESULT CommandHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
	int wmId, wmEvent;
	
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
			DF_SpeedTest(hWnd, 2, segments, segmentsCount);
			return 0;
			}
		case ID_TOOLS_DEBUG_SPEEDTEST10:
			{
			DF_SpeedTest(hWnd, 10, segments, segmentsCount);
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
			DF_MemOperationsTest();
			return 0;
			}
		case ID_TOOLS_DEBUG_STRINGCHECKLENGTHTEST:
			{
			DF_StringSizeTest(hWnd);
			return 0;
			}
		case 211: //STD_FILEOPEN
			{
			return 0;
			}
		case 212: //STD_FILESAVE
			{
			return 0;
			}
		case 213: //STD_CUT
			{
			return 0;
			}
		case 214: //STD_COPY
			{
			return 0;
			}
		case 215: //STD_PASTE
			{
			return 0;
			}
		case 216: //STD_UNDO
			{
			return 0;
			}
		case 217: //VIEW_LIST
			{
			return 0;
			}
		case 218: //STD_PROPERTIES
			{
			return 0;
			}
		}
	return 0;
	}

LRESULT VsScrollHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
	HDC hdc; 
	SCROLLINFO si;
	RECT rect;
	bool reRender=false;
	
	int sPos, sPosOrg;
	HideCaret(hWnd);
	// Get scroll bar position.
	si.cbSize = sizeof(si);
	si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
	
	GetScrollInfo((HWND)lParam, SB_CTL, &si);
	sPos = si.nPos;
	sPosOrg = si.nPos;
	// Act on the scroll code.
	switch (LOWORD(wParam))
		{
		case SB_LINEUP: // Also SB_LINELEFT
			sPos -= 1;
			reRender = true;
			break;
			
		case SB_LINEDOWN: // Also SB_LINERIGHT
			sPos += 1;
			reRender = true;
			break;
			
		case SB_PAGEUP: // Also SB_PAGELEFT
			sPos -= si.nPage;
			reRender = true;
			break;
			
		case SB_PAGEDOWN: // Also SB_PAGERIGHT
			sPos += si.nPage;
			reRender = true;
			break;
			
		case SB_THUMBPOSITION:
			sPos = HIWORD(wParam);
			reRender = true;
			break;
			
		case SB_THUMBTRACK:
			sPos = HIWORD(wParam);
			reRender = true;
			break;
		}
	// Check range.
	if (sPos < 0)
		sPos = 0;
	if (sPos > si.nMax - si.nPage + 1)
		sPos = si.nMax - si.nPage + 1;
	
	// Update scroll bar position.
	si.cbSize = sizeof(si);
	si.nPos = sPos;
	
	si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
	
	SetScrollInfo((HWND)lParam, SB_CTL, &si, TRUE);
	if (reRender == true)
		{
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		rect.top = TOPHEADERDEADZONE;
		rect.left = 3;
		rect.right -= SBWIDTH;
		GF_DrawTextByLineChoosenHDC(segments, segmentsCount, si.nPos, sPosOrg, 0, si.nPage, hdc, rect);
		ReleaseDC(hWnd, hdc);
		}
	ShowCaret(hWnd);
	return 0;
	}

LRESULT PaintHandler(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
	SCROLLINFO si;
	RECT rect;
	
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
	
	GF_DrawTextByLine(segments, segmentsCount, si.nPos, oldPos , 1, hWnd,rect);
	return 0;
	}

int InitInstance(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPWSTR lpCmdLine, int nCmdShow)
	{
	WNDCLASS wc;
    HWND hWnd;
	HICON hIcon;
	INITCOMMONCONTROLSEX icex;
	RECT rect;
	
	hInst = hInstance;
    wc.style = 0;                             // Window style
    wc.lpfnWndProc = MainWndProc;             // Callback function
    wc.cbClsExtra = 0;                        // Extra class data
    wc.cbWndExtra = 0;                        // Extra window data
    wc.hInstance = hInstance;                 // Owner handle
    wc.hIcon = NULL,                          // Application icon
		wc.hCursor = 0;							  // Default cursor
    wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wc.lpszMenuName =  NULL;                  // Menu name
    wc.lpszClassName = TEXT("MyClass");       // Window class name
	
    if (RegisterClass (&wc) == 0) return -1;
	
	icex.dwSize = sizeof (INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_COOL_CLASSES;
    InitCommonControlsEx (&icex);
	
    // Create main window.
    hWnd = CreateWindowEx(WS_EX_NODRAG,       // Ex style flags
		TEXT("MyClass"),    // Window class
		TEXT("KWord"),      // Window title
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
    if (!IsWindow (hWnd)) return -2; 
	
	hIcon=LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	int test = SendMessage(hWnd, WM_SETICON, FALSE, (LPARAM)hIcon);
	
	hwndMW=hWnd;
	
	GetClientRect (hWnd, &rect);
	
	hwndSB = CreateWindowEx( 
		0,                      // no extended styles 
		L"SCROLLBAR",           // scroll bar control class 
		(PTSTR) NULL,           // no window text 
		WS_CHILD | WS_VISIBLE   // window styles  
		| SBS_VERT,				// horizontal scroll bar style 
		rect.right-SBWIDTH,     // horizontal position 
		TOPHEADERDEADZONE,		// vertical position 
		rect.right,             // width of the scroll bar 
		rect.bottom-TOPHEADERDEADZONE, // height of the scroll bar
		hWnd,					// handle to main window 
		(HMENU) NULL,           // no menu 
		hInstance,              // instance owning this window 
		(PVOID) NULL            // pointer not needed 
        );
	
	if (!IsWindow (hwndSB)) return -3;
	
	segments = new SEGMENT[50];
	TO_CreateFont();
	TO_GetTextSegments(hWnd, segments, &segmentsCount);
	Setup(hwndSB);
	
	CreateCommandBand (hWnd, TRUE);
	CreateCaret(hWnd, (HBITMAP) NULL, 2,10);
	SetCaretPos(100,100);
	ShowCaret(hWnd);
	
    ShowWindow (hWnd, nCmdShow);
    UpdateWindow (hWnd);
	return 0;
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

//----------------------------------------------------------------------
// CreateCommandBand - Create a formatted command band control.
//
int CreateCommandBand (HWND hWnd, BOOL fFirst) {
    HWND hwndCB, hwndBand;
    INT i;
    LONG lStyle;
    HBITMAP hBmp;
    HIMAGELIST himl;
    REBARBANDINFO rbi[NUMBANDS];
	
    // Create image list control for bitmaps for minimized bands.
    himl = ImageList_Create (16, 16, ILC_COLOR, 2, 0);
    hBmp = LoadBitmap (hInst, MAKEINTRESOURCE(IDB_CMDBAR));
    ImageList_Add (himl, hBmp, NULL);
    DeleteObject (hBmp);
    hBmp = LoadBitmap (hInst, MAKEINTRESOURCE(IDB_CMDBAND));
    ImageList_Add (himl, hBmp, NULL);
    DeleteObject (hBmp);
	//hBmp = LoadBitmap (hInst, MAKEINTRESOURCE(IDB_CMDEDIT));
    //ImageList_Add (himl, hBmp, NULL);
    //DeleteObject (hBmp);
	
    hwndCB = CommandBands_Create (hInst, hWnd, IDC_CMDBAND,
		RBS_SMARTLABELS, himl);
	
    // Load bitmap used as background for command bar.
    hBmp = LoadBitmap (hInst, TEXT ("CmdBarBack"));
    // Initialize common REBARBANDINFO structure fields.
    for (i = 0; i < dim(rbi); i++) {
        rbi[i].cbSize = sizeof (REBARBANDINFO);
        rbi[i].fMask = RBBIM_ID | RBBIM_IMAGE | RBBIM_SIZE |
			/*RBBIM_BACKGROUND | */ RBBIM_STYLE;
			rbi[i].wID = IDB_CMDBAND+i;
		//        rbi[i].hbmBack = hBmp;
		}
	
    cbr[0].cxRestored = 230;
    cbr[1].cxRestored = 210;
    cbr[1].fStyle = RBBS_FIXEDBMP;
	// Initialize REBARBANDINFO structure for each band.
	// 1. Menu band
	rbi[0].fStyle = RBBS_FIXEDBMP | RBBS_NOGRIPPER;
	rbi[0].cx = cbr[0].cxRestored;
	rbi[0].iImage = 0;
	
	// 2. Standard button band
	rbi[1].fMask |= RBBIM_TEXT;
	rbi[1].iImage = 1;
	rbi[1].lpText = TEXT ("");
	// The next two parameters are initialized from saved data.
	rbi[1].cx = cbr[1].cxRestored;
	rbi[1].fStyle = cbr[1].fStyle;
	
	// Add bands.
	CommandBands_AddBands (hwndCB, hInst, 2, rbi);
	
	// Add menu to first band.
	hwndBand = CommandBands_GetCommandBar (hwndCB, 0);
	CommandBar_InsertMenubar (hwndBand, hInst, IDR_MENU1, 0);
	// Add standard buttons to second band.
	hwndBand = CommandBands_GetCommandBar (hwndCB, 1);
	// Insert buttons
	CommandBar_AddBitmap (hwndBand, HINST_COMMCTRL, IDB_STD_SMALL_COLOR,
		16, 0, 0);
	CommandBar_AddButtons (hwndBand, dim(tbCBStdBtns), tbCBStdBtns);
	
	TCHAR szTmp[64];
	
	hwndFONT = CommandBar_InsertComboBox (hwndBand, hInst, 75,
		CBS_DROPDOWNLIST | WS_VSCROLL,
		IDC_FONT, 10);
	hwndFONTSIZE = CommandBar_InsertComboBox (hwndBand, hInst, 40,
		CBS_DROPDOWNLIST | WS_VSCROLL,
		IDC_FONTSIZE, 11);
	// Fill in combo box.
	for (i = 0; i < 10; i++) {
		wsprintf (szTmp, TEXT ("Item %d"), i);
		SendDlgItemMessageW(hwndBand, IDC_FONT, CB_INSERTSTRING, -1,
			(LPARAM)szTmp);
		}
	
	for (i = 0; i < 10; i++) {
		wsprintf (szTmp, TEXT ("%d"), i);
		SendDlgItemMessageW(hwndBand, IDC_FONTSIZE, CB_INSERTSTRING, -1,
			(LPARAM)szTmp);
		}
	
	SendDlgItemMessageW (hwndBand, IDC_FONT, CB_SETCURSEL, 0, 0);
	SendDlgItemMessageW (hwndBand, IDC_FONTSIZE, CB_SETCURSEL, 0, 0);
	
	// Modify the style flags of each command bar to make transparent.
	for (i = 0; i < NUMBANDS; i++) {
		hwndBand = CommandBands_GetCommandBar (hwndCB, i);
		lStyle = SendMessage (hwndBand, TB_GETSTYLE, 0, 0);
		lStyle |= TBSTYLE_TRANSPARENT;
		SendMessage (hwndBand, TB_SETSTYLE, 0, lStyle);
		}
	
	// Add exit button to command band.
	CommandBands_AddAdornments (hwndCB, hInst, 0, NULL);
	return 0;
}
