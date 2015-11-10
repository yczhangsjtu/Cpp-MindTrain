#include "BoolMatrix.h"
#include <cstring>

extern void * GameState;

BoolMatrix gameBoolMatrix;

void initBoolMatrix(BoolMatrix* matrix, int m, int n, int expire)
{
	matrix -> _game = boolMatrix;
	if(m > 0 && n > 0)
	{
		matrix -> _m = m;
		matrix -> _n = n;
	}
	else
	{
		matrix -> _m = 4;
		matrix -> _n = 4;
	}
	strcpy(matrix->_name, "Bool Matrix");
	matrix -> _matrix = allocBoolMatrix(matrix -> _m, matrix -> _n);
	matrix -> _bgColor = 0x00ffff;
	matrix -> _state = BM_MEMORIZE;
	matrix -> _time = 0;
	matrix -> _expire = expire;
}

void reallocBoolMatrix(BoolMatrix* matrix, int m, int n)
{
	if(m > 0 && n > 0)
	{
		freeBoolMatrix(matrix);
		matrix -> _m = m;
		matrix -> _n = n;
		matrix -> _matrix = allocBoolMatrix(matrix -> _m, matrix -> _n);
	}
}

void updateBoolMatrix(BoolMatrix* matrix, int t)
{
	matrix -> _time += t;
	switch(matrix->_state)
	{
	case BM_MEMORIZE:
	{
		if(matrix->_time > matrix->_expire)
			matrix->_state = BM_VANISH;
	}
	break;
	}
}

bMatrix allocBoolMatrix(int m, int n)
{
	if(m > 0 && n > 0)
	{
		bMatrix B = new bArray[m];
		for(int i = 0; i < m; i++)
			B[i] = new bool[n];
		return B;
	}
	else
		return NULL;
}

void freeBoolMatrix(BoolMatrix* B)
{
	int m = B -> _m;
	if(B->_matrix == NULL) return;
	if(m > 0)
	{
		for(int i = 0; i < m; i++)
			delete[] (B->_matrix)[i];
		delete[] B->_matrix;
	}
}

void randSetBoolMatrix(BoolMatrix* B)
{
	int m = B -> _m;
	int n = B -> _n;
	if(m > 0 && n > 0)
	{
		for(int i = 0; i < m; i++)
			for(int j = 0; j < n; j++)
				(B->_matrix)[i][j] = rand() % 2;
		B -> _state = BM_MEMORIZE;
		B -> _time = 0;
	}
}

void drawBoolMatrix(HDC hdc, BoolMatrix* matrix, int x, int y, int s)
{
	if(matrix->_state==BM_VANISH) return;
	
	int m = matrix -> _m;
	int n = matrix -> _n;
	if(m <= 0 || n <= 0) return;
	
	HBRUSH sbrush  = CreateSolidBrush( 0xff0000 );
	HBRUSH ebrush  = CreateSolidBrush( 0xffffff );
	HPEN   pen  = CreatePen(PS_SOLID, 1, 0x000000);
	
	SelectObject(hdc, pen);
	
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
		{
			int xi = x + s * j;
			int yj = y + s * i;
			if((matrix->_matrix)[i][j])
				SelectObject(hdc, sbrush);
			else
				SelectObject(hdc, ebrush);
				
			Rectangle(hdc, xi, yj, xi+s, yj+s);
		}
	
	DeleteObject(sbrush);
	DeleteObject(ebrush);
	DeleteObject(pen);
}