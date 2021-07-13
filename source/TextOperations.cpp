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

TEXTMETRIC* fontsMetrics=NULL;
int segmentsCount=0;
int width=0;
bool cursorUpDownRepeat = false;


int TO_GetSegmentCount(){return segmentsCount;};

void TO_GetTextSegments(SEGMENT* segments, HWND hWnd, RECT rect)
{
	HDC hdc;

	FS_CreateFont(L"Arial", 16, 0,0);
	hdc = GetDC(hWnd);
	
	int countFonts = 0;
	SelectObject(hdc, FS_GetFonts(&countFonts)[0]);
	wchar_t* texts[]={someText1,someText2, someText9,someText3, someText4, someText5, someText6, someText9, someText7, someText8, someText9,someText2, someText9,someText3, someText4, someText5, someText6, someText9, someText4, someText5, someText6, someText9, someText7, someText8, someText9,someText2, someText9,someText3, someText4, someText5, someText6, someText9,}; 
	
	for(int i=0; i<sizeof(texts)/sizeof(wchar_t*); i++)
	{
		segments[i].text=(wchar_t*)malloc(512*sizeof(wchar_t));//texts[i];
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
		segments[i].linesHeight=(int*)malloc(32*sizeof(int));
		
		memcpy(segments[i].text,texts[i],segments[i].length*sizeof(wchar_t));

		TO_RecheckSpacesAndLines(segments, i, hdc, rect);

		segmentsCount++;
	}

	ReleaseDC(hWnd,hdc);
}

void TO_RecheckSpacesAndLines(SEGMENT* segments, int i, HDC hdc,RECT rect)
{		
	int countFonts = 0;
	SelectObject(hdc, FS_GetFonts(&countFonts)[0]);
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
		segments[i].linesLength[0]=0;
		segments[i].linesHeight[0]=16; //TODO Change to font used size
		return;
	}
	segments[i].spacesPointer[spaceArrayCounter]=segments[i].length;                                                                                                                                  
	spaceArrayCounter++;
	begin=0;
	count=1;
	lastSpacePointer=0;
	int lastJ=0;
	for (int j=0; j<spaceArrayCounter; j++)
	{
		while (j<spaceArrayCounter) //ѕроходим по массиву пробелов определ€€ предельную длинну строки (сколько вместитс€ строк с пробелами)
		{
			lastSpacePointer=segments[i].spacesPointer[j];
			GetTextExtentPoint32(hdc, &segments[i].text[begin], lastSpacePointer-begin,&textMetrics);
			if (textMetrics.cx>=rect.right-rect.left)
				break;
			j++;
		}
		if (j-lastJ<=1 && textMetrics.cx>=rect.right-rect.left) //≈сли поместилось лишь одно слово - мы вылезли за пределы экрана, бьем слово на части
		{
			//ƒержал 4 строки на Athlon XP 5 строк с полной перерисовкой
			//ѕосле отступа на 5 строк держал около 15
			//ѕосле прогрессивной шкалы отступа около 25 строк
			//ѕрогрессивна€ шкала отсупа, отсчет от начала до  конца строки - не смог повеситть
			lastSpacePointer=begin;
			textMetrics.cx=10;
			while (textMetrics.cx<rect.right-rect.left) //ѕервый проход примерно определ€ем возможную длинну
			{
				lastSpacePointer+=10;
				GetTextExtentPoint32(hdc, &segments[i].text[begin], lastSpacePointer-begin,&textMetrics);
			}
			while (textMetrics.cx>rect.right-rect.left) //¬торой проход уточн€ем длинну доступную дл€ отображени€
			{
				lastSpacePointer--;
				GetTextExtentPoint32(hdc, &segments[i].text[begin], lastSpacePointer-begin,&textMetrics);
			}
			
			lastSpacePointer--;

			segments[i].lineEnds[count]=lastSpacePointer;
			segments[i].linesLength[count-1]=lastSpacePointer-begin;
			segments[i].linesCounter++;
			segments[i].linesHeight[count-1]=textMetrics.cy;
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
			segments[i].linesHeight[count-1]=textMetrics.cy;
			begin=lastSpacePointer+1;
			lastJ=j;
			count++;
		}
		else //≈сли дошли до конца сегмента
		{
			segments[i].lineEnds[count]=lastSpacePointer+1;
			segments[i].linesLength[count-1]=lastSpacePointer-begin;
			segments[i].linesCounter++;
			segments[i].linesHeight[count-1]=textMetrics.cy;
			begin=lastSpacePointer+1;
			lastJ=j;
			count++;
		}
	}
}

bool TO_CheckSegmentPointers(SEGMENT* segments,int* segmentPointer, int* inSegmentLine)
{
	if ((*inSegmentLine)>=segments[*segmentPointer].linesCounter)
	{
		if ((*segmentPointer)+1>=segmentsCount)
			return false;
		*inSegmentLine=0;
		(*segmentPointer)++;
		
	}
	else if ((*inSegmentLine)<0)
	{
		if((*segmentPointer)-1<0)
			return false;
		(*segmentPointer)--;
		*inSegmentLine=segments[*segmentPointer].linesCounter-1;
	}
	return true;
}

