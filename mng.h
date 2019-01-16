#pragma once
#include "graphic.h"
#include "object.h"
#include "camera.h"
class grph_mng {
protected:
	camera* cam;
	std::vector<grph_obj*> models;
	std::vector<std::string> opis;
	void prp_rndr();
	void snd_toscr();
	void updt_view();
public:

	void add_model(grph_obj*,std::string _opis);


	grph_mng(HWND hwnd,HINSTANCE _hinstance) {
		grph_obj::InitDirectx(_hinstance, hwnd);
		cam = new camera(hwnd);
	}

	virtual ~grph_mng() {
		grph_obj::release();
	}
};

class ship_mng {
protected:
	std::vector<ship*> flotilia;
	
	void updt_obj();
	void rem_garb();
public:
	void add_obj(ship* now) {
		flotilia.push_back(now);
	}

};


class res_mng :public ship_mng, public grph_mng {
	grph_obj* sky;
	std::vector <std::string> opis_mod_dla_obiek;
	std::vector <std::string> tego_obiek;


	grph_obj* find_model(std::string opis_obj);
public:
	void add_zest(std::string opis_obj, std::string opis_modelu) {
		opis_mod_dla_obiek.push_back(opis_modelu);
		tego_obiek.push_back(opis_obj);
	}
	void add_sky(grph_obj* _sky) {
		sky = _sky;
	}
	res_mng(HWND _hwnd, HINSTANCE _hinstance) :grph_mng(_hwnd,_hinstance), ship_mng() {}
	void updt_elmnts() {
		updt_obj();
		rem_garb();
	}
	void draw();


};