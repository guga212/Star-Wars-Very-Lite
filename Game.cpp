#include "Game.h"
// funkcja nasluchiwujaca
LRESULT CALLBACK Game::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (Msg)
	{
	case WM_ACTIVATEAPP:
		Keyboard::ProcessMessage(Msg, wParam, lParam);
		Mouse::ProcessMessage(Msg, wParam, lParam);
		break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(Msg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(Msg, wParam, lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return 0;
}


HRESULT Game::InitWindow()
{
	//Rejestracja klasy
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &Game::InitialWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Klasa";
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	//Rejestracja okna
	if (FAILED(RegisterClassEx(&wcex))) {
		return 0;
	}
	RECT rc = { 0, 0, 400, 300 };
	GetClientRect(hwnd, &rc);
	AdjustWindowRect(&rc, 0, FALSE);
	Width = rc.right - rc.left;
	Height = rc.bottom - rc.top;
	hwnd = CreateWindow(L"Klasa", L"StarWars_VERY_LITE", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
	
		return 0;
	}
	ShowWindow(hwnd, CmdShow);
	UpdateWindow(hwnd);

		return S_OK;

}
//petla gry
int Game::LOOP() {
	MSG msg = { 0 };
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			_Ekran->DrawScene();
			++N;

			if(1/*N<680*/){
			_Ekran->Updt();
		
			}
			if (time.get_time()>1000) {
				N = 0;
				time.restart_time();
			}
		}
	}
	return (int)msg.wParam;
}


