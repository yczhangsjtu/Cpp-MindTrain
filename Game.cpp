#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "Game.h"

#define APPNAME "GAME"
#define CONFIG_FILE "config.txt"
#define SENTENCE_FILE "sentence.txt"

using namespace std;

void * GameState = NULL;
extern BoolMatrix gameBoolMatrix;
extern SentenceMem gameSentenceMem;
extern std::vector<std::string> Sentences;

const int nGames = 2;
void * games[nGames] = {&gameBoolMatrix, &gameSentenceMem};
void **pg = games;

const int bufsize = 256;

int timer = 0;

HINSTANCE g_hInst;          // current instance

void init()
{
	int bm_m = 4;
	int bm_n = 4;
	int bm_expire = 3000;
	ifstream config(CONFIG_FILE);
	if(config)
	{
		string read;
		int tmp;
		while(!config.eof())
		{
			config >> read;
			if(read=="BoolMatrixM")
			{
				config >> tmp;
				if(tmp > 0 && tmp < 10)
					bm_m = tmp;
			}
			else if(read=="BoolMatrixN")
			{
				config >> tmp;
				if(tmp > 0 && tmp < 10)
					bm_n = tmp;
			}
			else if(read=="BoolMatrixExpire")
			{
				config >> tmp;
				if(tmp > 0)
					bm_expire = tmp;
			}
		}
		config.close();
	}
	ifstream sentence(SENTENCE_FILE);
	if(sentence)
	{
		char read[256];
		while(!sentence.eof())
		{
			sentence.getline(read,256);
			read[255] = '\0';
			Sentences.push_back(read);
		}
		sentence.close();
	}
	
	srand((unsigned)time(0));
	initBoolMatrix(&gameBoolMatrix, bm_m, bm_n, bm_expire);
	randSetBoolMatrix(&gameBoolMatrix);
	initSentenceMem(&gameSentenceMem,"Begin!");
	randSetSentence(&gameSentenceMem);
	timer = clock();
	pg = games;
	GameState = *pg;
}

void updateGameState(void *p, int dt)
{
	Game g = *(Game*)GameState;
	switch(g)
	{
	case boolMatrix:
	{
		updateBoolMatrix((BoolMatrix*)GameState,dt);
	}
	break;
	}
}

void drawText(HDC hdc, RECT rc, const char * text)
{
	SetTextColor(hdc, RGB(255,255,255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, text, -1, &rc, 0);
}

void drawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	POINT points[2];
	points[0].x = x1;
	points[0].y = y1;
	points[1].x = x2;
	points[1].y = y2;
	Polyline(hdc, points, 2);
}

void draw(HWND hwnd)
{
	HDC         hdc, DC;
	RECT        rc, tRC;
	//PAINTSTRUCT ps;
	hdc = GetDC(hwnd);
	DC = CreateCompatibleDC(hdc);
	GetClientRect(hwnd, &rc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc,rc.right,rc.bottom);
	SelectObject(DC,hBmp);
	
	Game g = *(Game*)GameState;
	switch(g)
	{
	case boolMatrix:
	{
		BoolMatrix * p = (BoolMatrix *) GameState;
		int s = (rc.bottom - rc.top) / (p->_m);
		int x = (rc.right - rc.left - s * p->_n)/2;
		HBRUSH bBrush  = CreateSolidBrush( p -> _bgColor );
		SelectObject(DC, bBrush);
		Rectangle(DC, rc.left, rc.top, rc.right, rc.bottom);
		drawBoolMatrix(DC, p, x, 0, s);
		DeleteObject(bBrush);
	}
	break;
	case sentenceMem:
	{
		SentenceMem * p = (SentenceMem *) GameState;
		int s = 50;
		int x = 0, y = 0;
		HBRUSH bBrush  = CreateSolidBrush( p -> _bgColor );
		SelectObject(DC, bBrush);
		Rectangle(DC, rc.left, rc.top, rc.right, rc.bottom);
		drawSentence(DC,p,x,y,rc.right-x,rc.bottom-y,s);
		DeleteObject(bBrush);
	}
	break;
	}
	
	BitBlt(hdc,0,0,rc.right,rc.bottom,DC,0,0,SRCCOPY);
	//InvalidateRect(hwnd,NULL,TRUE);
	
	DeleteDC(DC);
	DeleteObject(hBmp);
	ReleaseDC(hwnd,hdc);
	
	int dt = clock() - timer;
	timer = clock();
	updateGameState(GameState,dt);
}

void quit(HWND hwnd)
{
	freeBoolMatrix(&gameBoolMatrix);
	DestroyWindow(hwnd);
}

void onKeyEnter()
{
	Game g = *(Game*)GameState;
	switch(g)
	{
	case boolMatrix:
	{
		BoolMatrix * p = (BoolMatrix *) GameState;
		if(p -> _state == BM_VANISH)
		{
			p -> _state = BM_APPEAR;
		}
		else if(p -> _state == BM_APPEAR)
		{
			randSetBoolMatrix(p);
		}
	}
	break;
	case sentenceMem:
	{
		SentenceMem * p = (SentenceMem *) GameState;
		if(p -> _state == SM_MEMORIZE)
			p-> _state = SM_VANISH;
		else if(p -> _state == SM_VANISH)
			p-> _state = SM_APPEAR;
		else if(p -> _state == SM_APPEAR)
			randSetSentence(p);
	}
	break;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		// ----------------------- press esc to get out of it...
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				quit(hwnd);
			}
			else if(wParam == VK_LEFT)
			{
				pg --;
				if(pg < games) pg = games;
				GameState = *pg;
			}
			else if(wParam == VK_RIGHT)
			{
				pg ++;
				if(pg >= games+nGames) pg = games+nGames-1;
				GameState = *pg;
			}
			break;
		case WM_CHAR:
			if(wParam == VK_BACK)
			{
			}
			else if(wParam == VK_RETURN)
			{
				onKeyEnter();
			}
			else
			{
			}
			InvalidateRect(hwnd,NULL,TRUE);
			break;
		// ----------------------- display our minimal info
		case WM_PAINT:
			draw(hwnd);
			break;
		// ----------------------- let windows do all other stuff
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

// Main window thread
int WINAPI WinMain(
   HINSTANCE hInstanceExe,
   HINSTANCE hPrevInstance,
   PTSTR pszCmdLine,
   int nCmdShow)
{
	init();
	g_hInst = hInstanceExe;
	// Fill in window class structure with parameters that describe
	// the main window.
	
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof wc);
	wc.hInstance     = hInstanceExe;
	wc.lpszClassName = APPNAME;
	wc.lpfnWndProc   = WndProc;
	wc.style         = CS_VREDRAW|CS_HREDRAW;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	
	if (FALSE == RegisterClass(&wc)) return 0;

	// create the browser
	HWND hwnd; // Handle of the window
	hwnd = CreateWindow(APPNAME, APPNAME,
		WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, g_hInst, 0);
	
	if (hwnd == NULL) return 0;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}