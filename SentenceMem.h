#ifndef SENTENCE_MEM_H
#define SENTENCE_MEM_H

#include <windows.h>
#include <cstdlib>
#include "Game.h"

const int SM_MEMORIZE = 0;
const int SM_VANISH   = 1;
const int SM_APPEAR   = 2;

typedef struct SentenceMem
{
	        Game _game;
	        char _name[32];
			char _sentence[256];
		     int _bgColor;
			 int _state;
} SentenceMem;

void initSentenceMem(SentenceMem* sm, const char *sentence);
void randSetSentence(SentenceMem* sm);
void drawSentence(HDC hdc, SentenceMem* sm, int x, int y, int w, int h, int s);

#endif