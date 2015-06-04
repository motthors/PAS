#include"ShaderBox.h"
#include"../base/DefineRendering.h"
#include <typeinfo.h>

ShaderBox::ShaderBox()
{
	m_pLayout = new D3D11_INPUT_ELEMENT_DESC[2];
	D3D11_INPUT_ELEMENT_DESC t_Layout[2] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	memcpy_s(m_pLayout, sizeof(t_Layout), t_Layout, sizeof(t_Layout));

	FOR(MultiRTNum)m_ArrpRTV[i] = nullptr;
	FOR(m_DepthStateNum)m_DepthStates[i] = nullptr;
	m_pDepthStencilOff0On1[0] = nullptr;

	FOR(m_TexF32Num)
	{
		pum_pTexF32[i] = nullptr;
		pum_pRTVF32[i] = nullptr;
		pum_pSRVF32[i] = nullptr;
	}
}


ShaderBox::~ShaderBox()
{
	for (auto i : VertexShaderBox)
	{
		RELEASE(i->ShaderObject);
		RELEASE(i->Layout);
		SAFE_DELETE(i);
	}
	for (auto i : PixelShaderBox)
	{
		RELEASE(i->ShaderObject);
		SAFE_DELETE(i);
	}
	SAFE_DELETEARRAY(m_pLayout);
	FOR(MultiRTNum)m_ArrpRTV[i] = nullptr;

	FOR(m_TexF32Num)RELEASE(pum_pTexF32[i]);
	FOR(m_TexF32Num)RELEASE(pum_pRTVF32[i]);
	FOR(m_TexF32Num)RELEASE(pum_pSRVF32[i]);
}


void ShaderBox::Init(DirectX11Base* pdx11)
{
	m_pDevice = pdx11->GetDevice();
	m_pContext = pdx11->GetContext();

	CreateTexture();
}


void ShaderBox::CreateTexture()
{
	FOR(m_TexF32Num)
	{
		CreateTexture2D(&pum_pTexF32[i],
			nullptr, 0,
			DefRender.RenderTargetX, DefRender.RenderTargetY,
			DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

		CreateRTV(pum_pTexF32[i], &pum_pRTVF32[i],
			DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_RTV_DIMENSION_TEXTURE2D);

		CreateSRV(pum_pTexF32[i], &pum_pSRVF32[i],
			DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);
	}
}


void ShaderBox::_CreateVertexShader(
	ID3D11VertexShader** OutVS,
	TCHAR* csoName, 
	ID3D11InputLayout** outLayout
	)
{
	HRESULT hr;

	// バイナリシェーダーファイルを読み込む
	FILE* fp = _tfopen(csoName, _T("rb"));
	if (fp == NULL)
	{
		ErrM.SetClassName(_T("ShaderBox::CreateVertexShader::_tfopen"));
		ErrM.SetErrorText(_T("csoファイル読み込みに失敗"));
		ErrM.SetHResult(USER_ERROR);
		throw &ErrM;
	}
	fseek(fp, 0, SEEK_END);
	long cso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* cso_data = new unsigned char[cso_sz];
	fread(cso_data, cso_sz, 1, fp);
	fclose(fp);

	//この時点でコンパイル後のバイナリはcso_dataに格納されている//

	// 頂点シェーダーオブジェクトの作成//
	hr = m_pDevice->CreateVertexShader(cso_data, cso_sz, NULL, OutVS);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreateVertexShader::CreateVertexShader"));
		ErrM.SetErrorText(_T("頂点シェーダーオブジェクトの作成に失敗"));
		ErrM.SetHResult(hr);
		delete[] cso_data;
		throw &ErrM;
	}

	// 入力頂点属性を作成する
	hr = m_pDevice->CreateInputLayout(m_pLayout, m_LayoutNum, cso_data, cso_sz, outLayout);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreateVertexShader::CreateInputLayout"));
		ErrM.SetErrorText(_T("入力レイアウト作成に失敗"));
		ErrM.SetHResult(hr);
		delete[] cso_data;
		throw &ErrM;
	}

	delete[] cso_data;
}



