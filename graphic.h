#pragma once
#include "App.h"
#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }
class grph_obj { 
	//#// wspolne wskazniki dla wszystkich obiektow graficznych///#//
protected:
	
	static HWND hwnd;
	static ID3D11Device* p_Device;   //Uradzenie do tworzenia
	static ID3D11DeviceContext* p_DeviceContext; //Urzadzenie do rysowania
	static IDXGISwapChain* p_SwapChain;  //Zmiana ekranow
	static ID3D11RenderTargetView* p_TargetView;               //Tylny bufer
   
	static ID3D11DepthStencilView* g_pDepthStencilView;        // Bufer glebok
	static ID3D11Texture2D*        g_pDepthStencil;             // Tekstura dla bufora glebok

	static XMMATRIX stat_m_view;

	/////#start_of_var#/////
	ID3D11VertexShader*     g_pVertexShader;            // Shader wierzcholko
	ID3D11PixelShader*      g_pPixelShader;             // Pixel shader
	ID3D11InputLayout*      g_pVertexLayout;             // Format wierzcholkow
	ID3D11Buffer*           g_pVertexBuffer;             // Bufer wierzcholkow
	ID3D11Buffer*           g_pIndexBuffer;              // Bufer indeksow wierzcholkow
	ID3D11Buffer*           g_pConstantBuffer;           // Const bufer dla shaderow
	ID3D11DepthStencilState* DSLessEqual;                // typ bufora glebokosci
	ID3D11RasterizerState*  WireFrame;                    // tryb rysowania
	ID3D11BlendState* Transparency;
	ID3D11ShaderResourceView* CubesTexture;
	ID3D11SamplerState* CubesTexSamplerState;
	/////#ilosc_punktow_do_rys#////
	UINT indx_nmb;
	////#domyslny_kier_modelu#/////
	XMVECTOR def_kier;
	/////#macierze_do_shader#/////
	wchar_t* tekstura;
	wchar_t* Shader;
	/////#macierze_do_shader#/////
	XMMATRIX m_world;
	XMMATRIX m_view;
	XMMATRIX m_projection;
	/////#macierze_do_modyfikacji#/////
	XMMATRIX m_scale;
	XMMATRIX m_rotate;
	XMMATRIX m_translate;
	/////#end_of_var#/////
public:
	static ID3D11DeviceContext* get_device_context() {
		return p_DeviceContext;
	}
	static IDXGISwapChain* get_SwapChain() {
		return p_SwapChain;
	}
	static ID3D11DepthStencilView* get_DepthBuff() {
		return g_pDepthStencilView;
	}
	static ID3D11RenderTargetView* get_TargView() {
		return p_TargetView;
	}
	static void cng_View(XMMATRIX& view) {
		stat_m_view = view;
	}


	static HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	static HRESULT InitDirectx(HINSTANCE hinstance, HWND hwnd);
	static void release();

protected:
	virtual HRESULT InitGeometry() = 0;
	virtual HRESULT InitMatrixes() = 0;
	virtual HRESULT InitStates()=0;
	/////////////////////////////
public:
	virtual void crt_texture(wchar_t*)=0;
	virtual void UpdtViewMatrix(XMMATRIX &Cm_view)=0;
	virtual void UpdtWorldMatrix() = 0;
	virtual void set_pos(XMVECTOR& _pos)=0;
	virtual void rotate(XMVECTOR &_kier, XMVECTOR &_def_kierr = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f))=0;
	virtual void scale_xyz(float x, float y, float z)=0;
	/////////////////////////////
	virtual void change_pendz()=0;
	virtual void Render() = 0;

};



//////////////////////////////////////////////
/////#//Objekt_o_stndrt_world_position//#//////
class std_bchv_model :public grph_obj {
public:
	virtual void set_pos(XMVECTOR& _pos);
	virtual void rotate(XMVECTOR &_kier, XMVECTOR &_def_kierr = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	virtual void UpdtViewMatrix(XMMATRIX &Cm_view);
	virtual void UpdtWorldMatrix();
	virtual void scale_xyz(float x, float y, float z);
	virtual void crt_texture(wchar_t*);
    std_bchv_model() {
		grph_obj::m_translate = XMMatrixIdentity();
		grph_obj::m_rotate = XMMatrixIdentity();
		grph_obj::m_scale = XMMatrixIdentity();

	}
};


/////////////////////////////////
/////#//Prosta_geometria//#//////
class prim : public std_bchv_model {
protected:
	
//////#//same_func_for_all_prim//#/////
	HRESULT InitMatrixes();
	HRESULT InitStates();
public:
	void change_pendz();
	void Render();
	prim(){
		InitStates();
		InitMatrixes();
	}
};



/////////////////////////////////////////////////
/////#//Prostopadloscian_graphics_models//#//////
class kwdrt:public prim{
	HRESULT InitGeometry();
public:
	kwdrt(wchar_t* a=L"assets/moon.jpg") :prim() {
		crt_texture(a);
		InitGeometry();
	}
};


////////////////////////////////////////////////
//////#//sphere_graphics_models//#/////////////

class sphere :public prim {
	HRESULT InitGeometry();
	void CreateSphere(float, UINT);
public:
	sphere(wchar_t* a = L"assets/sun1.jpg") :prim() {
		crt_texture(a);
		InitGeometry();
	}
};



//////#//graphics_from_.obj_models//#//////////
class loaded_obj_model : public std_bchv_model {
protected:
	/////#//variable//#/////
	
	std::string inputfile;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	XMVECTOR dflt_kier;
	/////#//methods//#/////
	virtual HRESULT InitMatrixes();
	virtual HRESULT InitStates();
	////////////////////////
	virtual void load_obj(std::string);
	virtual void change_pendz();

	loaded_obj_model(std::string a) {
		InitStates();
		InitMatrixes();
		load_obj(a);
	}
	
	/////#//virtual_methods//#/////
	virtual HRESULT crt_geom()=0;

};
////////#//single_bufor_.obj_models//#//////////
class obj_model_sb : public loaded_obj_model {
	HRESULT crt_geom();
	HRESULT InitGeometry();
public:
	void Render();
	obj_model_sb(wchar_t* b = L"assets/ex1.jpg", std::string a = "assets/shp1.obj") :loaded_obj_model(a) {
		crt_texture(b);
		InitGeometry();
	}

};




