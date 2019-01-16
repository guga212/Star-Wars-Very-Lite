#pragma once
#include "mng.h"
//#//////////////////////////////#//
////////#//model_manger//#/////////
void grph_mng::prp_rndr() {
	float bgColor[] = { 0.6f,0.6f,0.6f,1.0f };
	grph_obj::get_device_context()->ClearRenderTargetView(grph_obj::get_TargView(), bgColor);
	grph_obj::get_device_context()->ClearDepthStencilView(grph_obj::get_DepthBuff(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void grph_mng::snd_toscr() {
	grph_obj::get_SwapChain()->Present(0, 0);
}
void grph_mng::add_model(grph_obj* x, std::string _opis){
	opis.push_back(_opis);
	models.push_back(x);
}
void grph_mng::updt_view() {
	cam->Updt();

	grph_obj::cng_View(cam->get_view());
}
//#//////////////////////////////#//
////////#//object_manager//#/////////

void ship_mng::updt_obj() {
	for (int i = 0; i < flotilia.size(); i++) {
		flotilia[i]->updt();
		if (flotilia[i]->death_chck() == 1) {
			delete flotilia[i];
			flotilia[i] = NULL;
		}
	}

}

void ship_mng::rem_garb() {
	std::vector<ship*>::iterator it;
	for (it = flotilia.begin(); it != flotilia.end(); )
	{
	
		if ((*it) == NULL)
			it = flotilia.erase(it);
		else
			++it;
	}
}
///////////////////////////////////
//#//////////////////////////////#//
////////#//res_manager//#/////////

void res_mng::draw() {
//	static int f = 0;
	//////////////////////////
	prp_rndr();
	updt_view();


	//mozno skomentowac
	/*
	if ((flotilia[f] != NULL)&&(flotilia.size() > 0)) {
		cam->Updt_pos(flotilia[f]->get_pos() + 5 * flotilia[f]->get_kier());
	}
	else {
		if (f < flotilia.size())
			++f;
		else
			--f;
		
	}
//end	
*/
	//////////////////////////
	sky->set_pos(cam->get_cam_pos());
	sky->Render();
	//////////////////////////

	for (int i = 0; i < flotilia.size(); i++) {
		grph_obj* model = find_model(flotilia[i]->get_opis());
		model->scale_xyz(flotilia[i]->get_size(), flotilia[i]->get_size(), flotilia[i]->get_size());
		model->rotate(flotilia[i]->get_kier());
		model->set_pos(flotilia[i]->get_pos());
		model->Render();


		for (int j = 0; j < flotilia[i]->crtd_objects.size(); j++) {

			grph_obj* model1 = find_model((flotilia[i]->crtd_objects[j]->get_opis()));
			model1->scale_xyz(flotilia[i]->crtd_objects[j]->get_size(), flotilia[i]->crtd_objects[j]->get_size(), flotilia[i]->crtd_objects[j]->get_size());
			model1->rotate(flotilia[i]->crtd_objects[j]->get_kier());
			model1->set_pos(flotilia[i]->crtd_objects[j]->get_pos());
			model1->Render();

		}

	}



	///////////////////////////////////////
	snd_toscr();

}
///////////////////////////////////
grph_obj* res_mng::find_model(std::string opis_obj) {
	int n1 = -1;
	int n2 = -1;
	for (int i = 0; i < tego_obiek.size(); i++) {
		if (tego_obiek[i] == opis_obj) {
			n1 = i;
			break;
		}
	}

	for (int i = 0; i < opis.size(); i++) {
		if (opis_mod_dla_obiek[n1] == opis[i]) {
			n2 = i;
			break;
		}
	}

	if (n2 < models.size() && n2 >= 0)
		return models[n2];
	else {
		MessageBox(NULL, L"Error", L"nie znaleziono modelu do opisu", 1);
		return NULL;
	}
}
