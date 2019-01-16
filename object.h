#pragma once
#include"App.h"

//#////////////////////////////#//
//////#//objekty_swiata//#////////
//#////////////////////////////#//
enum stany { vac, rnd, flw_tr, flw_fr, flw_enmy, run, anticoll };
struct inf_ship_obj {
	stany stan;
	float size;
	int hp;
	int hp_max;
	int id;
	int dmg;
	bool team;
	float v;
	float v_max;
	float w;
	float w_max;
	XMVECTOR pos;
	XMVECTOR kier;
	XMVECTOR TARGpos;
};
struct inf_bullet_obj {
	int hp;
	int hp_max;
	int id_cr;
	int dmg;
	bool team;
	float v;
	float v_max;
	float w;
	float w_max;
	XMVECTOR pos;
	XMVECTOR start_pos;
	XMVECTOR kier;
	XMVECTOR TARGpos;
};
//******************************//

class object {
protected:
////#//variables//#//////
	XMVECTOR pos;
	XMVECTOR kier;
	float size;
	std::string opis;
////#//methods//#//////
public:
	virtual std::string get_opis() = 0;
	virtual void set_pos(XMVECTOR)=0;
	virtual  XMVECTOR get_pos()=0;
	virtual void set_kier(XMVECTOR) = 0;
	virtual  XMVECTOR get_kier() = 0;
	virtual float get_size()=0;
	virtual void set_size(float) = 0;
	virtual void updt() = 0;
	virtual bool death_chck() = 0;
	
};
//#////////////////////////////////#//
//////#//podstawowy_typ_obj//#////////
class stdrt_object:public object {
public:
	std::string get_opis();
	void set_pos(XMVECTOR);
	XMVECTOR get_pos() ;
	void set_kier(XMVECTOR);
	XMVECTOR get_kier() ;
	float get_size();
	void set_size(float);
};
//#/////////////////////////////////#//
//////////#//aktywne_obj//#///////////
//#///////////////////////////////#//
class act_obj :public stdrt_object {

public:
	std::vector<object*> crtd_objects;
protected:
	bool destroyed;
	
	virtual void updt_elmnts() = 0;
};

//#/////////////////////////////////#//
//////#//podstawowe_pociski//#////////
class bullet :public stdrt_object{
protected:
	inf_bullet_obj lokal;
public:
	virtual void chck_pos()=0;
	virtual ~bullet() {

	}
};

class bullet_simple :public bullet{
	void chck_pos();
public:
	bullet_simple(XMVECTOR _kier, XMVECTOR _pos, int _id, bool _team, int _dmg) {
		size = 0.06f;
		opis = "SimpleBullet";
		lokal.start_pos =pos= _pos;
		lokal.dmg = _dmg;
		lokal.id_cr = _id;
		lokal.team = _team;
		lokal.hp = 1;
		lokal.kier =kier= _kier;
		lokal.pos = _pos + (XMVector3Normalize(_kier));
		lokal.v = 16.6f;
	}
	bool death_chck();
	void updt_elmnts() {

	}
	void updt();

	~bullet_simple(){

	}

};

class bullet_int :public bullet {
	inf_ship_obj* target;
	int n_count;
	float w;
	void chck_pos();
public:
	bullet_int(XMVECTOR _kier, inf_ship_obj* _target, XMVECTOR _pos, int _id, bool _team, int _dmg) {
		size = 0.085f;
		target = _target;
		n_count = 0;
		lokal.w_max = 50 * (float)XM_PI * 0.0125f;
		lokal.w = lokal.w_max;
		opis = "IntBullet";
		lokal.start_pos = pos = _pos;
		lokal.dmg = _dmg;
		lokal.id_cr = _id;
		lokal.team = _team;
		lokal.hp = 1;
		lokal.kier = kier = _kier;
		lokal.pos = _pos + (XMVector3Normalize(_kier));
		lokal.v = 10.6f;
	}
	bool death_chck();
	void updt_elmnts() {

	}
	void updt();

	~bullet_int() {

	}
};


//#////////////////////////////////#//
//////#//podstawowe_statki//#////////
class ship :public act_obj {
	
	friend bullet_simple;
	friend bullet_int;
protected:
	UINT n_alg=0;
	UINT n_safe=0;
//#///////static_var////////#//
	static std::vector<inf_ship_obj*> allshp_pos;
//#//////////var///////////#//
timer* clocks[5];
inf_ship_obj lokal;
//#////////methods////////#//
virtual void flw_targ() = 0;
virtual void upd_stat() = 0;
virtual void col_det() = 0;
//#////////bchv_methods////////#//
virtual void anti_coll() = 0;
virtual void rnd_bhv() = 0;
virtual void flw_enm() = 0;
virtual void shoot() = 0;
virtual void go_away() = 0;
//#////////destructor////////#//
public:
	ship() {
		UINT n_alg=0;
		UINT n_safe=0;
	}
virtual ~ship() {
	//add
}
};

