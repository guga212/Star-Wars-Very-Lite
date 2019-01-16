#pragma once

#include"App.h"
#include"Ekran.h"

class Game{
UINT N;
timer time;
UINT Width;
UINT Height;
int CmdShow;
HINSTANCE hInstance;
HWND hwnd;
Ekran* _Ekran;
HRESULT InitWindow();
int LOOP();
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK InitialWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if (Msg == WM_NCCREATE) {
		LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		void * lpCreateParam = create_struct->lpCreateParams;
		Game * this_window = reinterpret_cast<Game*>(lpCreateParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_window));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Game::StaticWndProc));
		return this_window->WndProc(hWnd, Msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	LONG_PTR user_data = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	Game * this_window = reinterpret_cast<Game *>(user_data);
	return this_window->WndProc(hWnd, Msg, wParam, lParam);
}



public:
	Game(HINSTANCE handle, int Cmd = SW_MAXIMIZE) :CmdShow(Cmd), hInstance(handle),Width(800),Height(600) {
		if (FAILED(InitWindow())) {
			MessageBox(hwnd, L"Ooops", L"Nie udalo sie utworzyc okno", MB_OK);
		}
		N = 0;
		_Ekran = new Ekran(hInstance,hwnd);
		time.restart_time();
		LOOP();
	}

};