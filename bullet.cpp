#pragma once
#include "object.h"
//#////////////////////////////#//
//////////#//bullet_obj//#//////////
//#////////////////////////////#//

//*******************************//
//*******************************//


//#//////////////////////////////////#//
//////////#//Simple_bullet//#//////////
void bullet_simple::chck_pos() {

	XMFLOAT3 cnt;
	for (int i = 0; i < ship::allshp_pos.size(); i++) {

		XMStoreFloat3(&cnt, XMVector3Length(ship::allshp_pos[i]->pos - lokal.pos));
		if (abs(cnt.x < 0.5f*ship::allshp_pos[i]->size) && (lokal.id_cr != ship::allshp_pos[i]->id)) {
			lokal.hp -= 10;
				//MessageBox(NULL, L"pocisk -HP", L"upii", 1);
			if ((lokal.team != ship::allshp_pos[i]->team)) {
				ship::allshp_pos[i]->hp -= lokal.dmg;
				//MessageBox(NULL, L"pocisk -HP", L"upii", 1);
			}
		}
	}
}

void bullet_simple::updt() {
	chck_pos();
	lokal.pos += lokal.kier*(lokal.v / 100);
	pos = lokal.pos;
}

bool bullet_simple::death_chck() {

	XMFLOAT3 cnt;
	XMStoreFloat3(&cnt, XMVector3Length(lokal.pos - lokal.start_pos));

	if ((lokal.hp <= 0) || (cnt.x > 60.0f))
		return 1;
	else
		return 0;
}

//#///////////////////////////////////////#//
//////////#//INtelligent_bullet//#//////////
void bullet_int::chck_pos() {

	XMFLOAT3 cnt;
	for (int i = 0; i < ship::allshp_pos.size(); i++) {

		XMStoreFloat3(&cnt, XMVector3Length(ship::allshp_pos[i]->pos - lokal.pos));
		if (abs(cnt.x < 0.7f*ship::allshp_pos[i]->size) && (lokal.id_cr != ship::allshp_pos[i]->id)) {
			lokal.hp -= 10;
			//MessageBox(NULL, L"pocisk -HP", L"upii", 1);
			if ((lokal.team != ship::allshp_pos[i]->team)) {
				ship::allshp_pos[i]->hp -= lokal.dmg;
				//MessageBox(NULL, L"pocisk -HP", L"upii", 1);
			}
		}
	}
}

void bullet_int::updt() {
	chck_pos();
	n_count++;
	lokal.TARGpos = target->pos;
	XMFLOAT3 cmp;
	XMFLOAT3 cmpl1;
	XMFLOAT3 cmpl2;

	XMStoreFloat3(&cmp, XMVectorEqual(lokal.TARGpos, lokal.pos));

	if ((cmp.x == 0) || (cmp.y == 0) || (cmp.z == 0)) {

		XMVECTOR targ_vect = lokal.TARGpos - lokal.pos;

		XMStoreFloat3(&cmpl1, XMVector3Length(targ_vect));

			lokal.pos += lokal.kier*(lokal.v / 100);


		targ_vect = XMVector3Normalize(targ_vect);
		lokal.kier = XMVector3Normalize(lokal.kier);

		XMVECTOR os_obr = XMVector3Cross(lokal.kier, targ_vect);

		XMFLOAT3 angle;
		XMStoreFloat3(&angle, XMVector3AngleBetweenVectors(lokal.kier, targ_vect));

		if ((angle.x != 0.0f) && (angle.x != XM_PI)) {
			if (abs(angle.x) > lokal.w / 80) {
				if (angle.x>0)
					lokal.kier = XMVector3TransformCoord(lokal.kier, XMMatrixRotationAxis(os_obr, lokal.w / 80));
				else
					lokal.kier = XMVector3TransformCoord(lokal.kier, XMMatrixRotationAxis(os_obr, lokal.w / 80));
			}
		}
	}
	kier = lokal.kier;
	pos = lokal.pos;
}

bool bullet_int::death_chck() {

	XMFLOAT3 cnt;
	XMStoreFloat3(&cnt, XMVector3Length(lokal.pos - lokal.start_pos));

	if ((lokal.hp <= 0) || (cnt.x > 120.0f)||(n_count>1000))
		return 1;
	else
		return 0;
}