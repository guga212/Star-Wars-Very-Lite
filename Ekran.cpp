#include"Ekran.h"
#include<thread>
Ekran::Ekran(HINSTANCE _hinstance,HWND _hwnd) {

	hinstance = _hinstance;
	hwnd = _hwnd;


	mng = new res_mng(hwnd,hinstance);
	sky = new sphere(L"assets/stars5.jpg");
	sky->scale_xyz(200.0f, 200.0f, 200.0f);

	
	

	mng->add_sky(sky);



	std::thread thrTab[6];
	

		thrTab[0] = std::thread(&res_mng::add_model, mng, new obj_model_sb(L"assets/ex12.jpg", "assets/X-wing3.obj"), "czarny_st");
		thrTab[1] = std::thread(&res_mng::add_model, mng, new obj_model_sb(L"assets/ex6.jpg", "assets/X-wing3.obj"), "bialy_st");
		thrTab[2] = std::thread(&res_mng::add_model, mng, new obj_model_sb(L"assets/ex12.jpg", "assets/sok3.obj"), "czarny_sok");
		thrTab[3] = std::thread(&res_mng::add_model, mng, new obj_model_sb(L"assets/ex6.jpg", "assets/sok3.obj"), "bialy_sok");
		thrTab[4] = std::thread(&res_mng::add_model, mng, new obj_model_sb(L"assets/ex12.jpg", "assets/imp1.obj"), "czarny_imp");
		thrTab[5] = std::thread(&res_mng::add_model, mng, new obj_model_sb(L"assets/ex6.jpg", "assets/imp1.obj"), "bialy_imp");


	for (int i = 0; i < 6; ++i) {

		if (thrTab[i].joinable())
			thrTab[i].join();
	}

	/*
	mng->add_model(new obj_model_sb(L"ex12.jpg","X-wing3.obj"),"czarny_st");
	mng->add_model(new obj_model_sb(L"ex6.jpg", "X-wing3.obj"), "bialy_st");
	mng->add_model(new obj_model_sb(L"ex12.jpg", "sok3.obj"), "czarny_sok");
	mng->add_model(new obj_model_sb(L"ex6.jpg", "sok3.obj"), "bialy_sok");
	mng->add_model(new obj_model_sb(L"ex12.jpg", "imp1.obj"), "czarny_imp");
	mng->add_model(new obj_model_sb(L"ex6.jpg", "imp1.obj"), "bialy_imp");
	*/


/*
	mng->add_model(new obj_model_sb(L"ex12.jpg"), "czarny_st");
	mng->add_model(new obj_model_sb(L"ex6.jpg"), "bialy_st");
	mng->add_model(new obj_model_sb(L"ex12.jpg"), "czarny_sok");
	mng->add_model(new obj_model_sb(L"ex6.jpg"), "bialy_sok");
	mng->add_model(new obj_model_sb(L"ex12.jpg"), "czarny_imp");
	mng->add_model(new obj_model_sb(L"ex6.jpg"), "bialy_imp");
*/


	mng->add_model(new obj_model_sb(L"assets/ex1.jpg", "assets/rocket3.obj"), "missle");
	mng->add_model(new sphere(), "pocisk");
	mng->add_model(new sphere(L"assets/plasma1.jpg"), "wybuch");

	for (int i = 0; i < 6; i++) {
		if (i<0.5 * 6)
			
			mng->add_obj(new ship_sml());
		else
			mng->add_obj(new ship_sml(1));
	}

	for (int i = 0; i < 4; i++) {
		if(i<0.5*4)
			mng->add_obj(new ship_middle());
		else
			mng->add_obj(new ship_middle(1));
	}
	
	for (int i = 0; i < 2; i++) {
		if (i<0.5 * 2)
			mng->add_obj(new ship_big());
		else
			mng->add_obj(new ship_big(1));
	}





	mng->add_zest("Sml_shp_ally", "czarny_st");
	mng->add_zest("Sml_shp_enmy", "bialy_st");

	mng->add_zest("Middle_shp_ally", "czarny_sok");
	mng->add_zest("Middle_shp_enmy", "bialy_sok");


	mng->add_zest("Big_shp_ally", "czarny_imp");
	mng->add_zest("Big_shp_enmy", "bialy_imp");

	mng->add_zest("SimpleBullet", "pocisk");
	mng->add_zest("IntBullet", "missle");
	mng->add_zest("boom", "wybuch");


	
	
}

void Ekran::DrawScene() {

	mng->draw();
	
}


void Ekran::Updt() {
	
	mng->updt_elmnts();
	
}
