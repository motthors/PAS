#include"ShaderManager.h"

#include"ShaderClass/EffectDefault.h"
#include"ShaderClass/NonLight.h"

#include"ShaderClass/HDRToneMap.h"
#include"ShaderClass/BlurBloom.h"
#include"ShaderClass/ScaleDownMagic.h"

#include"../base/DefineRendering.h"


ShaderManager::ShaderManager()
{
	m_pDx11		= nullptr;
	m_pDevice	= nullptr;
	m_pContext	= nullptr;
	m_pSwapChain= nullptr;
	pDataBox	= nullptr;
	pMatBoxM	= nullptr;
	pMB			= nullptr;
	pBBBM		= nullptr;
	pDebugTex	= nullptr;

	pHDR		= nullptr;
	pScaleDown	= nullptr;
	FOR(EFFECT_NUM)aEffect[i] = nullptr;

	FOR(MATBOX_TOTALNUM)EffectIdx[i]= 0;
	FOR(MATBOX_TOTALNUM)PS[i] = NULL;
	EffectIdx[MATBOX_SPACE]	= 1;
	EffectIdx[MATBOX_EFFECT]	= 1;


	//pHDRBlurTex = 0;
	//pHDRBlurUAV = 0;
	m_pRenderTargetView = 0;
	m_pDepthStencil = 0;
	m_pDepthStencilView = 0;

	m_pCommonRasterizerState = 0;
	FOR(m_BlendStateNum)m_pCommonTextureBlendState[i] = 0;
	m_pCommonDepthStencilState = 0;
	FOR(m_SamplerStateNum)m_pSamplerState[i] = 0;
}

ShaderManager::~ShaderManager()
{
	SAFE_DELETE(pHDR);
	SAFE_DELETE(pScaleDown);
	SAFE_DELETE(pBlurBloom);
	FOR(EFFECT_NUM)SAFE_DELETE(aEffect[i]);

	RELEASE(m_pDepthStencilView);
	RELEASE(m_pDepthStencil);
	RELEASE(m_pRenderTargetView);

	RELEASE(m_pCommonRasterizerState);
	FOR(m_BlendStateNum)RELEASE(m_pCommonTextureBlendState[i]);
	RELEASE(m_pCommonDepthStencilState);
	FOR(m_SamplerStateNum)RELEASE(m_pSamplerState[i]);
}


void ShaderManager::SetPointerSet(MatrixBox** pPS)
{
	FOR(MATBOX_TOTALNUM)PS[i] = pPS[i];
}


void ShaderManager::Init(DirectX11Base* pDx11, ShaderBox* pshader)
{
	m_pDx11 = pDx11;
	m_pDevice = pDx11->GetDevice();
	m_pContext = pDx11->GetContext();
	m_pSwapChain = pDx11->GetSwapChain();

	m_pShaderBox = pshader;

	// ビューポートの設定
	m_vp.Width = static_cast<float>(DefRender.RenderTargetX);
	m_vp.Height = static_cast<float>(DefRender.RenderTargetY);
	m_vp.MinDepth = 0.0f;
	m_vp.MaxDepth = 1.0f;
	m_vp.TopLeftX = 0;
	m_vp.TopLeftY = 0;
	m_pContext->RSSetViewports(1, &m_vp);

	// 共用ステンシルZバッファ作成
	CreateDepthStencil();

	// 最終描画ターゲット作成
	CreateRenderTarget();
	m_pShaderBox->FinalRTV = m_pRenderTargetView;

	CreateCommonState();

	pPASCT->SetSampler(m_pSamplerState[0]);
	pPASCT->CreateBlendState(m_pCommonTextureBlendState[0]);
	int idx=0;

	m_pShaderBox->SetDepthstencil(m_pDepthStencilView);
	m_pShaderBox->SetDepthFlag(1);
	//描画物体用途別シェーダークラス初期化
	//{
	//	ShaderBase* temp;
	//	////temp = new FixedPipeline;
	//	temp = new EffectDefault;
	//	temp->Init(this,pdev);
	//	aEffect[idx++] = temp;

	//	temp = new NonLight;
	//	temp->Init(this,pdev);
	//	aEffect[idx++] = temp;
	//}
	//if(idx != EFFECT_NUM)
	//{
	//	ALERT(_T("DEBUGERROR EFFECT_NUMがおかしい"));
	//	ErrM.SetClassName(_T("ShaderManager::Init"));
	//	ErrM.SetHResult(0);
	//	throw &ErrM;
	//}
	
	// ブラーブルームシェーダークラス初期化
	pBlurBloom = new BlurBloom;
	pBlurBloom->Init(m_pDx11, pshader, pDebugTex);

	// 縮小マジックシェーダークラス初期化
	pScaleDown = new ScaleDownMagic;
	pScaleDown->Init(m_pDx11, pshader, pDebugTex);

	//pHDR = new HDRToneMap;
	//pHDR->Init(this,pdev);
}


