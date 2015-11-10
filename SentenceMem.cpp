#include "SentenceMem.h"
#include <string>
#include <vector>
#include <cstdlib>

SentenceMem gameSentenceMem;

std::vector<std::string> Sentences;

void initSentenceMem(SentenceMem* sm, const char *sentence)
{
	sm -> _game = sentenceMem;
	strcpy(sm -> _name, "Sentence Memorizing");
	strncpy(sm -> _sentence, sentence, 255);
	sm -> _bgColor = 0x00ffff;
	sm -> _state = SM_MEMORIZE;
}

void randSetSentence(SentenceMem* sm)
{
	if(Sentences.size()==0)
		strncpy(sm->_sentence,"There is no sentence!",255);
	else
	{
		int i = rand() % Sentences.size();
		strncpy(sm->_sentence,Sentences.at(i).c_str(),255);
	}
	sm -> _state = SM_MEMORIZE;
}

void drawSentence(HDC hdc, SentenceMem* sm, int x, int y, int w, int h, int s)
{
	if(sm->_state==SM_VANISH) return;
	
	HFONT font = CreateFont(
			  s, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
			  OUT_DEFAULT_PRECIS,
			  CLIP_DEFAULT_PRECIS,
			  DEFAULT_QUALITY,
			  DEFAULT_PITCH,
			  NULL
			);
			
	SetTextColor(hdc, RGB(0,0,0));
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc,font);
	RECT rc = {x,y,x+w,y+h};
	DrawText(hdc, sm->_sentence, -1, &rc, DT_WORDBREAK);
	
	DeleteObject(font);
}