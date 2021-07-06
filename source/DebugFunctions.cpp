#include "stdafx.h"
#include "DebugFunctions.h"

void DF_StringSizeTest(HWND hWnd)
{
	SIZE results;
	TEXTMETRIC test;
	
	wchar_t bufferA[512]=TEXT("Some kind  q|{} of telkglosdf;ljas;ldfdiuagsdgajsghdkixt");
	int length = wcslen(bufferA);
	
	__int64 start = DF_Milliseconds_now();
	HDC hdc;
	hdc = GetDC(hWnd);
	GetTextMetrics(hdc, &test ); 
	for (int i=0; i<100000; i++)
	{
		
		GetTextExtentPoint32(hdc, bufferA, length,&results);
		
	}
	ReleaseDC(hWnd,hdc);
	__int64 stop = DF_Milliseconds_now();
	__int64 timer=stop-start;
	
	wchar_t  buffer[200];
	
	int len=swprintf( buffer,L"Test 100,000 Text length getting, Total:%d ms", timer);
	
	MessageBox(NULL, buffer, TEXT("Results"), MB_OK);
	return;
}

void DF_MemOperationsTest()
{
	wchar_t bufferA[512]=TEXT("Some kind of text");
	int textLength=wcslen(bufferA)*sizeof(wchar_t);
	int currPos=4;
	__int64 start = DF_Milliseconds_now();
	for (long i=0; i<1000000; i++)
	{
		memmove(&bufferA[currPos+1],&bufferA[currPos], textLength-currPos);
		bufferA[currPos]=L'd';
		memmove(&bufferA[currPos],&bufferA[currPos+1], textLength-currPos);
	}

	__int64 stop = DF_Milliseconds_now();

	__int64 timer=stop-start;
	
	wchar_t  buffer[200];
	
	int len=swprintf( buffer,L"Test 1,000,000 memory shifts (1 pos) and delete char, Total:%d ms", timer);
	
	MessageBox(NULL, buffer, TEXT("Results"), MB_OK);
}

void DF_SpeedTest(HWND hWnd, TOCURSORPOS* carrage, int lineScrolls, SEGMENT* segments, int segmentsCount)
{

	SCROLLINFO si;
	
	// Get scroll bar position.
	si.cbSize = sizeof (si);
	si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
	
	int ans=GetScrollInfo (hWnd, SB_CTL, &si);
	__int64 stop=0;
	__int64 start=0;
	__int64 part2=0;
	__int64 part3=0;
	__int64 drawTime=0;
	__int64 updateTime=0;
	
	start= DF_Milliseconds_now();
	
	int a=lineScrolls;
	int b=0;
	int c=0;;
	for (int j=0; j<5; j++)
	{
		part2=DF_Milliseconds_now();
		for (int jj=0; jj<50; jj++)
		{
			GF_DrawTextByLine(segments, carrage, segmentsCount, a, b , 0, 13);
			c=a;
			a=b;
			b=c;
		}
		part3=DF_Milliseconds_now();
		updateTime+=part3-part2;
	}
	
	stop=DF_Milliseconds_now();
	__int64 timer=stop-start;
	
	wchar_t  buffer[200];
	
	int len=swprintf( buffer,L"Test 250 redraws, Total:%dms Update%dms ", timer, updateTime);
	len+=swprintf( buffer+len,L"FPS: %f",(float)250/(float(timer)/1000.0));
	
	MessageBox(NULL, buffer, TEXT("Results"), MB_OK);	
}

__int64 DF_Milliseconds_now(){
    static _LARGE_INTEGER s_frequency;
    static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
    if (s_use_qpc) {
        _LARGE_INTEGER now;
		
        QueryPerformanceCounter(&now);
        return (1000L * now.QuadPart) / s_frequency.QuadPart;
    } else {
        return GetTickCount();
    }
}