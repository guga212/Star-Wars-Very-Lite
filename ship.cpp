#pragma once
#include "object.h"

int ship_sml::n_sml;
int ship_middle::n_middle;
int ship_big::n_big;
//#////////////////////////////#//
//////////#//ship_obj//#//////////
//#////////////////////////////#//
 std::vector<inf_ship_obj*> ship::allshp_pos;
//*******************************//
//*******************************//


//#////////////////////////////#//
//////////#//sml_ship//#//////////
//#////////////////////////////#//

 void ship_sml::updt() {
	 updt_elmnts();
	
	 if (lokal.hp > 0) {

	      flw_enm();
		  rnd_bhv();
		  go_away();
		  anti_coll();

	    // must have funkcji

		 flw_targ();
		 upd_stat();
	 }

 }

 /////#//bchv_func//#/////////
 //#//////////////////////#//


/////#//agressive//#/////////
 void ship_sml::shoot() {
	 if (clocks[0]->get_time() > 1000) {

		 bullet* wsk2 = new bullet_simple(lokal.kier, lokal.pos, lokal.id, lokal.team, lokal.dmg);
		 crtd_objects.push_back(wsk2);

		 clocks[0]->restart_time();
	 }

 }
 void ship_sml::flw_enm() {

	 XMFLOAT3 cnt;
	 float min = 1000000.0f;

	 for (int i = 0; i < allshp_pos.size(); i++) {
		 if ((allshp_pos[i]->team != lokal.team) && (allshp_pos[i]->hp > 0) && (lokal.stan <= flw_enmy)) {
			 lokal.stan = flw_enmy;
			 XMStoreFloat3(&cnt, XMVector3Length(allshp_pos[i]->pos - lokal.pos));
			 if (cnt.x < min) {
				 min = cnt.x;
				 lokal.TARGpos = allshp_pos[i]->pos;
				 if (cnt.x < 50.0f) {
					 XMVECTOR targ_vect = lokal.TARGpos - lokal.pos;
					 XMStoreFloat3(&cnt, XMVector3AngleBetweenVectors(lokal.kier, targ_vect));
					 if (abs(cnt.x) < lokal.w / 40) {
						 shoot();
					 }


				 }
			 }
		 }
		 else
		 {
			 if (lokal.stan == flw_enmy)
				 lokal.stan = vac;
		 }
	 }
 }
 ////////#//end//#//////////

 void ship_sml::go_away() {


	 float min = 100000.0f;
	 int enm_nmb = 0;
	 XMFLOAT3 cnt;
	 int al_nmb;

	 for (int i = 0; i < allshp_pos.size(); i++) {

		 if (lokal.team != allshp_pos[i]->team) {
			 enm_nmb++;
		 }
	 }

	 al_nmb = allshp_pos.size() - enm_nmb;



	 for (int i = 0; i < allshp_pos.size(); i++) {

		 if (allshp_pos[i]->team != lokal.team){
			 
			 XMStoreFloat3(&cnt, XMVector3Length(allshp_pos[i]->pos - lokal.pos));
		 if (min > cnt.x)
			 min = cnt.x;
		 }

		 }
	 

	

			 if ((min < 25.0f) && (lokal.stan < run) && (lokal.hp<0.6*lokal.hp_max) && (al_nmb <= enm_nmb)) {
				 lokal.TARGpos = (-400 * lokal.kier);
				 lokal.w = 1.35*lokal.w_max;
				 lokal.stan = run;
			 }
			
				 if ((min>30.0f) && (lokal.stan == run)){
					 lokal.stan = vac;
					// MessageBox(NULL, L"STOP", L"Distance", 1);
				 }
		 
	 if ((enm_nmb == 0) && (lokal.stan == run))
		 lokal.stan = vac;

 }

 void ship_sml::rnd_bhv() {
	 int enm_nmb = 0;
	 for (int i = 0; i < allshp_pos.size(); i++) {

		 if ((lokal.team != allshp_pos[i]->team)&&(allshp_pos[i]->hp>0)) {
			 enm_nmb++;
		 }
	 }
	 // random behaviour
	 if ((lokal.stan <= rnd)&&(1)) {
		 if (rand() % 100 == 0) {
			 lokal.TARGpos = XMVectorSet(rand() % 8 * 25, rand() % 6 * 18, rand() % 8 * 28, 0);
		 }
		 lokal.stan = rnd;
	 }
	 //	else{
	 //		lokal.stan = vac;
	 //	}
 }

 void ship_sml::anti_coll() {
	 XMFLOAT3 cnt;
	 XMFLOAT3 cnt1;

	 if (lokal.stan == anticoll) {
		 n_alg++;
	 }
	 int fl_vac=1;
	 for (int i = 0; i < allshp_pos.size(); i++) {

		 XMStoreFloat3(&cnt, XMVector3Length(allshp_pos[i]->pos - lokal.pos));

		 if (((cnt.x < 8.f) && (cnt.x != 0)) && (lokal.stan < anticoll) && (allshp_pos[i]->stan != anticoll)&&(n_safe<=0)) {
			 n_safe = 400;
			 n_alg = 0;

			 XMStoreFloat3(&cnt1, XMVectorEqual(allshp_pos[i]->kier, lokal.kier));
			 if ((cnt1.x == 0) || (cnt1.y == 0) || (cnt1.z == 0)) {
				 lokal.TARGpos = XMVectorSet(-50.0f + (rand() % 100), -50.0f + (rand() % 100), -50.0f + (rand() % 100), 0.0f);
				 lokal.TARGpos = lokal.kier*(-40.f);
				 lokal.TARGpos = XMVector3Cross(lokal.kier + XMVectorSet(-0.2, 0.4f, 0.12, 0.0f), allshp_pos[i]->kier)*45.0f;
				 
			 }
			 else {
				 lokal.TARGpos = XMVector3Cross(allshp_pos[i]->kier, lokal.kier)*45.0f;
				 allshp_pos[i]->TARGpos = -lokal.TARGpos;
			 }
			 allshp_pos[i]->w = allshp_pos[i]->w_max*1.4;
			 allshp_pos[i]->stan = anticoll;
			 lokal.w = 1.4*lokal.w_max;
			 lokal.stan = anticoll;
			 break;
		 }

		 if ((cnt.x < 16.f)&&(cnt.x!=0)) {
			 fl_vac=0;
		 }

	 }

	 if (((fl_vac) || (n_alg>3000)) && (lokal.stan == anticoll)) {
		 lokal.stan = vac;
	 }
	 if (n_alg > 3000) {
		 n_safe--;
	 }
 }
 //#////////end///////////#//