void ShaderBox::_CreatePixelShader(
	ID3D11PixelShader** OutPS,
	TCHAR* csoName
	)
{
	// バイナリファイルを読み込む
	FILE* fp = _tfopen(csoName, _T("rb"));
	if (fp == NULL)
	{
		ErrM.SetClassName(_T("ShaderBox::CreatePixelShader::_tfopen"));
		ErrM.SetErrorText(_T("csoファイル読み込みに失敗"));
		ErrM.SetHResult(USER_ERROR);
		throw &ErrM;
	}
	fseek(fp, 0, SEEK_END);
	long cso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* cso_data = new unsigned char[cso_sz];
	fread(cso_data, cso_sz, 1, fp);
	fclose(fp);

	//この時点でコンパイル後のバイナリはcso_dataに格納されている//

	// 頂点シェーダーオブジェクトの作成//
	HRESULT hr = m_pDevice->CreatePixelShader(cso_data, cso_sz, NULL, OutPS);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreatePixelShader::CreatePixelShader"));
		ErrM.SetErrorText(_T("ピクセルシェーダーオブジェクトの作成に失敗"));
		ErrM.SetHResult(hr);
		delete[] cso_data;
		throw &ErrM;
	}

	delete[] cso_data;
}


UINT ShaderBox::CreateVertexShader(TCHAR* csoName)
{
	// シェーダーオブジェクトリストにすでに登録されていればそれを返す
	// なかったら新しく作る
	FOR (VertexShaderBox.size())
	{
		if (0 == _tcscmp(VertexShaderBox[i]->FileName, csoName))
		{
			return i;
		}
	}

	// 無かったので新しく作る
	VertexShaderStruct temp;
	_CreateVertexShader(
		&temp.ShaderObject,
		csoName,
		&temp.Layout);

	VertexShaderStruct* ptemp = new VertexShaderStruct;
	_tcscpy_s(ptemp->FileName, 100, csoName);
	ptemp->ShaderObject = temp.ShaderObject;
	ptemp->Layout = temp.Layout;
	VertexShaderBox.push_back(ptemp);
	return VertexShaderBox.size()-1;
}

UINT ShaderBox::CreatePixelShader(TCHAR* csoName)
{
	FOR(PixelShaderBox.size())
	{
		if (0 == _tcscmp(PixelShaderBox[i]->FileName, csoName))
		{
			return i;
		}
	}
	PixelShaderStruct temp;
	_CreatePixelShader(&temp.ShaderObject, csoName);
	PixelShaderStruct* ptemp = new PixelShaderStruct;
	_tcscpy_s(ptemp->FileName, 100, csoName);
	ptemp->ShaderObject = temp.ShaderObject;
	PixelShaderBox.push_back(ptemp);
	return PixelShaderBox.size() - 1;
}


void ShaderBox::SetShader(UINT idVS, UINT idPS)
//セットされているシェーダーと違うものが要求されていたら切り替え
{
	//if (NowSetVertexShaderID != idVS)
	{
		m_pContext->IASetInputLayout(VertexShaderBox[idVS]->Layout);
		m_pContext->VSSetShader(VertexShaderBox[idVS]->ShaderObject, NULL, 0);
		NowSetVertexShaderID = idVS;
	}
	//if (NowSetPixelShaderID != idPS)
	{
		m_pContext->PSSetShader(PixelShaderBox[idPS]->ShaderObject, NULL, 0);
		NowSetPixelShaderID = idPS;
	}
}


void ShaderBox::SetMatrixVector(mat4* V, mat4* P, vec3* pCamPos)
{
	memcpy_s(&matView, sizeof(mat4), V, sizeof(mat4));
	memcpy_s(&matProj, sizeof(mat4), P, sizeof(mat4));
	memcpy_s(&CameraPos, sizeof(vec3), pCamPos, sizeof(vec3));
}



