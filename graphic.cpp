#pragma once
#include "graphic.h"

//#/////////////////////////////////////////#////
/////////////#//Graphic_object/#/////////////////
//#/////////////////////////////////////////#////

HWND grph_obj::hwnd;
XMMATRIX grph_obj::stat_m_view;
ID3D11Device* grph_obj::p_Device = NULL;   //Uradzenie do tworzenia
ID3D11DeviceContext* grph_obj::p_DeviceContext = NULL; //Urzadzenie do rysowania
IDXGISwapChain* grph_obj::p_SwapChain = NULL;  //Zmiana ekranow
ID3D11RenderTargetView* grph_obj::p_TargetView = NULL;               //Tylny bufer
ID3D11DepthStencilView* grph_obj::g_pDepthStencilView = NULL;        // Bufer glebok
ID3D11Texture2D* grph_obj::g_pDepthStencil = NULL;             // Tekstura dla bufora glebok

HRESULT grph_obj::InitDirectx(HINSTANCE hinstance, HWND _hwnd) {
	hwnd = _hwnd;
	D3D_DRIVER_TYPE  p_DriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL p_DriverLvl = D3D_FEATURE_LEVEL_11_0;
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	UINT createDeviceFlags = 0;
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		p_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, p_DriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &p_SwapChain, &p_Device, &p_DriverLvl, &p_DeviceContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)) return hr;


	ID3D11Texture2D* pBackBuffer = NULL;
	hr = p_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return hr;


	hr = p_Device->CreateRenderTargetView(pBackBuffer, NULL, &p_TargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) return hr;


	D3D11_TEXTURE2D_DESC descDepth;    
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;      
	descDepth.Height = height;    
	descDepth.MipLevels = 1;         
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;        
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	////////


	
	hr = p_Device->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	if (FAILED(hr)) return hr;

	
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;            
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;         
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	//////////////////////////////



	
	hr = p_Device->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr)) return hr;


	p_DeviceContext->OMSetRenderTargets(1, &p_TargetView, g_pDepthStencilView);


	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	p_DeviceContext->RSSetViewports(1, &vp);


	return S_OK;


}

HRESULT grph_obj::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (FAILED(hr)) {
		MessageBox(NULL, L"Ooops", L"Nie udalo sie skompilowac shader", MB_OK);
	}


	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void  grph_obj::release() {
	// Odlaczamy urz rysowania
	p_DeviceContext->ClearState();
	// Usuwamy wszystko
	SAFE_RELEASE(g_pDepthStencil);
	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(p_TargetView);
	SAFE_RELEASE(p_SwapChain);
	SAFE_RELEASE(p_DeviceContext);
	SAFE_RELEASE(p_Device);
}



//###################################################################//
//###################################################################//
//###################################################################//
//###################################################################//



//#/////////////////////////////////////////#////
//////////#//Standart_bhv_models/#///////////////
//#/////////////////////////////////////////#////



//#//Operacje na macierzach//#//
void std_bchv_model::set_pos(XMVECTOR& _pos) {

	m_translate = XMMatrixTranslationFromVector(_pos);
}

void std_bchv_model::rotate(XMVECTOR &_kier, XMVECTOR &_def_kier) {

	XMFLOAT3 cmp;
	XMStoreFloat3(&cmp, XMVectorEqual(_kier, _def_kier));
	if ((cmp.x == 0) || (cmp.y == 0) || (cmp.z == 0)) {
		XMVECTOR rot_vect = XMVector3AngleBetweenVectors(_def_kier, _kier);
		XMFLOAT3 angle;
		XMStoreFloat3(&angle, rot_vect);
		XMVECTOR os_obr = XMVector3Cross(_def_kier, _kier);
		if ((angle.x != 0.0f) && (angle.x != XM_PI))
			m_rotate = XMMatrixRotationAxis(os_obr, angle.x);
	}
}

void std_bchv_model::scale_xyz(float x, float y, float z) {
	m_scale = XMMatrixScaling(x, y, z);
}

void std_bchv_model::UpdtViewMatrix(XMMATRIX &Cm_view) {
	m_view = Cm_view;
}

void std_bchv_model::UpdtWorldMatrix() {
	m_world = m_scale*m_rotate*m_translate;
	
}
//#//Tworzenie nowego bufora tekstury//#//
void std_bchv_model::crt_texture(wchar_t* ss) {
	tekstura = ss;
	HRESULT hr = S_OK;
	hr = CreateWICTextureFromFile(p_Device, tekstura, NULL, &CubesTexture);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Nie znalieziono tekstury.", L"Error", MB_OK);
	}

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = p_Device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);
}
