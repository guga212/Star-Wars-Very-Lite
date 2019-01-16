#pragma once
#include"App.h"

class timer{
private:
	bool stop;
	float t;
	float offset;
	DWORD dwTimeStart;
	DWORD offsetStart;
public:
	
	timer(bool _stop=0) {
		DWORD dwTimeStart= GetTickCount();
		offset = 0;
		t = 0;
		stop =_stop;
	}

	float get_time();
	void start_offset();
	void push_offset();
	void restart_time();

	void start_time() {
//	DWORD dwTimeStart = GetTickCount();
	stop = 0;
	}
	void stop_time() {
		stop = 1;
	}
};



/*
static float t = 0.0f;
t += (float)XM_PI * 0.0125f;
static DWORD dwTimeStart = 0;
DWORD dwTimeCur = GetTickCount();
if (dwTimeStart == 0)
dwTimeStart = dwTimeCur;
t = (dwTimeCur - dwTimeStart) / 1000.0f;
*/