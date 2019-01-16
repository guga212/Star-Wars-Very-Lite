#pragma once
#include "graphic.h"
//#/////////////////////////////////////////#//
////////////#//loaded_obj_model;//#////////////
//#/////////////////////////////////////////#//
//#//Load_obj_do_vectora//#//
void loaded_obj_model::load_obj(std::string aa) {
	std::string err;
	bool ret = tinyobj::LoadObj(shapes, materials, err, aa.c_str());
	if (ret == 0) {
		MessageBox(NULL, L"Obj nie znalieziono", L"ERROE .obj", 1);
	}
}

//#//Inicjalizacja_Stanu&&ConstBuforMatrx//#//
HRESULT loaded_obj_model::InitStates() {
	HRESULT hr S_OK;
	D3D11_BUFFER_DESC bd;
	// Const buf dla shaderu
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	hr = p_Device->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
	if (FAILED(hr)) return hr;

	/////////////////////////////////////
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	//wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.FillMode = D3D11_FILL_SOLID;
	//wfdesc.CullMode = D3D11_CULL_NONE;
	hr = p_Device->CreateRasterizerState(&wfdesc, &WireFrame);
	p_DeviceContext->RSSetState(WireFrame);  //ustawienia trybu wyswietk=lienia
											 /////////////////////////
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	//dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	p_Device->CreateDepthStencilState(&dssDesc, &DSLessEqual);
	//////////////////
	return hr;
}
//#//Inicjalizacja_Macierzy//#//
HRESULT loaded_obj_model::InitMatrixes()
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;     // szerokosc
	UINT height = rc.bottom - rc.top;   // wysokosc


	m_world = XMMatrixIdentity();


	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view = XMMatrixLookAtLH(Eye, At, Up);


	m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 1000.0f);

	return S_OK;
}

void loaded_obj_model::change_pendz() {
	//#//vertex&index_bufors_insert//#//
	UINT stride = sizeof(SimpleVertex2);
	UINT offset = 0;
	p_DeviceContext->IASetInputLayout(g_pVertexLayout);
	p_DeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	p_DeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	p_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//#//tekstury_insert//#//
	p_DeviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);
	p_DeviceContext->PSSetShaderResources(0, 1, &CubesTexture);
	//#//raster&depth&blend_state_insert//#//
	p_DeviceContext->OMSetBlendState(0, 0, 0xffffffff);
	p_DeviceContext->RSSetState(WireFrame);
	p_DeviceContext->OMSetDepthStencilState(DSLessEqual, 0);

	//#//shadery_insert//#//
	p_DeviceContext->VSSetShader(g_pVertexShader, NULL, 0);
	p_DeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	p_DeviceContext->PSSetShader(g_pPixelShader, NULL, 0);

	//#//view_sync//#//
	m_view = stat_m_view;

	//#//matrixes_insert//#//
	UpdtWorldMatrix();
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(m_world);
	cb.mView = XMMatrixTranspose(m_view);
	cb.mProjection = XMMatrixTranspose(m_projection);
	p_DeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
}



//#/////////////////////////////////////////#//
/////////#//obj_model_single_bufor;//#/////////
//#/////////////////////////////////////////#//

HRESULT obj_model_sb::crt_geom() {

	HRESULT hr = S_OK;
	float pi = 3.14159265359f;
	int sizeV = 0;
	for (unsigned int i = 0; i < shapes.size(); i++) {
		sizeV += shapes[i].mesh.positions.size();
	}
	sizeV /= 3; // wszystkie vertichies wszystkich shape'ow dzielony przez 3
	SimpleVertex2* vertices = new SimpleVertex2[sizeV];
	int j = 0;
	int i1 = 0;
	for (int i = 0; i < sizeV; i++, i1++) {
		if (i1 == shapes[j].mesh.positions.size() / 3) {
			j++;
			i1 = 0;
		}
		vertices[i].Pos.x = float(shapes[j].mesh.positions[3 * i1 + 0]);
		vertices[i].Pos.y = float(shapes[j].mesh.positions[3 * i1 + 1]);
		vertices[i].Pos.z = float(shapes[j].mesh.positions[3 * i1 + 2]);

		vertices[i].TexCoord.x = 0.5 + (atan2(shapes[j].mesh.normals[3 * i1 + 2], shapes[j].mesh.normals[3 * i1]) / (2 * pi));
		vertices[i].TexCoord.y = 0.5f - (asin(shapes[j].mesh.normals[3 * i1 + 1]) / pi);
		//vertices[i].TexCoord.y = rand() % 11 / 10;
		//vertices[i].TexCoord.x = rand() % 11 / 10;
	}






	////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC bd;  // Bufer wierzcholkow
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex2) * sizeV;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;         //info struktura  
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	//Tworzymy bufer wierzchokow
	hr = p_Device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr)) return hr;
	// Bufer indeksow:
	// Tabela z danymi
	//////////////////////////////////////////////////////////////////////
	int sizeI = 0;
	for (unsigned int i = 0; i < shapes.size(); i++) {
		sizeI += shapes[i].mesh.indices.size();
	}
	indx_nmb = sizeI;

	DWORD* indices = new DWORD[sizeI];


	j = 0;
	i1 = 0;
	int przes = 0;
	for (int i = 0; i < sizeI; ++i, ++i1) {

		if (i1 == shapes[j].mesh.indices.size()) {
			przes += shapes[j].mesh.positions.size() / 3;
			++j;
			i1 = 0;
		}

		indices[i] = shapes[j].mesh.indices[i1] + przes;
	}


	/////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;            // Bufer wierzcholkow
	bd.ByteWidth = sizeof(DWORD) * sizeI;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	p_Device->CreateBuffer(&bd, &iinitData, &g_pIndexBuffer);
	if (FAILED(hr)) return hr;

	//////////////////////////////////
	return hr;
}

HRESULT obj_model_sb::InitGeometry() {
	HRESULT hr = S_OK;

	// Shader wierzcholkow
	ID3DBlob* pVSBlob = NULL; //Miejsce w pamieci
	hr = CompileShaderFromFile(L"shaders/MainShader.fx", "VS2", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Nie znalieziona shader", L"Error", MB_OK);
		return hr;
	}

	// Tworzymy shader
	hr = p_Device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Wzor szablonu wprowadzenia wierzcholkow
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Tworzenie szablonu
	hr = p_Device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr)) return hr;

	// Dolaczamy szablon
	p_DeviceContext->IASetInputLayout(g_pVertexLayout);

	// Komppilacja pixel shader z pliku
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(L"shaders/MainShader.fx", "PS2", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Nie znalieziona shader.", L"Error", MB_OK);
		return hr;
	}
	hr = p_Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;
	crt_geom();
	return hr;
}

void obj_model_sb::Render()
{
	change_pendz();
	p_DeviceContext->DrawIndexed(indx_nmb, 0, 0);
}