class ship_sml :public ship {
	static int n_sml;
	//#////////methods////////#//
	void flw_targ();
	void updt_elmnts();
	void updt_boom();
	void col_det();
	void upd_stat();
	//#////////bchv_methods////////#//
	void anti_coll();
	void rnd_bhv();
	void flw_enm();
	void shoot();
	void go_away();
	//#////////construct////////#//
public:
	ship_sml(bool _team = 0) {
		
		size = 1.0f;
		lokal.size = size;
		destroyed = 0;
		lokal.dmg = 12;
		lokal.hp_max = 156;
		lokal.hp = lokal.hp_max;
		lokal.id = allshp_pos.size();
		lokal.team = _team;
		lokal.w_max = 5 * (float)XM_PI * 0.0125f;
		lokal.w = lokal.w_max;
		lokal.v_max = 1.5f;
		lokal.v = lokal.v_max;
		lokal.stan = vac;
		if (_team == 0) {
			opis = "Sml_shp_ally";
			lokal.pos = pos = XMVectorSet(-10 * n_sml/2, 0.0f, 0.0f, 0.0f);
			lokal.TARGpos = XMVectorSet(-10 * n_sml/2, 0.0f, 50.0f, 0.0f);
			lokal.kier = kier = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		}
		else {
			opis = "Sml_shp_enmy";
			lokal.pos = pos = XMVectorSet(10 * n_sml/2, 0, 140.0f, 0.0f);
			lokal.TARGpos = XMVectorSet(10 * n_sml/2, 0.0f, 50.0f, 0.0f);
			lokal.kier = kier = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		}
		n_sml++;
		allshp_pos.push_back(&lokal);
		clocks[0] = new timer();
		clocks[1] = new timer(1);
	}
	void updt();
	bool death_chck();
};

class ship_middle :public ship {
	//#////////methods////////#//
	static int n_middle;
	void flw_targ();
	void updt_elmnts();
	void upd_stat();
	void col_det();
	void updt_boom();
	//#////////bchv_methods////////#//
	void anti_coll();
	void rnd_bhv();
	void flw_enm();
	void shoot();
	void go_away();
	//#////////construct////////#//
public:
	ship_middle(bool _team = 0) {
	
		size = 1.9f;
		lokal.size = size;
		destroyed = 0;
		lokal.dmg = 5;
		lokal.hp_max = 300;
		lokal.hp = lokal.hp_max;
		lokal.id = allshp_pos.size();
		lokal.team = _team;
		lokal.w_max = 3 * (float)XM_PI * 0.0125f;
		lokal.w = lokal.w_max;
		lokal.v_max = 1.6f;
		lokal.v = lokal.v_max;
		lokal.stan = vac;
		if (_team == 0) {
			opis = "Middle_shp_ally";
			lokal.pos = pos = XMVectorSet(-15* n_middle/2, 20 , 0.0f, 0.0f);
			lokal.TARGpos = XMVectorSet(-15 * n_middle/2, 20.0f, 50.0f, 0.0f);
			lokal.kier = kier = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		}
		else {
			opis = "Middle_shp_enmy";
			lokal.pos = pos = XMVectorSet(15 * n_middle/2, 20,  140.0f, 0.0f);
			lokal.TARGpos = XMVectorSet(15 * n_middle/2, 20.0f, 50.0f, 0.0f);
			lokal.kier = kier = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		}
		n_middle++;
		allshp_pos.push_back(&lokal);
		clocks[0] = new timer();
		clocks[1] = new timer(1);
	}
	void updt();
	bool death_chck();
};

class ship_big :public ship {
	static int n_big;
	std::vector<UINT> targets;
	UINT n; //folowed enmy number
	//#////////methods////////#//
	void flw_targ();
	void updt_elmnts();
	void upd_stat();
	void col_det();
	void updt_boom();
	//#////////bchv_methods////////#//
	void anti_coll();
	void rnd_bhv();
	void flw_enm();
	void shoot();
	void go_away();
	//#////////construct////////#//
public:
	ship_big(bool _team = 0) {
		size = 3.9f;
		lokal.size = size;
		destroyed = 0;
		lokal.dmg = 15;
		lokal.hp_max = 750;
		lokal.hp = lokal.hp_max;
		lokal.id = allshp_pos.size();
		lokal.team = _team;
		lokal.w_max = 0.24 * (float)XM_PI * 0.0125f;
		lokal.w = lokal.w_max;
		lokal.v_max = 1.7f;
		lokal.v = lokal.v_max;
	
		lokal.stan = vac;
		if (_team == 0) {
			opis = "Big_shp_ally";
			lokal.kier = kier = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			lokal.pos = pos =XMVectorSet(20 * (n_big / 2) + 20, 35.0f+10 * n_big, 0.0f, 0.0f);
			lokal.TARGpos = XMVectorSet(-20 * (n_big/2), 40.0f, 50.0f, 0.0f);
			
		}
		else {
			opis = "Big_shp_enmy";
			lokal.kier = kier = XMVectorSet(0.1f, 0.1f, -0.8f, 0.0f);
			lokal.pos = pos = 0.5*size*XMVectorSet(20 * (n_big / 2)+20, 35.0f+10*n_big/2, 52.0f, 0.0f);
			lokal.TARGpos = XMVectorSet(20 * int(n_big/2), 40.0f, 65.0f, 0.0f);
			

		}
		n_big++;
		allshp_pos.push_back(&lokal);
		clocks[0] = new timer();
		clocks[1] = new timer(1);
	}
	void updt();
	bool death_chck();
};



//#/////////////////////////////#//
//////#//dekoracyjne_obj//#////////
//#////////////////////////////#//

class explose :public stdrt_object {

	int n_count;
	int rate;
	int life_time;
	
	
public:
	explose(XMVECTOR _pos,float _rate, int _life_time) {
		opis = "boom";
		size = 0.1;
		pos = _pos;
	
		rate = _rate;
		life_time = _life_time;
		n_count = 0;
	}

	void updt() {
		
			
		kier = XMVector3Normalize(XMVectorSet(rand() % 11, rand() % 11, rand() % 11, 0.0f));
		size +=(rate-size)/life_time;
		n_count++;
	}

	bool death_chck()  {
		if (n_count>life_time) {
			return 1;
		}
		else return 0;
	}

};










