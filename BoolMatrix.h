#ifndef BOOL_MATRIX_H
#define BOOL_MATRIX_H

#include <windows.h>
#include <cstdlib>
#include "Game.h"

const int BM_MEMORIZE = 0;
const int BM_VANISH   = 1;
const int BM_APPEAR   = 2;

typedef bool *  bArray;
typedef bool ** bMatrix;

typedef struct BoolMatrix
{
	        Game _game;
	        char _name[32];
	unsigned int _m;
	unsigned int _n;
	     bMatrix _matrix;
		     int _bgColor;
			 int _state;
			 int _time;
			 int _expire;
} BoolMatrix;

void initBoolMatrix(BoolMatrix* matrix, int m, int n, int expire);
void reallocBoolMatrix(BoolMatrix* matrix, int m, int n);
void updateBoolMatrix(BoolMatrix* matrix, int t);
bMatrix allocBoolMatrix(int m, int n);
void freeBoolMatrix(BoolMatrix* B);
void randSetBoolMatrix(BoolMatrix* B);
void drawBoolMatrix(HDC hdc, BoolMatrix* matrix, int x, int y, int s);

#endif