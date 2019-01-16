#pragma once
#include "App.h"
#include "graphic.h"
#include "mng.h"
#include"camera.h"
#include"object.h"


class Ekran {
	
	
	HINSTANCE hinstance;
	HWND hwnd;


	res_mng* mng;


	sphere* sky;
	timer* timer1;

public:
	Ekran(HINSTANCE _hinstance,HWND _hwnd);

	void Updt();
	void DrawScene();
	~Ekran();

	
};