////#// must have state//#///////
 void ship_sml::upd_stat() {


	 if (lokal.hp <= (lokal.hp*0.35)) {
		 lokal.v = 0.45*lokal.v_max;
		 lokal.dmg = 0.45*lokal.v_max;
		 lokal.w = 0.45*lokal.w_max;
	 }
	 else {
		 if (lokal.hp <= (lokal.hp*0.5)) {
			 lokal.v = 0.7*lokal.v_max;
			 lokal.dmg = 0.7*lokal.v_max;
			 lokal.w = 0.7*lokal.w_max;
		 }
	 }

 }

 void ship_sml::col_det() {
	 XMFLOAT3 cnt;
	 for (int i = 0; i < ship::allshp_pos.size(); i++) {

		 XMStoreFloat3(&cnt, XMVector3Length(ship::allshp_pos[i]->pos - lokal.pos));
		 if (abs(cnt.x < lokal.size* 0.3f*ship::allshp_pos[i]->size) && (lokal.id != ship::allshp_pos[i]->id)) {

			if(lokal.hp_max<=allshp_pos[i]->hp_max){
				 lokal.hp -= lokal.hp_max;
				 allshp_pos[i]->hp -= lokal.hp_max;
				 //MessageBox(NULL, L"pocisk -HP", L"upii", 1);
			}
			else
			{
				lokal.hp -= lokal.hp_max;
				allshp_pos[i]->hp -= 0.7*lokal.hp_max;
			}
		
		 }
	 }
 }

 void ship_sml::flw_targ() {
	 
	 XMFLOAT3 cmp;
	 XMFLOAT3 cmpl1;
	 XMFLOAT3 cmpl2;

	 XMStoreFloat3(&cmp, XMVectorEqual(lokal.TARGpos, lokal.pos));

	 if ((cmp.x == 0) || (cmp.y == 0) || (cmp.z == 0)) {

		 XMVECTOR targ_vect = lokal.TARGpos - lokal.pos;

		 XMStoreFloat3(&cmpl1, XMVector3Length(targ_vect));

		 if (abs(cmpl1.x) >4.0f)
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
	 // przesuw wspolrzednych


	 }

 void ship_sml::updt_boom(){
	 ////#//updt_booms//#////
	 if ((lokal.hp <= 0) && (!destroyed)) {
		 object* wsk = new explose(lokal.pos, 2.4f, 180);
		 crtd_objects.push_back(wsk);
		 destroyed = 1;
		 // MessageBox(NULL, L"BOOM", L"BOOM", 1);
	 }
	 ////#//end_booms//#////
 }

 void ship_sml::updt_elmnts() {
	 col_det();
	 updt_boom();

	 pos = lokal.pos;
	 kier = lokal.kier;

	 ////#//updt_objects//#////
	 XMFLOAT3 cnt;

	 for (int i = 0; i < crtd_objects.size(); ++i) {
		 crtd_objects[i]->updt();
		 if (crtd_objects[i]->death_chck()) {
			 delete crtd_objects[i];
			 crtd_objects[i] = NULL;
			 // MessageBox(NULL, L"deleted", L"zniszcz pocis", 1);
		 }

	 }
	 ////#//end_updt_objects//#////




	 std::vector<object*>::iterator it;
	 for (it = crtd_objects.begin(); it != crtd_objects.end(); )
	 {

		 if ((*it) == NULL)
			 it = crtd_objects.erase(it);
		 else
			 ++it;
	 }

 }

 bool ship_sml::death_chck() {


	 bool fl_all_dead=1;

	 if (crtd_objects.size() == 0)
		 fl_all_dead = 1;


	 for (int i = 0; i < crtd_objects.size(); ++i) {
		 if (!crtd_objects[i]->death_chck()) {
			 fl_all_dead = 0;
		 }
	 }
	

	 if ((lokal.hp <= 0)&&(fl_all_dead))
		 return 1;
	 else
		 return 0;
 }
 //#////////end///////////#//


 //-----------------------------------------------------------------//
 //-----------------------------------------------------------------//
 //-----------------------------------------------------------------//
 //-----------------------------------------------------------------//




 //#/////////////////////////////#//
 ////////#//middle_ship//#/////////
 //#///////////////////////////#//

 void ship_middle::updt() {
	 updt_elmnts();

	 if (lokal.hp > 0) {

		 flw_enm();
		 rnd_bhv();
		 go_away();
		 anti_coll();

		 // must have funkcji

		 flw_targ();
		 upd_stat();
	 }
 }

 /////#//bchv_func//#/////////
 //#//////////////////////#//


 /////#//agressive//#/////////
 void ship_middle::shoot() {
	 if (clocks[0]->get_time() > 1000) {


		 XMFLOAT3 cnt;
		 XMVECTOR rot2 = XMVector3Cross(lokal.kier, XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
		 XMVECTOR angle = XMVector3AngleBetweenVectors(lokal.kier, XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
		 XMStoreFloat3(&cnt, angle);
		 rot2 = XMVector3Normalize(rot2);
		 XMMATRIX m1 = XMMatrixRotationAxis(rot2, cnt.x);
		 XMVECTOR rot1 = XMVector3TransformCoord(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), m1);


		 bullet* wsk1 = new bullet_simple(lokal.kier, lokal.pos - 0.8 * rot1, lokal.id, lokal.team, lokal.dmg);
		 crtd_objects.push_back(wsk1);

		 bullet* wsk2 = new bullet_simple(lokal.kier, lokal.pos, lokal.id, lokal.team, lokal.dmg);
		 crtd_objects.push_back(wsk2);

		 bullet* wsk3 = new bullet_simple(lokal.kier, lokal.pos + 0.8 * rot1, lokal.id, lokal.team, lokal.dmg);
		 crtd_objects.push_back(wsk3);


		 clocks[0]->restart_time();
	 }
 }
 

 void ship_middle::flw_enm() {

	 XMFLOAT3 cnt;
	 float min = 1000000.0f;

	 for (int i = 0; i < allshp_pos.size(); i++) {
		 if ((allshp_pos[i]->team != lokal.team) && (allshp_pos[i]->hp > 0) && (lokal.stan <= flw_enmy)) {
			 lokal.stan = flw_enmy;
			 XMStoreFloat3(&cnt, XMVector3Length(allshp_pos[i]->pos - lokal.pos));
			 if (cnt.x < min) {
				 min = cnt.x;
				 lokal.TARGpos = allshp_pos[i]->pos;
				 if (cnt.x < 50.0f) {
					 XMVECTOR targ_vect = lokal.TARGpos - lokal.pos;
					 XMStoreFloat3(&cnt, XMVector3AngleBetweenVectors(lokal.kier, targ_vect));
					 if (abs(cnt.x) < lokal.w / 40) {
						 shoot();
					 }


				 }
			 }
		 }
		 else
		 {
			 if (lokal.stan == flw_enmy)
				 lokal.stan = vac;
		 }
	 }
 }
 ////////#//end//#//////////

 void ship_middle::go_away() {


	 float min = 100000.0f;
	 int enm_nmb = 0;
	 XMFLOAT3 cnt;
	 int al_nmb;

	 for (int i = 0; i < allshp_pos.size(); i++) {

		 if (lokal.team != allshp_pos[i]->team) {
			 enm_nmb++;
		 }
	 }

	 al_nmb = allshp_pos.size() - enm_nmb;



	 for (int i = 0; i < allshp_pos.size(); i++) {

		 if (allshp_pos[i]->team != lokal.team) {

			 XMStoreFloat3(&cnt, XMVector3Length(allshp_pos[i]->pos - lokal.pos));
			 if (min > cnt.x)
				 min = cnt.x;
		 }

	 }




	 if ((min < 25.0f) && (lokal.stan < run) && (lokal.hp<0.4*lokal.hp_max) && (al_nmb <= enm_nmb)) {
		 lokal.TARGpos = (-400 * lokal.kier);
		 lokal.w = 1.35*lokal.w_max;
		 lokal.stan = run;
	 }

	 if ((min>30.0f) && (lokal.stan == run)) {
		 lokal.stan = vac;
		 // MessageBox(NULL, L"STOP", L"Distance", 1);
	 }

	 if ((enm_nmb == 0) && (lokal.stan == run))
		 lokal.stan = vac;

 }
 

 void ship_middle::rnd_bhv() {
	 int enm_nmb = 0;
	 for (int i = 0; i < allshp_pos.size(); i++) {

		 if ((lokal.team != allshp_pos[i]->team) && (allshp_pos[i]->hp>0)) {
			 enm_nmb++;
		 }
	 }
	 // random behaviour
	 if ((lokal.stan <= rnd) && (1)) {
		 if (rand() % 100 == 0) {
			 lokal.TARGpos = XMVectorSet(rand() % 8 * 25, rand() % 6 * 18, rand() % 8 * 28, 0);
		 }
		 lokal.stan = rnd;
	 }
	 //	else{
	 //		lokal.stan = vac;
	 //	}
 }


 void ship_middle::anti_coll() {
	 XMFLOAT3 cnt;
	 XMFLOAT3 cnt1;

	 if (lokal.stan == anticoll) {
		 n_alg++;
	 }
	 int fl_vac = 1;
	 for (int i = 0; i < allshp_pos.size(); i++) {

		 XMStoreFloat3(&cnt, XMVector3Length(allshp_pos[i]->pos - lokal.pos));

		 if (((cnt.x < 9.f) && (cnt.x != 0)) && (lokal.stan < anticoll) && (allshp_pos[i]->stan != anticoll) && (n_safe <= 0)) {
			 n_safe = 400;
			 n_alg = 0;

			 XMStoreFloat3(&cnt1, XMVectorEqual(allshp_pos[i]->kier, lokal.kier));
			 if ((cnt1.x == 0) || (cnt1.y == 0) || (cnt1.z == 0)) {
				 lokal.TARGpos = XMVectorSet(-50.0f + (rand() % 100), -50.0f + (rand() % 100), -50.0f + (rand() % 100), 0.0f);
				 lokal.TARGpos = lokal.kier*(-40.f);
				 lokal.TARGpos = XMVector3Cross(lokal.kier + XMVectorSet(-0.2, 0.4f, 0.12, 0.0f), allshp_pos[i]->kier)*45.0f;

			
				 allshp_pos[i]->TARGpos = -lokal.TARGpos;

			 }
			 else {
				 lokal.TARGpos = XMVector3Cross(allshp_pos[i]->kier, lokal.kier)*45.0f;

			
				 allshp_pos[i]->TARGpos = -lokal.TARGpos;
			 }
			 allshp_pos[i]->w = allshp_pos[i]->w_max*1.4;
			 allshp_pos[i]->stan = anticoll;
			 lokal.w = 1.4*lokal.w_max;
			 lokal.stan = anticoll;
			 break;
		 }

		 if ((cnt.x < 18.f) && (cnt.x != 0)) {
			 fl_vac = 0;
		 }

	 }

	 if (((fl_vac) || (n_alg>3000)) && (lokal.stan == anticoll)) {
		 lokal.stan = vac;
	 }
	 if (n_alg > 3000) {
		 n_safe--;
	 }
 }
 //#////////end///////////#//

 ////#// must have state//#///////
 void ship_middle::upd_stat() {


	 if (lokal.hp <= (lokal.hp*0.35)) {
		 lokal.v = 0.45*lokal.v_max;
		 lokal.dmg = 0.45*lokal.v_max;
		 lokal.w = 0.45*lokal.w_max;
	 }
	 else {
		 if (lokal.hp <= (lokal.hp*0.5)) {
			 lokal.v = 0.7*lokal.v_max;
			 lokal.dmg = 0.7*lokal.v_max;
			 lokal.w = 0.7*lokal.w_max;
		 }
	 }

 }

 void ship_middle::col_det() {
	 XMFLOAT3 cnt;
	 for (int i = 0; i < ship::allshp_pos.size(); i++) {

		 XMStoreFloat3(&cnt, XMVector3Length(ship::allshp_pos[i]->pos - lokal.pos));
		 if (abs(cnt.x < lokal.size* 0.25f*ship::allshp_pos[i]->size) && (lokal.id != ship::allshp_pos[i]->id)) {

			 if (lokal.hp_max <= allshp_pos[i]->hp_max) {
				 lokal.hp -= lokal.hp_max;
				 allshp_pos[i]->hp -= lokal.hp_max;
				 //MessageBox(NULL, L"pocisk -HP", L"upii", 1);
			 }
			 else
			 {
				 lokal.hp -= lokal.hp_max;
				 allshp_pos[i]->hp -= 0.7*lokal.hp_max;
			 }

		 }
	 }
 }

 void ship_middle::flw_targ() {

	 XMFLOAT3 cmp;
	 XMFLOAT3 cmpl1;
	 XMFLOAT3 cmpl2;

	 XMStoreFloat3(&cmp, XMVectorEqual(lokal.TARGpos, lokal.pos));

	 if ((cmp.x == 0) || (cmp.y == 0) || (cmp.z == 0)) {

		 XMVECTOR targ_vect = lokal.TARGpos - lokal.pos;

		 XMStoreFloat3(&cmpl1, XMVector3Length(targ_vect));

		 if (abs(cmpl1.x) >6.0f*size)
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
	 // przesuw wspolrzednych


 }

 void ship_middle::updt_boom() {
	 ////#//updt_booms//#////
	 if ((lokal.hp <= 0) && (!destroyed)) {
		 object* wsk = new explose(lokal.pos, 2.4f*size, 180);
		 crtd_objects.push_back(wsk);
		 destroyed = 1;
		 // MessageBox(NULL, L"BOOM", L"BOOM", 1);
	 }
	 ////#//end_booms//#////
 }

 void ship_middle::updt_elmnts() {
	 col_det();
	 updt_boom();

	 pos = lokal.pos;
	 kier = lokal.kier;

	 ////#//updt_objects//#////
	 XMFLOAT3 cnt;

	 for (int i = 0; i < crtd_objects.size(); ++i) {
		 crtd_objects[i]->updt();
		 if (crtd_objects[i]->death_chck()) {
			 delete crtd_objects[i];
			 crtd_objects[i] = NULL;
			 // MessageBox(NULL, L"deleted", L"zniszcz pocis", 1);
		 }

	 }
	 ////#//end_updt_objects//#////




	 std::vector<object*>::iterator it;
	 for (it = crtd_objects.begin(); it != crtd_objects.end(); )
	 {

		 if ((*it) == NULL)
			 it = crtd_objects.erase(it);
		 else
			 ++it;
	 }

 }

 bool ship_middle::death_chck() {


	 bool fl_all_dead = 1;

	 if (crtd_objects.size() == 0)
		 fl_all_dead = 1;


	 for (int i = 0; i < crtd_objects.size(); ++i) {
		 if (!crtd_objects[i]->death_chck()) {
			 fl_all_dead = 0;
		 }
	 }


	 if ((lokal.hp <= 0) && (fl_all_dead))
		 return 1;
	 else
		 return 0;
 }
 //#////////end///////////#//



 //#////////////////////////////#//
 //////////#//BIG_ship//#//////////
 //#////////////////////////////#//

 void ship_big::updt() {
	 updt_elmnts();

	 if (lokal.hp > 0) {

		 flw_enm();
		 rnd_bhv();
		 go_away();
		 anti_coll();

		 // must have funkcji

		 flw_targ();
		 upd_stat();
	 }

 }

 /////#//bchv_func//#/////////
 //#//////////////////////#//


 /////#//agressive//#/////////

 void ship_big::shoot() {

		 if (clocks[0]->get_time() > 4600) {

			 bullet* wsk2;
			 for (int i = 0; i < targets.size();i++){

			 if(lokal.team==1){
 			  wsk2= new bullet_int(XMVector3Normalize(-lokal.TARGpos), allshp_pos[targets[i]], lokal.pos, lokal.id, lokal.team, lokal.dmg);
			  crtd_objects.push_back(wsk2);
 			 }
			 if (lokal.team == 0) {
				 wsk2 = new bullet_int(XMVector3Normalize(lokal.TARGpos), allshp_pos[targets[i]], lokal.pos, lokal.id, lokal.team, lokal.dmg);
				 crtd_objects.push_back(wsk2);
			 }
			 }
			 

			 clocks[0]->restart_time();
			
		 }

	
	 }
 


 void ship_big::flw_enm() {

	 XMFLOAT3 cnt;
	 float min = 1000000.0f;

	 for (int k = 0; k < targets.size(); k++) {
		 targets[k] = 0;
	 }
	 std::vector<UINT>::iterator it;
	 for (it = targets.begin(); it != targets.end(); )
	 {

		 if ((*it) == 0)
			 it = targets.erase(it);
		 else
			 ++it;
	 }

	 for (int i = 0; i < allshp_pos.size(); i++) {
		 if ((allshp_pos[i]->team != lokal.team) && (allshp_pos[i]->hp > 0) && (lokal.stan <= flw_enmy)) {
			 lokal.stan = flw_enmy;
			 XMStoreFloat3(&cnt, XMVector3Length(allshp_pos[i]->pos - lokal.pos));

			 if (cnt.x < 160.0f) {
				 bool stop = 0;
				 for (int j = 0; j < targets.size(); j++) {

					 if (targets[j] == i)
						 stop = 1;
				 }
				 if ((!stop)&&(targets.size()<3)) {
					 targets.push_back(i);
					
				 }
			 }

			
			
			 
			 

			 if (cnt.x < min) {
				 min = cnt.x;
				 lokal.TARGpos = allshp_pos[i]->pos;
				
			 }
		 }
		 else
		 {
			 if (lokal.stan == flw_enmy)
				 lokal.stan = vac;
		 }
	 }
	 shoot();
 }
 ////////#//end//#//////////

 void ship_big::go_away() {


	 float min = 100000.0f;
	 int enm_nmb = 0;
	 XMFLOAT3 cnt;
	 int al_nmb;

	 for (int i = 0; i < allshp_pos.size(); i++) {

		 if (lokal.team != allshp_pos[i]->team) {
			 enm_nmb++;
		 }
	 }

	 al_nmb = allshp_pos.size() - enm_nmb;



	 for (int i = 0; i < allshp_pos.size(); i++) {

		 if (allshp_pos[i]->team != lokal.team) {

			 XMStoreFloat3(&cnt, XMVector3Length(allshp_pos[i]->pos - lokal.pos));
			 if (min > cnt.x)
				 min = cnt.x;
		 }

	 }




	 if ((min < 25.0f) && (lokal.stan < run) && (lokal.hp<0.05*lokal.hp_max) && (al_nmb <= enm_nmb)) {
		 lokal.TARGpos = (-400 * lokal.kier);
		 lokal.w = 1.35*lokal.w_max;
		 lokal.stan = run;
	 }

	 if ((min>30.0f) && (lokal.stan == run)) {
		 lokal.stan = vac;
		 // MessageBox(NULL, L"STOP", L"Distance", 1);
	 }

	 if ((enm_nmb == 0) && (lokal.stan == run))
		 lokal.stan = vac;

 }

 void ship_big::rnd_bhv() {
	 int enm_nmb = 0;
	 for (int i = 0; i < allshp_pos.size(); i++) {

		 if ((lokal.team != allshp_pos[i]->team) && (allshp_pos[i]->hp>0)) {
			 enm_nmb++;
		 }
	 }
	 // random behaviour
	 if ((lokal.stan <= rnd) && (1)) {
		 if (rand() % 100 == 0) {
			 lokal.TARGpos = XMVectorSet(rand() % 8 * 25, rand() % 6 * 18, rand() % 8 * 28, 0);
		 }
		 lokal.stan = rnd;
	 }
	 //	else{
	 //		lokal.stan = vac;
	 //	}
 }

 void ship_big::anti_coll() {
	 XMFLOAT3 cnt;
	 XMFLOAT3 cnt1;
	 int fl_vac = 1;
	 for (int i = 0; i < allshp_pos.size(); i++) {

		 XMStoreFloat3(&cnt, XMVector3Length(allshp_pos[i]->pos - lokal.pos));

		 if (((cnt.x < 6.f) && (cnt.x != 0)) && (lokal.stan < anticoll) && (allshp_pos[i]->stan != anticoll)&& (allshp_pos[i]->hp_max== lokal.hp_max)) {

			 XMStoreFloat3(&cnt1, XMVectorEqual(allshp_pos[i]->kier, lokal.kier));
			 if ((cnt1.x == 0) || (cnt1.y == 0) || (cnt1.z == 0)) {
				 lokal.TARGpos = XMVectorSet(-50.0f + (rand() % 100), -50.0f + (rand() % 100), -50.0f + (rand() % 100), 0.0f);
				 lokal.TARGpos = lokal.kier*(-40.f);
				 lokal.TARGpos = XMVector3Cross(allshp_pos[i]->kier, -lokal.kier + XMVectorSet(-0.2, 0.4f, 0.12, 0.0f))*45.0f;
				 allshp_pos[i]->TARGpos = XMVector3Cross(allshp_pos[i]->kier, lokal.kier + XMVectorSet(-0.03, 0.05f, 0.02, 0.0f))*45.0f;
			 }
			 else {
				 lokal.TARGpos = XMVector3Cross(allshp_pos[i]->kier, lokal.kier)*45.0f;
				 allshp_pos[i]->TARGpos = XMVector3Cross(allshp_pos[i]->kier, -lokal.kier)*45.0f;
			 }
			 allshp_pos[i]->w = allshp_pos[i]->w_max*1.4;
			 allshp_pos[i]->stan = anticoll;
			 lokal.w = 1.4*lokal.w_max;
			 lokal.stan = anticoll;
			 break;
		 }

		 if ((cnt.x <9.f*size) && (cnt.x != 0)) {
			 fl_vac = 0;
		 }

	 }
	 if ((fl_vac) && (lokal.stan == anticoll)) {
		 lokal.stan = vac;
	 }
 }
 //#////////end///////////#//

 ////#// must have state//#///////
 void ship_big::upd_stat() {


	 if (lokal.hp <= (lokal.hp*0.35)) {
		 lokal.v = 0.45*lokal.v_max;
		 lokal.dmg = 0.45*lokal.v_max;
		 lokal.w = 0.45*lokal.w_max;
	 }
	 else {
		 if (lokal.hp <= (lokal.hp*0.5)) {
			 lokal.v = 0.7*lokal.v_max;
			 lokal.dmg = 0.7*lokal.v_max;
			 lokal.w = 0.7*lokal.w_max;
		 }
	 }

 }

 void ship_big::col_det() {
	 XMFLOAT3 cnt;
	 for (int i = 0; i < ship::allshp_pos.size(); i++) {

		 XMStoreFloat3(&cnt, XMVector3Length(ship::allshp_pos[i]->pos - lokal.pos));
		 if (abs(cnt.x <lokal.size* 0.3f*ship::allshp_pos[i]->size) && (lokal.id != ship::allshp_pos[i]->id)) {

			 if (lokal.hp_max <= allshp_pos[i]->hp_max) {
				 lokal.hp -= lokal.hp_max;
				 allshp_pos[i]->hp -= lokal.hp_max;
				 //MessageBox(NULL, L"pocisk -HP", L"upii", 1);
			 }
			 else
			 {
				 lokal.hp -= lokal.hp_max;
				 allshp_pos[i]->hp -= 0.7*lokal.hp_max;
			 }

		 }
	 }
 }

 void ship_big::flw_targ() {

	 XMFLOAT3 cmp;
	 XMFLOAT3 cmpl1;
	 XMFLOAT3 cmpl2;

	 XMStoreFloat3(&cmp, XMVectorEqual(lokal.TARGpos, lokal.pos));

	 if ((cmp.x == 0) || (cmp.y == 0) || (cmp.z == 0)) {

		 XMVECTOR targ_vect = lokal.TARGpos - lokal.pos;

		 XMStoreFloat3(&cmpl1, XMVector3Length(targ_vect));

		 if (abs(cmpl1.x) >40.0f)
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
			 else
			 {
				 lokal.TARGpos *= 20;
			 }
		 }
	 }
	 // przesuw wspolrzednych


 }

 void ship_big::updt_boom() {
	 ////#//updt_booms//#////
	 if ((lokal.hp <= 0) && (!destroyed)) {
		 object* wsk = new explose(lokal.pos, 1.5*size*2.4f, 480);
		 crtd_objects.push_back(wsk);
		 destroyed = 1;
		 // MessageBox(NULL, L"BOOM", L"BOOM", 1);
	 }
	 ////#//end_booms//#////
 }

 void ship_big::updt_elmnts() {
	 col_det();
	 updt_boom();

	 pos = lokal.pos;
	 kier = lokal.kier;

	 ////#//updt_objects//#////
	 XMFLOAT3 cnt;

	 for (int i = 0; i < crtd_objects.size(); ++i) {
		 crtd_objects[i]->updt();
		 if (crtd_objects[i]->death_chck()) {
			 delete crtd_objects[i];
			 crtd_objects[i] = NULL;
			 // MessageBox(NULL, L"deleted", L"zniszcz pocis", 1);
		 }

	 }
	 ////#//end_updt_objects//#////




	 std::vector<object*>::iterator it;
	 for (it = crtd_objects.begin(); it != crtd_objects.end(); )
	 {

		 if ((*it) == NULL)
			 it = crtd_objects.erase(it);
		 else
			 ++it;
	 }

 }

 bool ship_big::death_chck() {


	 bool fl_all_dead = 1;

	 if (crtd_objects.size() == 0)
		 fl_all_dead = 1;


	 for (int i = 0; i < crtd_objects.size(); ++i) {
		 if (!crtd_objects[i]->death_chck()) {
			 fl_all_dead = 0;
		 }
	 }


	 if ((lokal.hp <= 0) && (fl_all_dead))
		 return 1;
	 else
		 return 0;
 }
 //#////////end///////////#//