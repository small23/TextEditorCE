#include "stdafx.h"
#include "FontsStorage.h"


HFONT* fonts=NULL;
int fontsArraySize=0;
int fontsArrayCounter=0;
FSFONTFAMILY* fontStash=NULL;
int fontStashCounter=0;
int fontStashMaxSize=0;

HFONT* FS_GetFonts(int *ArrayCounter)
{
	*ArrayCounter=fontsArrayCounter; 
	return fonts;
}

void FS_CreateFont(unsigned short* font, int size, unsigned char italic, unsigned char underline)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = size;
	lf.lfItalic=italic;
	lf.lfUnderline=underline;  
	lstrcpy(lf.lfFaceName, font);

	if (fonts==NULL)
	{
		fonts=(HFONT*)malloc(sizeof(HFONT)*10);
		fontsArraySize=10;
		fontsArrayCounter=0;
	}
	fonts[fontsArrayCounter]= CreateFontIndirect(&lf);
	fontsArrayCounter++;
}

void FS_InitFonts(HWND hWnd)
{
	HDC hdc;
	hdc=GetDC(hWnd);

	int rc = EnumFontFamilies(hdc, NULL, FS_FontsCallback, 0);

	for (int i=0; i< fontStashCounter; i++)
	{
		rc = EnumFontFamilies(hdc, fontStash[i].szFontFamily, FS_EnumSingleFontFamily, (LPARAM)&fontStash[i]);
	}

	ReleaseDC(hWnd,hdc);
}

int CALLBACK FS_FontsCallback(CONST LOGFONT *lplf, CONST TEXTMETRIC *lpntm, DWORD nFontType, LPARAM lParam)
{
	if (fontStash==NULL)
	{
		fontStash = (FSFONTFAMILY*)malloc(10*sizeof(FSFONTFAMILY));
		fontStashMaxSize = 5;
	}
	if (fontStashCounter>=fontStashMaxSize-1)
	{
		fontStashMaxSize+=5;
		fontStash = (FSFONTFAMILY*)realloc(fontStash, fontStashMaxSize * sizeof(FSFONTFAMILY));
	}

	lstrcpy(fontStash[fontStashCounter].szFontFamily, lplf->lfFaceName);
	int i=0;
	while (true)
	{
		if (fontStash[fontStashCounter].szFontFamily[i]!=(unsigned short)'\0' && fontStash[fontStashCounter].szFontFamily[i]!=L' ')
		{
			fontStash[fontStashCounter].szShortName[i]=fontStash[fontStashCounter].szFontFamily[i];
			i++;
		}
		else
			break;
	}
	fontStash[fontStashCounter].nNumFonts=0;
	fontStashCounter++;
	return 1;
}

int CALLBACK FS_EnumSingleFontFamily(CONST LOGFONT *lplf, CONST TEXTMETRIC *lpntm, DWORD nFontType, LPARAM lParam)
{
	FSFONTFAMILY* fontData;
	fontData = (FSFONTFAMILY*) lParam;
	fontData->nNumFonts++;
	return 1;
}

int FS_GetFontsCounter(){return fontStashCounter;}

FSFONTFAMILY* FS_GetFontFamily(int i)
{
	if (i<fontStashCounter)
		return &fontStash[i];
	else
		return NULL;
}