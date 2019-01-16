#include "graphic.h"

//#/////////////////////////////////////////#////
/////////#//Geometrical_primitives/#/////////////
//#/////////////////////////////////////////#////

//#//Inicjalizacja_Stanu&&ConstBuforMatrx//#//
HRESULT prim::InitStates() {
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
	//	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
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
HRESULT prim::InitMatrixes()
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;     // szerokosc
	UINT height = rc.bottom - rc.top;   // wysokosc


	m_world = XMMatrixIdentity();


	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_view =XMMatrixLookAtLH(Eye, At, Up);
	m_projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 1000.0f);

	return S_OK;
}

void prim::change_pendz() {
	//#//vertex&index_bufors_insert//#//
	UINT stride = sizeof(SimpleVertex2);
	UINT offset = 0;
	p_DeviceContext->IASetInputLayout(g_pVertexLayout);
	p_DeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	p_DeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
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

void prim::Render()
{
	change_pendz();
	p_DeviceContext->DrawIndexed(indx_nmb, 0, 0);
}


//#/////////////////////////////////////////#////
/////////#//Prostopadloscian/#///////////////////
//#/////////////////////////////////////////#////

HRESULT kwdrt::InitGeometry()
{
	HRESULT hr = S_OK;

	// Shader wierzcholkow
	ID3DBlob* pVSBlob = NULL; //Miejsce w pamieci
	hr = CompileShaderFromFile(L"shaders/MainShader.fx", "VS2", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Nie znalieziona shader1", L"Error", MB_OK);
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

	// Tworzymy pixel shader
	hr = p_Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr)) return hr;


	SimpleVertex2 v[] =
	{
		// Front Face
		SimpleVertex2(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		SimpleVertex2(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		SimpleVertex2(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		SimpleVertex2(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Back Face
		SimpleVertex2(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		SimpleVertex2(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		SimpleVertex2(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		SimpleVertex2(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		SimpleVertex2(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		SimpleVertex2(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		SimpleVertex2(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		SimpleVertex2(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		// Bottom Face
		SimpleVertex2(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		SimpleVertex2(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		SimpleVertex2(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		SimpleVertex2(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		// Left Face
		SimpleVertex2(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		SimpleVertex2(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		SimpleVertex2(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		SimpleVertex2(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Right Face
		SimpleVertex2(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		SimpleVertex2(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		SimpleVertex2(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		SimpleVertex2(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SimpleVertex2) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;         //info struktura  
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = v;
	//Tworzymy bufer wierzchokow
	hr = p_Device->CreateBuffer(&vertexBufferDesc, &InitData, &g_pVertexBuffer);
	if (FAILED(hr)) return hr;
	// Bufer indeksow:
	// Tabela z danymi
	WORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	p_Device->CreateBuffer(&indexBufferDesc, &iinitData, &g_pIndexBuffer);
	indx_nmb = 36;

	return S_OK;
	///////////////////////
}

//#/////////////////////////////////////////#////
////////////////#//Sfera/#///////////////////////
//#/////////////////////////////////////////#////
HRESULT sphere::InitGeometry()
{


	HRESULT hr = S_OK;

	// Shader wierzcholkow
	ID3DBlob* pVSBlob = NULL; //Miejsce w pamieci
	hr = CompileShaderFromFile(L"shaders/MainShader.fx", "VS7", "vs_4_0", &pVSBlob);
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
	hr = CompileShaderFromFile(L"shaders/MainShader.fx", "PS7", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Nie znalieziona shader.", L"Error", MB_OK);
		return hr;
	}

	// Tworzymy pixel shader
	hr = p_Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr)) return hr;
	//////////////////////////////////////////////////////////////////
	CreateSphere(2, 32);
	

	return S_OK;
}

void sphere::CreateSphere(float r, UINT n) {
	HRESULT hr S_OK;
	float nRadius = r;
	UINT nSegments = n;

	//float nRadius = 2.0f;
	//UINT nSegments = 32;
	UINT nTexturesCount = 1;

	float dec = (float)2 / nSegments;
	float curry, currangle, currrad, texturey;
	int pntr = 0, nrm;
	float m_fRadius = nRadius;

	SimpleVertex2 *vertices = new SimpleVertex2[(nSegments + 1) * (nSegments + 1)];
	nrm = -1;
	for (UINT i = 0; i <= nSegments; i++) {
		currangle = XM_2PI * i / nSegments;

		for (UINT n = 0; n <= nSegments; n++) {
			curry = (1.0f - 2 * (FLOAT)n / nSegments) * nRadius;
			currrad = sqrtf(nRadius*nRadius - curry*curry);

			if (nTexturesCount == 1)
				texturey = (FLOAT)nTexturesCount * n / nSegments;
			else {
				texturey = ((FLOAT)nTexturesCount * n / nSegments) * 0.8f + 0.1f;
			}

			if (n == 0 || n == nSegments) {
				vertices[pntr].Pos = XMFLOAT3(0.0f, curry, 0.0f);
				vertices[pntr].TexCoord = XMFLOAT2((FLOAT)nTexturesCount*i / nSegments, texturey);
				vertices[pntr].Normal = XMFLOAT3(0.0f, curry*nrm / nRadius, 0.0f);
				++pntr;
				continue;
			}


			vertices[pntr].Pos = XMFLOAT3(cosf(currangle)*currrad, curry, sinf(currangle)*currrad);
			vertices[pntr].TexCoord = XMFLOAT2((FLOAT)nTexturesCount*i / nSegments, texturey);
			vertices[pntr].Normal = XMFLOAT3(10.0f, (FLOAT)nrm, 0.0f);
			++pntr;
		}
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex2) * (nSegments + 1) * (nSegments + 1);	// размер буфера
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = p_Device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr)) g_pVertexBuffer = NULL;



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	indx_nmb = nSegments * (nSegments - 1) * 2 * 3;
	WORD *indices = new WORD[indx_nmb];
	UINT arcpntr;
	pntr = 0;

	for (UINT i = 0; i<nSegments; i++) {
		//(i==(nSegments-1)) ? arcpntr = 0 : arcpntr = i+1;
		arcpntr = i + 1;
		for (UINT n = 0; n<nSegments; n++) {
			if (n == 0) {
				indices[pntr++] = (nSegments + 1)*i + n;
				indices[pntr++] = (nSegments + 1)*arcpntr + n + 1;
				indices[pntr++] = (nSegments + 1)*i + n + 1;
				continue;
			}
			else if (n == (nSegments - 1)) {
				indices[pntr++] = (nSegments + 1)*i + n;
				indices[pntr++] = (nSegments + 1)*arcpntr + n;
				indices[pntr++] = (nSegments + 1)*i + n + 1;
				continue;
			}
			indices[pntr++] = (nSegments + 1)*i + n;
			indices[pntr++] = (nSegments + 1)*arcpntr + n;
			indices[pntr++] = (nSegments + 1)*arcpntr + n + 1;
			indices[pntr++] = (nSegments + 1)*i + n;
			indices[pntr++] = (nSegments + 1)*arcpntr + n + 1;
			indices[pntr++] = (nSegments + 1)*i + n + 1;
		}
	}

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * indx_nmb;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = p_Device->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr)) g_pIndexBuffer = NULL;

}