void ShaderManager::CreateTexture(ID3D11Texture2D** pTex, UINT x, UINT y, DXGI_FORMAT Format)
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
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Desc.CPUAccessFlags = 0;
	Desc.MiscFlags = 0;
	HRESULT	hr = m_pDevice->CreateTexture2D(&Desc, NULL, pTex);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateDepthStencil::CreateTexture2D"));
		ErrM.SetErrorText(_T("深度バッファテクスチャ作成に失敗"));
		ErrM.SetHResult(hr);
	}
}

void ShaderManager::CreateSRV(ID3D11Texture2D* pTex, ID3D11ShaderResourceView** pView, DXGI_FORMAT Format)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.Format = Format;
	Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	// レンダリングターゲットを作成する
	HRESULT hr = m_pDevice->CreateShaderResourceView(pTex, &Desc, pView);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateUAV::CreateUnorderedAccessView"));
		ErrM.SetErrorText(_T("UAV作成に失敗"));
		ErrM.SetHResult(hr);
	}
}

void ShaderManager::CreateRTV(ID3D11Texture2D* pTex, ID3D11RenderTargetView** pView, DXGI_FORMAT Format)
{
	D3D11_RENDER_TARGET_VIEW_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.Format = Format;
	Desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// レンダリングターゲットを作成する
	HRESULT hr = m_pDevice->CreateRenderTargetView(pTex, &Desc, pView);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateUAV::CreateUnorderedAccessView"));
		ErrM.SetErrorText(_T("UAV作成に失敗"));
		ErrM.SetHResult(hr);
	}
}

void ShaderManager::CreateUAV(ID3D11Texture2D* pTex, ID3D11UnorderedAccessView** pView, DXGI_FORMAT Format)
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.Format = Format;
	Desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

	// レンダリングターゲットを作成する
	HRESULT hr = m_pDevice->CreateUnorderedAccessView(pTex, &Desc, pView);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateUAV::CreateUnorderedAccessView"));
		ErrM.SetErrorText(_T("UAV作成に失敗"));
		ErrM.SetHResult(hr);
	}
}


