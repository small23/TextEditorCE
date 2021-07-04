#include "stdafx.h"
#include "TextOperations.h"

wchar_t *someText1=TEXT("It goes without saying that humans (mammals identifiable as those that stand upright and are comparatively advanced and capable of detailed thought) have pretty remarkable bodies, given all that they've accomplished. (Furthermore, an especially intelligent human brain produced this text!) To be sure, humans have overcome predators, disease, and all sorts of other obstacles over thousands of years.");
wchar_t *someText2=TEXT("To fully understand and appreciate these accomplishments, let's take at some of the most well-known parts of the human body!");
wchar_t *someText3=TEXT("The head, or the spherical body part that contains the brain and rests at the top of the human body, has quite a few individual organs and body parts on it. (It should quickly be mentioned that hair occupies the space on top of the head, and the ears, the organs responsible for hearing, are located on either side of the head.) From top to bottom, the eyebrows, or horizontal strips of hair that can be found above the eye, are the first components of the head. The eyes are below them, and are round, orb-like organs that allow humans to see.");
wchar_t *someText4=TEXT("The eyes make way for the nose, or an external (sticking-out) organ that plays an important part in the breathing and bacteria-elimination processes. Below that is the mouth, or a wide, cavernous organ that chews food, removes bacteria, helps with breathing, and more. The mouth contains teeth, or small, white-colored, pointed body parts used to chew food, and the tongue, or a red-colored, boneless organ used to chew food and speak.");
wchar_t *someText5=TEXT("The neck is the long body part that connects the head to the chest (the muscular body part that protects the heart and lungs), and the stomach, or the part of the body that contains food and liquid-processing organs, comes below that.");
wchar_t *someText6=TEXT("The legs are the long, muscular body parts that allow humans to move from one spot to another and perform a variety of actions. Each leg contains a thigh (a thick, especially muscular body part used to perform strenuous motions; the upper part of the leg) and a calf (thinner, more flexible body part that absorbs the shock associated with movement; the lower part of the leg). Feet can be found at the bottom of legs, and each foot is comprised of five toes, or small appendages that help balance.");
wchar_t *someText7=TEXT("Arms are long, powerful body parts that are located on either side of chest, below the shoulders;arms are comprised of biceps (the thicker, more powerful upper portion), and forearms (the thinner, more flexible lower portion). Hands, or small, gripping body parts used for a tremendous number of actions, are at the end of arms. Each hand contains five fingers, or small appendages used to grip objects.");
wchar_t *someText8=TEXT("The aforementioned shoulders are rounded body parts that aid arms' flexibility. One's back is found on the opposite side of the stomach, and is a flat section of the body that contains important muscles that're intended to protect the lungs and other internal organs, in addition to helping humans perform certain motions and actions.");
wchar_t *someText9=TEXT("");
wchar_t *someText10=TEXT("");

HFONT* fonts=NULL;
TEXTMETRIC* fontsMetrics=NULL;
int fontsArraySize=0;
int fontsArrayCounter=0;
TOCURSORPOS cursorPosition={0,0};

TOCURSORPOS* TO_GetCursorPosition()
{
	return &cursorPosition;
}

void TO_SetCursorPositionByTap(int x, int y)
{
	
}

void TO_SetCursorPositionByShift(int shift)
{
	
}

HFONT* TO_GetFonts(int *ArrayCounter)
{
	*ArrayCounter=fontsArrayCounter; 
	return fonts;
}

void TO_CreateFont()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));          // Clear out structure.
	lf.lfHeight = FONTHEIGHT;                         // Request a 12-pixel-height
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lstrcpy(lf.lfFaceName, _T("Arial"));

	if (fonts==NULL)
	{
		fonts=(HFONT*)malloc(sizeof(HFONT)*10);
		fontsArraySize=10;
		fontsArrayCounter=0;
	}
	fonts[fontsArrayCounter]= CreateFontIndirect(&lf);  // Create the font.
	fontsArrayCounter=1;
}

void TO_GetTextSegments(HWND hWnd, SEGMENT* segments, int* segmentsCount, RECT rect)
{
	HDC hdc;

	hdc = GetDC(hWnd);

	SelectObject(hdc, fonts[0]);
	wchar_t* texts[]={someText1,someText2, someText9,someText3, someText4, someText5, someText6, someText9, someText7, someText8, someText9,someText2, someText9,someText3, someText4, someText5, someText6, someText9, someText4, someText5, someText6, someText9, someText7, someText8, someText9,someText2, someText9,someText3, someText4, someText5, someText6, someText9,}; 
	
	for(int i=0; i<sizeof(texts)/sizeof(wchar_t*); i++)
	{
		segments[i].text=(wchar_t*)malloc(512*sizeof(wchar_t));//texts[i];
		//entersPos=(int*)realloc(entersPos,1536);
		segments[i].textArraySize=512;
		segments[i].length=wcslen(texts[i]);
		while(segments[i].length > segments[i].textArraySize)
		{
			segments[i].text=(wchar_t*)realloc(segments[i].text, segments[i].textArraySize*sizeof(wchar_t)*2);
			segments[i].textArraySize=segments[i].textArraySize*2;
		}
		segments[i].spacesPointer=(int*)malloc(512*sizeof(int));
		segments[i].linesCounter=0;
		segments[i].lineEnds=(int*)malloc(32*sizeof(int));
		segments[i].lineEnds[0]=0;
		segments[i].spacesCounter=0;
		segments[i].linesLength=(int*)malloc(32*sizeof(int));
		segments[i].linesLength[0]=0;
		segments[i].lineEnds[0]=0;
		
		memcpy(segments[i].text,texts[i],segments[i].length*sizeof(wchar_t));

		TO_RecheckSpacesAndLines(segments, i, hdc, rect);

		(*segmentsCount)++;
	}
	ReleaseDC(hWnd,hdc);
}

