
#pragma once
#include"time.h"

float timer::get_time() {
	if (stop == 0) {
		DWORD dwTimeCur = GetTickCount();
		t = (dwTimeCur - dwTimeStart);
		return t - offset;
	}
	else
		return 0;
}

void timer::restart_time() {
	dwTimeStart = GetTickCount();
}

void timer::start_offset() {
	offsetStart = GetTickCount();

}
void timer::push_offset() {
	DWORD dwTimeCur = GetTickCount();
	offset += (dwTimeCur - offsetStart);
}