void ShaderManager::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateRenderTarget::GetBuffer"));
		ErrM.SetErrorText(_T("バックバッファ取得に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}

	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateRenderTarget::CreateRenderTargetView"));
		ErrM.SetErrorText(_T("バックバッファRTV作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}

void ShaderManager::CreateDepthStencil()
{
	// 深度バッファテクスチャを作成する
	D3D11_TEXTURE2D_DESC	depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = DefRender.RenderTargetX;//DefRender.RenderTargetX;
	depthDesc.Height = DefRender.RenderTargetY;// DefRender.RenderTargetY;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	HRESULT	hr = m_pDevice->CreateTexture2D(&depthDesc, NULL, &m_pDepthStencil);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateDepthStencil::CreateTexture2D"));
		ErrM.SetErrorText(_T("深度バッファテクスチャ作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}

	// 深度バッファターゲットを作成する
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencil, &dsvDesc, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateDepthStencil::CreateDepthStencilView"));
		ErrM.SetErrorText(_T("深度バッファターゲット作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}


void ShaderManager::CreateCommonState()
{
	HRESULT hr;

	/////////////////////////////////////////////////////
	// ブレンドステート
	D3D11_BLEND_DESC BlendDesc[m_BlendStateNum];
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc[0].AlphaToCoverageEnable = FALSE;
	BlendDesc[0].IndependentBlendEnable = TRUE;
	BlendDesc[0].RenderTarget[0].BlendEnable = TRUE;
	BlendDesc[0].RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc[0].RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc[0].RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc[0].RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc[0].RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc[0].RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc[0].RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	BlendDesc[0].RenderTarget[1].BlendEnable = TRUE;
	BlendDesc[0].RenderTarget[1].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc[0].RenderTarget[1].DestBlend = D3D11_BLEND_ZERO;
	BlendDesc[0].RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc[0].RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc[0].RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc[0].RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc[0].RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	BlendDesc[1].AlphaToCoverageEnable = FALSE;
	BlendDesc[1].IndependentBlendEnable = FALSE;
	BlendDesc[1].RenderTarget[0].BlendEnable = TRUE;
	BlendDesc[1].RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc[1].RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc[1].RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc[1].RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc[1].RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc[1].RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc[1].RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	BlendDesc[2].AlphaToCoverageEnable = FALSE;
	BlendDesc[2].IndependentBlendEnable = FALSE;
	BlendDesc[2].RenderTarget[0].BlendEnable = TRUE;
	BlendDesc[2].RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc[2].RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendDesc[2].RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc[2].RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc[2].RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc[2].RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc[2].RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	FOR(m_SamplerStateNum)
	{
		hr = m_pDevice->CreateBlendState(&BlendDesc[i], &m_pCommonTextureBlendState[i]);
		if (FAILED(hr))
		{
			ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateBlendState"));
			ErrM.SetErrorText(_T("ブレンドステート作成に失敗"));
			ErrM.SetHResult(hr);
			throw &ErrM;
		}
	}
	/////////////////////////////////////////////////////
	// ラスタライザステート
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.DepthClipEnable = TRUE;
	hr = m_pDevice->CreateRasterizerState(&rsDesc, &m_pCommonRasterizerState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateRasterizerState"));
		ErrM.SetErrorText(_T("ラスタライザステート作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}

	/////////////////////////////////////////////////////
	// デプスステンシルステート -> ShaderBox へ移動しました
	//D3D11_DEPTH_STENCIL_DESC dsDesc;
	//ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	//dsDesc.DepthEnable = TRUE;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//dsDesc.StencilEnable = FALSE;
	//hr = m_pDevice->CreateDepthStencilState(&dsDesc, &m_pCommonDepthStencilState);
	//if (FAILED(hr))
	//{
	//	ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateDepthStencilState"));
	//	ErrM.SetErrorText(_T("デプスステンシルステート作成に失敗"));
	//	ErrM.SetHResult(hr);
	//	throw &ErrM;
	//}


	/////////////////////////////////////////////////////
	// サンプラステート
	D3D11_SAMPLER_DESC samDesc[m_SamplerStateNum];
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc[0].Filter = D3D11_FILTER_ANISOTROPIC;
	samDesc[0].AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc[0].AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc[0].AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc[0].MaxAnisotropy = 1;
	samDesc[0].ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc[0].MaxLOD = D3D11_FLOAT32_MAX;
	samDesc[1].Filter = D3D11_FILTER_ANISOTROPIC;
	samDesc[1].AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc[1].AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc[1].AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc[1].MaxAnisotropy = 1;
	samDesc[1].ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc[1].MaxLOD = D3D11_FLOAT32_MAX;

	FOR(m_SamplerStateNum)
	{
		hr = m_pDevice->CreateSamplerState(&samDesc[i], &m_pSamplerState[i]);
		if (FAILED(hr))
		{
			ErrM.SetClassName(_T("ShaderManager::CreateCommonState::CreateSamplerState"));
			ErrM.SetErrorText(_T("サンプラステート作成に失敗"));
			ErrM.SetHResult(hr);
			throw &ErrM;
		}
	}

	ResetState();
}

void ShaderManager::ResetState()
{
	// Set State
	m_pContext->OMSetBlendState(m_pCommonTextureBlendState[0], Colors::Black, 0xffffffff);
	//m_pContext->OMSetDepthStencilState(m_pCommonDepthStencilState, 0);
	m_pContext->RSSetState(m_pCommonRasterizerState);
	m_pContext->PSSetSamplers(0, 2, m_pSamplerState);
	m_pContext->CSSetSamplers(0, 2, m_pSamplerState);
	m_pShaderBox->SetDepthFlag(0);
	m_pShaderBox->SetDepthState(m_pShaderBox->No);
	
	// temp
	m_pContext->CSSetSamplers(0, 1, &m_pSamplerState[0]);
}

//void ShaderManager::SetViewProj(mat4* V, mat4* P, vec* pCamPos)
//{
//	m_pShaderBox->matView = *V;
//	m_pShaderBox->matProj = *P;
//	m_pShaderBox->CameraPos = *pCamPos;
//
//	
//	if( pInput->GetKey( DIK_AT ) )((HDRToneMap*)pHDR)->up();
//	if( pInput->GetKey( DIK_COLON ) )((HDRToneMap*)pHDR)->down();
//}
//
//void ShaderManager::SetViewProj(mat4* V, mat4* P, vec* pCamPos, vec* pCamDir)
//{
//	m_pShaderBox->CameraDir = *pCamDir;
//	SetViewProj(V, P, pCamPos);
//}


//void ShaderManager::DrawTime(D3DXMATRIX *world, float scale, DWORD mSec)
//{
//	SetMatrix_W_WIT(world);
//
//	apEffect[EffectIdx]->BeginPass(0);
//	pDataBox->DrawTime(mSec);
//	apEffect[EffectIdx]->EndPass();	
//}


void ShaderManager::DrawObject()
{
	if(pMB->MeshIdx==-1)
	{
		pDataBox->Draw(pMB->ObjNum, pMB->TexNum);
		//pDataBox->DrawMesh(pMB->ObjNum);
	}
	else
	{
		pDataBox->DrawOneMesh(pMB->ObjNum, pMB->TexNum, pMB->MeshIdx);
		//pDataBox->DrawOneMesh(pMB->ObjNum, pMB->MeshIdx);
	}
}


//void ShaderManager::DrawTexture(ID3DXEffect* p)
//// 各シェーダーから呼ばれるDataBoxへのTexture描画命令
//{
//	//pDataBox->SetTexture(pMB->TexNum, p);
//}


void ShaderManager::Draw()
//MatrixBoxの中を見ながらDataBoxとShaderでゴリゴリ描画
{
	m_vp.Width = static_cast<float>(DefRender.RenderTargetX);
	m_vp.Height = static_cast<float>(DefRender.RenderTargetY);
	m_pContext->RSSetViewports(1, &m_vp);

	m_pShaderBox->SetDepthFlag(0);

	//m_pShaderBox->ChangeRenderTarget(0, pFixedSizeRTV);
	//m_pShaderBox->ChangeRenderTarget(1, m_pMyDepthRTView);
	m_pShaderBox->ChangeRenderTarget(0, (UINT)0);
	m_pShaderBox->ChangeRenderTarget(1, 1);
	m_pShaderBox->SetRTsToShader();

	//pd3ddev->SetRenderTarget( 0, pFixedSizeSurface );	
	//pd3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(1, 0, 0, 0), 1.0f, 0);
	//pd3ddev->SetRenderTarget( 1, pHDRBlurSurface );

	//FOR(MATBOX_TOTALNUM)
	//{
	//	pMB = PS[i];

	//	while(pMB != NULL)
	//	{
	//		//シェーダに行列をセット
	//		pmatWorld = &pMB->mat;
	//		aEffect[EffectIdx[i]]->SetMatrix();

	//		//draw
	//		aEffect[EffectIdx[i]]->Render();
	//		pMB = pMB->next;
	//	}
	//}


	//大気散乱シミュ描画　PAS 
	pPASCT->Render();

	// BBB描画
	//pBBBM->MeshSet();
	//D3DXMATRIX matInit;
	//pmatWorld = D3DXMatrixIdentity(&matInit);
	//aEffect[0]->SetMatrix();
	//aEffect[0]->Begin();
	//pBBBM->Draw();
	//aEffect[0]->End();

	//m_pContext->OMSetBlendState(m_pCommonTextureBlendState[1], Colors::Black, 0xffffffff);

	// ToneMap + BlurBloom
	if (pInput->GetKey(DIK_AT))((BlurBloom*)pBlurBloom)->up();
	if (pInput->GetKey(DIK_COLON))((BlurBloom*)pBlurBloom)->down();
	pBlurBloom->Render();
	//pd3ddev->SetTexture(0, pFixedSizeTex );
	//pd3ddev->SetRenderTarget( 1, NULL );
	//pd3ddev->SetRenderTarget( 0, pHDRBlurSurface );
	//pHDR->Render();
	//pScaleDown->Render();

	m_pContext->OMSetBlendState(m_pCommonTextureBlendState[0], Colors::Black, 0xffffffff);

	m_pShaderBox->SetDepthFlag(0);

	//縮小マジック
	m_vp.Width = static_cast<float>(DefRender.WindowX);
	m_vp.Height = static_cast<float>(DefRender.WindowY);
	m_pContext->RSSetViewports(1, &m_vp);
	//pd3ddev->SetTexture(0, pHDRBlurTex );
	//pd3ddev->SetTexture(0, pFixedSizeTex );
	pScaleDown->Render();

	//2D描画
	p2DDrawer->Render();

	// debug Texture View
	m_pContext->OMSetBlendState(m_pCommonTextureBlendState[2], Colors::Black, 0xffffffff);
	if (pInput->GetKeyNow(DIK_SLASH)) pDebugTex->TurnOnOff();
	pDebugTex->Render();
}


void ShaderManager::BeginDraw()
{
	// Clear the back buffer
	float	clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float	clearDepth[] = { 1, 0.0f, 0.0f, 0.0f };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	m_pContext->ClearRenderTargetView(m_pShaderBox->pum_pRTVF32[0], clearColor);
	m_pContext->ClearRenderTargetView(m_pShaderBox->pum_pRTVF32[1], clearDepth);
	m_pContext->ClearRenderTargetView(m_pShaderBox->pum_pRTVF32[3], clearColor);
	
	// Clear the depth buffer to 1.0 (max depth)
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	ResetState();
}

void ShaderManager::EndDraw()
{
	// Present our back buffer to our front buffer
	m_pSwapChain->Present(0, 0);
}