void TO_RecheckSpacesAndLines(SEGMENT* segments, int i, HDC hdc,RECT rect)
{		
	SelectObject(hdc, fonts[0]);
	segments[i].linesCounter=0;
	SIZE textMetrics;
	int count=0;
	int spaceArrayCounter=0;
	int arrayCounter=0;
	int lastSpacePointer;
	int begin;
	//подсчет числа пробелов в сегменте - проходим весь сегмент
	while (count<segments[i].length)
	{
		if (segments[i].text[count]==' ')
		{
			//lastSpacePointer=count;
			segments[i].spacesPointer[spaceArrayCounter]=count; //¬ массив загон€ютс€ координаты пробелов
			spaceArrayCounter++;
		}
		count++;
	}
	if (segments[i].length==0) //≈сли сегмент ничего не содержит - указываем что он содержит 1 строку дл€ отрисовки
	{
		segments[i].linesCounter=1;
		segments[i].spacesPointer[0]=0;
		return;
	}
	segments[i].spacesPointer[spaceArrayCounter]=count-1;
	spaceArrayCounter++;
	begin=0;
	count=1;
	lastSpacePointer=0;
	int lastJ=0;
	for (int j=0; j<spaceArrayCounter; j++)
	{
		while (j<spaceArrayCounter) //ѕроходим по массиву пробелов определ€€ предельную длинну строки (сколько вместитс€ строк с пробелами)
		{
			lastSpacePointer=segments[i].spacesPointer[j]; //TODO!!!!!! ѕри сканировании не учитываетс€ последнее слово сегмента
			GetTextExtentPoint32(hdc, &segments[i].text[begin], lastSpacePointer-begin,&textMetrics);
			if (textMetrics.cx>=rect.right-rect.left)
				break;
			j++;
		}
		if (j-lastJ<=1) //≈сли поместилось лишь одно слово - мы вылезли за пределы экрана, бьем слово на части
		{
			//ƒержал 4 строки на Athlon XP 5 строк с полной перерисовкой
			//ѕосле отступа на 5 строк держал около 15
			//ѕосле прогрессивной шкалы отступа около 25 строк
			//ѕрогрессивна€ шкала отсупа, отсчет от начала до  конца строки - не смог повеситть
			lastSpacePointer=begin;
			textMetrics.cx=10;
			while (textMetrics.cx<rect.right) //ѕервый проход примерно определ€ем возможную длинну
			{
				lastSpacePointer+=10;
				GetTextExtentPoint32(hdc, &segments[i].text[begin], lastSpacePointer-begin,&textMetrics);
			}
			while (textMetrics.cx>rect.right) //¬торой проход уточн€ем длинну доступную дл€ отображени€
			{
				lastSpacePointer--;
				GetTextExtentPoint32(hdc, &segments[i].text[begin], lastSpacePointer-begin,&textMetrics);
			}
			
			lastSpacePointer--;

			segments[i].lineEnds[count]=lastSpacePointer;
			segments[i].linesLength[count-1]=lastSpacePointer-begin;
			segments[i].linesCounter++;
			begin=lastSpacePointer;
			count++;
			j--;
		}
		else if (j!=spaceArrayCounter) //≈сли ограничени€ строки найдены - записываем координаты строки, ее длинну, счетчик линий повышаем
		{
			j--;
			lastSpacePointer=segments[i].spacesPointer[j];
			segments[i].lineEnds[count]=lastSpacePointer+1;
			segments[i].linesLength[count-1]=lastSpacePointer-begin;
			segments[i].linesCounter++;
			begin=lastSpacePointer+1;
			lastJ=j;
			count++;
		}
		else //≈сли дошли до конца сегмента
		{
			segments[i].lineEnds[count]=lastSpacePointer+1;
			segments[i].linesLength[count-1]=lastSpacePointer-begin+1;
			segments[i].linesCounter++;
			begin=lastSpacePointer+1;
			count++;
			lastJ=j;
		}
	}
}

void TO_InsertSymbol(SEGMENT* segments,TOCURSORPOS* carrage, wchar_t simbol, HDC hdc, RECT rect)
{
	if (segments[carrage->segment].length>=segments[carrage->segment].textArraySize)
	{
			segments[carrage->segment].text=(wchar_t*)realloc(segments[carrage->segment].text, segments[carrage->segment].textArraySize*sizeof(wchar_t)+512*sizeof(wchar_t));
			segments[carrage->segment].textArraySize=segments[carrage->segment].textArraySize+512;
	}
	memmove(&segments[carrage->segment].text[carrage->position+1],&segments[carrage->segment].text[carrage->position], (segments[carrage->segment].length-carrage->position)*sizeof(wchar_t));
	segments[carrage->segment].text[carrage->position]=simbol;
	segments[carrage->segment].length++;
	carrage->position++;
	TO_RecheckSpacesAndLines(segments, carrage->segment, hdc, rect);
}

void TO_DeleteSymbol(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect)
	{
		if(carrage->position>0)
		{
			memmove(&segments[carrage->segment].text[carrage->position-1],&segments[carrage->segment].text[carrage->position], (segments[carrage->segment].length-carrage->position)*sizeof(wchar_t));
			segments[carrage->segment].length--;
			carrage->position--;
		}
		else //≈сли переходим на новый сегмент
		{

		}
		TO_RecheckSpacesAndLines(segments, carrage->segment, hdc, rect);
	}