void ShaderBox::CreateComputeShader(
	ID3D11ComputeShader** outCS,
	TCHAR* csoName
	)
{
	// バイナリファイルを読み込む
	FILE* fp = _tfopen(csoName, _T("rb"));
	if (fp == NULL)
	{
		ErrM.SetClassName(_T("ShaderBox::CreateComputeShader::_tfopen"));
		ErrM.SetErrorText(_T("csoファイル読み込みに失敗"));
		ErrM.SetHResult(USER_ERROR);
		throw &ErrM;
	}
	fseek(fp, 0, SEEK_END);
	long cso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* cso_data = new unsigned char[cso_sz];
	fread(cso_data, cso_sz, 1, fp);
	fclose(fp);

	//この時点でコンパイル後のバイナリはcso_dataに格納されている//

	// 頂点シェーダーオブジェクトの作成//
	HRESULT hr = m_pDevice->CreateComputeShader(cso_data, cso_sz, NULL, outCS);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreateComputeShader::CreateComputeShader"));
		ErrM.SetErrorText(_T("頂点シェーダーオブジェクトの作成に失敗"));
		ErrM.SetHResult(hr);
		delete[] cso_data;
		throw &ErrM;
	}

	delete[] cso_data;
}


void ShaderBox::CreateTexture2D(
	ID3D11Texture2D** ppTex, 
	void* pInitData, size_t size,
	UINT x, UINT y, 
	DXGI_FORMAT Format,
	UINT D3D11_BIND_Flags,
	D3D11_USAGE usage)
{
	// テクスチャを作成する
	D3D11_TEXTURE2D_DESC	Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.Width = x;
	Desc.Height = y;
	Desc.MipLevels = 1;
	Desc.ArraySize = 1;
	Desc.Format = Format;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Usage = usage;
	Desc.BindFlags = D3D11_BIND_Flags;
	Desc.CPUAccessFlags = usage == D3D11_USAGE_STAGING ? D3D11_CPU_ACCESS_READ : 0;
	Desc.MiscFlags = 0;
	// 初期値を設定する
	D3D11_SUBRESOURCE_DATA* resource = NULL;
	if (pInitData)
	{
		resource = new D3D11_SUBRESOURCE_DATA();
		resource->pSysMem = pInitData;
		resource->SysMemPitch = size * x;
		resource->SysMemSlicePitch = size * x * y;
	}
	// 作成
	HRESULT	hr = m_pDevice->CreateTexture2D(&Desc, resource, ppTex);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreateTexture2D::CreateTexture2D"));
		ErrM.SetErrorText(_T("テクスチャ作成に失敗"));
		ErrM.SetHResult(hr);
	}
	SAFE_DELETE(resource);
}