void TO_CalcCarragePos(SEGMENT* segments,TOCURSORPOS* carrage,HDC hdc, RECT rect)
{
	int countFonts = 0;
	SelectObject(hdc, FS_GetFonts(&countFonts)[0]);
	int i=1;
	while (segments[carrage->segment].lineEnds[i]<=carrage->position && segments[carrage->segment].length !=0)
	{
		i++;
	}
	SIZE textMetrics;
	GetTextExtentPoint32(hdc, &segments[carrage->segment].text[segments[carrage->segment].lineEnds[i-1]], carrage->position - segments[carrage->segment].lineEnds[i-1],&textMetrics);
	carrage->x=textMetrics.cx + rect.left;
	carrage->height=textMetrics.cy;
	carrage->lineLocation=i-1;
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
	TO_CalcCarragePos(segments,carrage, hdc, rect);

	cursorUpDownRepeat=false;
}

void TO_DeleteSymbol(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect)
{
	if(carrage->position>0)
	{
		memmove(&segments[carrage->segment].text[carrage->position-1],&segments[carrage->segment].text[carrage->position], (segments[carrage->segment].length-carrage->position)*sizeof(wchar_t));
		segments[carrage->segment].length--;
		carrage->position--;
	}
	else //≈сли переходим на новый сегмент //TODO Deleting-mergin segments
	{
		carrage->lineLocation--;
		if (TO_CheckSegmentPointers(segments, &carrage->segment,&carrage->lineLocation))
		{
			carrage->position=segments[carrage->segment].length;
			memmove(&segments[carrage->segment].text[carrage->position-1],&segments[carrage->segment].text[carrage->position], (segments[carrage->segment].length-carrage->position)*sizeof(wchar_t));
			segments[carrage->segment].length--;
			carrage->position--; 
		}
		else
			carrage->lineLocation++;
	}
	TO_RecheckSpacesAndLines(segments, carrage->segment, hdc, rect);
	TO_CalcCarragePos(segments,carrage, hdc, rect);

	cursorUpDownRepeat=false;
}

void TO_AddSegment(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect)
{


}

void TO_HomeEnd(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, WPARAM key)
{
	switch (key)
	{
		case VK_END:
			if (segments[carrage->segment].length>0)
				carrage->position=segments[carrage->segment].lineEnds[carrage->lineLocation+1]-1;
			else
				carrage->position=segments[carrage->segment].lineEnds[carrage->lineLocation];
			break; 
		case VK_HOME:
			carrage->position=segments[carrage->segment].lineEnds[carrage->lineLocation];
			break; 
	}
	cursorUpDownRepeat=false;
	TO_CalcCarragePos(segments,carrage, hdc, rect);
}

void TO_Arrows(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, WPARAM arrow)
{
	switch (arrow) 
	{
	case VK_LEFT:
		cursorUpDownRepeat=false;
		carrage->position--;
		if (carrage->position<0)
		{
			if (carrage->segment>0)
			{
				carrage->segment--;
				carrage->position=segments[carrage->segment].length;
			}
			else
				carrage->position=0;
		}
		break; 
	case VK_RIGHT:
		cursorUpDownRepeat=false;
		carrage->position++;
		if (carrage->position>segments[carrage->segment].length)
		{
			if (carrage->segment<segmentsCount-1)
			{
				carrage->segment++;
				carrage->position=0;
			}
			else
				carrage->position--;;
		}
		break; 
	case VK_UP:
		TO_ArrowUpDownPosCalc(segments, carrage, hdc, rect, -1);
		break; 

	case VK_DOWN:
		TO_ArrowUpDownPosCalc(segments, carrage, hdc, rect, 1);
		break; 
	} 	
	TO_CalcCarragePos(segments,carrage, hdc, rect);
}

void TO_ArrowUpDownPosCalc(SEGMENT* segments,TOCURSORPOS* carrage, HDC hdc, RECT rect, int counter)
{
	SIZE textMetrics;
	int countFonts;

	SelectObject(hdc, FS_GetFonts(&countFonts)[0]);
	if (!cursorUpDownRepeat)
	{
		GetTextExtentPoint32(hdc, &segments[carrage->segment].text[segments[carrage->segment].lineEnds[carrage->lineLocation]], carrage->position - segments[carrage->segment].lineEnds[carrage->lineLocation],&textMetrics);      
		width = textMetrics.cx;
		cursorUpDownRepeat=true;
	}

	carrage->lineLocation+=counter;
	if (TO_CheckSegmentPointers(segments, &carrage->segment,&carrage->lineLocation))
	{
		carrage->position=segments[carrage->segment].lineEnds[carrage->lineLocation];
	}
	else
	{
		carrage->lineLocation-=counter;
		return;
	}

	if (segments[carrage->segment].length==0)
		return;

	textMetrics.cx=-1;
	textMetrics.cy=-1;

	while (textMetrics.cx<width)
	{
		GetTextExtentPoint32(hdc, &segments[carrage->segment].text[segments[carrage->segment].lineEnds[carrage->lineLocation]],
			carrage->position - segments[carrage->segment].lineEnds[carrage->lineLocation],&textMetrics);
		if (carrage->position>=segments[carrage->segment].lineEnds[carrage->lineLocation+1])
		{
			carrage->position--;
			break;
		}
		if (textMetrics.cx>=width)
		{
			if (textMetrics.cx==width){break;}
			else if (carrage->position > 0 && carrage->position > segments[carrage->segment].lineEnds[carrage->lineLocation])
			{
				int widthHigh=textMetrics.cx;
				GetTextExtentPoint32(hdc, &segments[carrage->segment].text[segments[carrage->segment].lineEnds[carrage->lineLocation]],
					(carrage->position -1)  - segments[carrage->segment].lineEnds[carrage->lineLocation],&textMetrics);
				if (widthHigh-width>width-textMetrics.cx)
					carrage->position--;
			}
			break;
		}
		carrage->position++;
	}
}