void ShaderBox::CreateTexture3D(
	ID3D11Texture3D** ppTexture, 
	void* pInitData, size_t size,
	UINT x, UINT y, UINT z,
	DXGI_FORMAT Format,
	UINT D3D11_BIND_Flags,
	D3D11_USAGE usage)
{
	// テクスチャを作成する
	D3D11_TEXTURE3D_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.Width = x;
	Desc.Height = y;
	Desc.Depth = z;
	Desc.MipLevels = 1;
	Desc.Format = Format;
	Desc.Usage = usage;
	Desc.BindFlags = D3D11_BIND_Flags;
	Desc.CPUAccessFlags = usage == D3D11_USAGE_STAGING ? D3D11_CPU_ACCESS_READ : 0;
	Desc.MiscFlags = 0;
	// 初期値を設定する
	D3D11_SUBRESOURCE_DATA* resource = NULL;
	if (pInitData)
	{
		resource = new D3D11_SUBRESOURCE_DATA();
		resource->pSysMem = pInitData;
		resource->SysMemPitch = size * x;
		resource->SysMemSlicePitch = size * x * y;
	}
	// 作成
	HRESULT	hr = m_pDevice->CreateTexture3D(&Desc, resource, ppTexture);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("PASComputeTexture::CreatePASTexture3D::CreateTexture3D"));
		ErrM.SetErrorText(_T("テクスチャ作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	SAFE_DELETE(resource);
}


void ShaderBox::CreateSRV(
	ID3D11Resource* pTex,
	ID3D11ShaderResourceView** ppSRV,
	DXGI_FORMAT Format,
	D3D11_SRV_DIMENSION ViewDimension)
{// シェーダから読み込む専用View
	HRESULT hr;

	// ShaderResourceView作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = Format;
	srvDesc.ViewDimension = ViewDimension;
	srvDesc.Texture2D.MipLevels = 1;

	hr = m_pDevice->CreateShaderResourceView(pTex, &srvDesc, ppSRV);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreateSRView::CreateShaderResourceView"));
		ErrM.SetErrorText(_T("SRVの作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}


void ShaderBox::CreateRTV(
	ID3D11Resource* pTex,
	ID3D11RenderTargetView** ppRTV,
	DXGI_FORMAT Format,
	D3D11_RTV_DIMENSION ViewDimension)
{		// 書き込みのためのView
	HRESULT hr;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = Format;
	rtvDesc.ViewDimension = ViewDimension;

	// レンダリングターゲットを作成する
	hr = m_pDevice->CreateRenderTargetView(pTex, &rtvDesc, ppRTV);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreateRTV::CreateRenderTargetView"));
		ErrM.SetErrorText(_T("RTV作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}


void ShaderBox::CreateUAV(
	ID3D11Resource* pTex, 
	ID3D11UnorderedAccessView** ppUAV,
	DXGI_FORMAT Format,
	D3D11_UAV_DIMENSION ViewDimension)
{	// 読み書き汎用View
	HRESULT hr;
	
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = Format;
	uavDesc.ViewDimension = ViewDimension;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	hr = m_pDevice->CreateUnorderedAccessView(pTex, &uavDesc, ppUAV);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderBox::CreateUAV::CreateUnorderedAccessView"));
		ErrM.SetErrorText(_T("UAVの作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}


void ShaderBox::ChangeRenderTarget(UINT index, ID3D11RenderTargetView* pRTV)
{
	m_ArrpRTV[index] = pRTV;
}
void ShaderBox::ChangeRenderTarget(UINT index, UINT pum_pRTVID)
{
	m_ArrpRTV[index] = pum_pRTVF32[pum_pRTVID];
}
void ShaderBox::SetRTsToShader()
{
	m_pContext->OMSetRenderTargets(MultiRTNum, m_ArrpRTV, m_pDepthStencilOff0On1[m_OnOffFlag]);
}


void ShaderBox::CreateDepthStencilStates()
{
	HRESULT hr;
	/////////////////////////////////////////////////////
	// デプスステンシルステート作成
	D3D11_DEPTH_STENCIL_DESC dsDesc[m_DepthStateNum];
	FOR(m_DepthStateNum)ZeroMemory(&dsDesc[i], sizeof(D3D11_DEPTH_STENCIL_DESC));

	//// Depth OFF ////
	dsDesc[0].DepthEnable = FALSE;
	//// Depth ON Less WriteOK////
	dsDesc[1].DepthEnable = TRUE;
	dsDesc[1].DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc[1].DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc[1].StencilEnable = FALSE;
	//// Depth ON AllOK WritNO ////
	dsDesc[2].DepthEnable = TRUE;
	dsDesc[2].DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc[2].DepthFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc[2].StencilEnable = FALSE;

	FOR(m_DepthStateNum)
	{
		hr = m_pDevice->CreateDepthStencilState(&dsDesc[i], &m_DepthStates[i]);
		if (FAILED(hr))
		{
			ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateDepthStencilState"));
			ErrM.SetErrorText(_T("デプスステンシルステート作成に失敗"));
			ErrM.SetHResult(hr);
			throw &ErrM;
		}
	}
}


void ShaderBox::SetDepthState(KindOfDepthState num)
{
	m_pContext->OMSetDepthStencilState(m_DepthStates[num], 0);
}