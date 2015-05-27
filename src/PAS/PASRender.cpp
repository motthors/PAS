#include"PASRender.h"
#include"../base/DefineRendering.h"

PASRender::PASRender()
{
	m_p2Dsq = nullptr;
	m_pShaderBox = nullptr;

	ZeroMemory(&m_PAS_ConstStruct_VS, sizeof(m_PAS_ConstStruct_VS));
	ZeroMemory(&m_PAS_ConstStruct_PS, sizeof(m_PAS_ConstStruct_PS));
	FOR(MaxPlanetNum) m_pPlanet[i] = nullptr;
	FOR(m_RVnum)m_pRenderVarTexture[i] = nullptr;
	FOR(m_RVnum)m_pRenderVarSRView[i] = nullptr;
	FOR(m_RVnum)m_pRenderVarRTView[i] = nullptr;
}


PASRender::~PASRender()
{
	// Texture
	//RELEASE(m_TransmittanceTexture);
	//RELEASE(m_IrradianceTexture);
	//RELEASE(m_InscatterTexture);
	// SR View
	//RELEASE(m_TransmittanceView);
	//RELEASE(m_IrradianceView);
	//RELEASE(m_InscatterView);
	// Cont Baffer
	RELEASE(m_pConstantBafferVS);
	RELEASE(m_pConstantBafferPS);

	FOR(m_RVnum)RELEASE(m_pRenderVarTexture[i]);
	FOR(m_RVnum)RELEASE(m_pRenderVarSRView[i]);
	FOR(m_RVnum)RELEASE(m_pRenderVarRTView[i]);

	RELEASE(m_pRenderPalamBlendState);
	RELEASE(m_pAddBlendState);

	SAFE_DELETE(m_p2Dsq);

	FOR(MaxPlanetNum)SAFE_DELETE(m_pPlanet[i]); 
}


void PASRender::Init(
	DirectX11Base* pdx11,
	DataBox* pDB,
	MatrixBoxManager* pMB,
	ShaderBox* pSB
	)
{
	//TextureTestViewFlag = 0;

	m_pDatabox = pDB;
	//pMatBoxM = pMB;
	m_pDx11 = pdx11;
	m_pDevice = m_pDx11->GetDevice();
	m_pContext = m_pDx11->GetContext();
	m_pShaderBox = pSB;

	// planet struct
	AddPlanet();
	AddPlanet();

	// ピクセルシェーダ
	idPS_PAS = m_pShaderBox->CreatePixelShader(_T("data/hlsl/PAS_PS.cso"));
	idPS_PrePAS = m_pShaderBox->CreatePixelShader(_T("data/hlsl/PAS_PS1_PreRender.cso"));
	idPS_PAS_Ground = m_pShaderBox->CreatePixelShader(_T("data/hlsl/PAS_PS2_Ground.cso"));
	idPS_PAS_Atmosphere = m_pShaderBox->CreatePixelShader(_T("data/hlsl/PAS_PS3_Atmosphere.cso"));
	idPS_PAS_Sun = m_pShaderBox->CreatePixelShader(_T("data/hlsl/PAS_PS4_Sun.cso"));

	m_p2Dsq = new D3D2DSQUARE(pdx11, pSB, DefRender.RenderTargetX, DefRender.RenderTargetY);
	m_p2Dsq->Init();
	m_p2Dsq->SetVertexShader(m_pShaderBox->CreateVertexShader(_T("data/hlsl/PAS_VS.cso")));
	m_p2Dsq->SetPixelShader(idPS_PAS);

	//定数バッファ作成
	m_pDx11->CreateConstantBuffer(&m_pConstantBafferVS, NULL, sizeof(PAS_ConstStruct_VS), D3D11_CPU_ACCESS_WRITE);
	m_pDx11->CreateConstantBuffer(&m_pConstantBafferPS, NULL, sizeof(PAS_ConstStruct_PS), D3D11_CPU_ACCESS_WRITE);

	//if (CheckLoadTextreFile() == false)
	PASRender::CreateTexture();
}


void PASRender::CheckLoadTextreFile(void* pData, char* FileName, UINT dataNum, UINT LayerNum)
{
	float* pdata = reinterpret_cast<float*>(pData);
	FILE* fp=0;
	fopen_s(&fp, FileName, "rb");
	if (fp == nullptr)
	{
		pData = 0;
		return;
	}
	FOR(LayerNum)
	{
		fread((pdata + 4*i*dataNum), sizeof(float)*4, dataNum, fp);
	}
	fclose(fp);
}


void PASRender::CreateTexture()
{
	//texidRefrectanceTexture = m_pDatabox->GetTexture(_T("data/earth.dds"));

	//m_pShaderBox->CreateTexture2D(&m_TransmittanceTexture, bufferTrans, sizeof(FLOAT128), TRANSMITTANCE_W, TRANSMITTANCE_H, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE);
	//m_pShaderBox->CreateTexture2D(&m_IrradianceTexture, bufferIrrad, sizeof(FLOAT128), SKY_W, SKY_H, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE);
	//m_pShaderBox->CreateTexture3D(&m_InscatterTexture, bufferInsca, sizeof(FLOAT128), RES_MU_S * RES_NU, RES_MU, RES_R, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE);

	//m_pShaderBox->CreateSRV(m_TransmittanceTexture, &m_TransmittanceView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);
	//m_pShaderBox->CreateSRV(m_IrradianceTexture, &m_IrradianceView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);
	//m_pShaderBox->CreateSRV(m_InscatterTexture, &m_InscatterView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE3D);
	
	// RenderVariableTexture生成
	FOR(m_RVnum)m_pShaderBox->CreateTexture2D(&m_pRenderVarTexture[i], nullptr, 0, DefRender.RenderTargetX, DefRender.RenderTargetY, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
	FOR(m_RVnum)m_pShaderBox->CreateSRV(m_pRenderVarTexture[i], &m_pRenderVarSRView[i], DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);
	FOR(m_RVnum)m_pShaderBox->CreateRTV(m_pRenderVarTexture[i], &m_pRenderVarRTView[i], DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_RTV_DIMENSION_TEXTURE2D);

}


void PASRender::LoadPASTextureFromFile(ID3D11Resource* pTex, wchar_t* fname, int DataNnum)
{
	HRESULT hr;

	// テクスチャデータファイルオープン
	FILE* fp;
	_wfopen_s(&fp, fname, L"wb");
	if (fp == NULL)
	{
		ErrM.SetClassName(_T("PASRender::CreatePASView::fopen"));
		ErrM.SetErrorText(_T("テクスチャデータファイルのオープンに失敗"));
		ErrM.SetHResult(USER_ERROR);
		throw &ErrM;
	}


	// テクスチャをロック
	D3D11_MAPPED_SUBRESOURCE LockedRect;
	hr = m_pContext->Map(pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &LockedRect);

	ZeroMemory(LockedRect.pData, DataNnum*sizeof(vec4));
	vec4 *pTexture = reinterpret_cast<vec4*>(LockedRect.pData);

	// 読み込み
	FOR(DataNnum)
	{
		fread_s(&pTexture[i], sizeof(vec4)*DataNnum, sizeof(vec4), DataNnum, fp);
	}


	// テクスチャのロック解除
	m_pContext->Unmap(pTex, 0);

	fclose(fp);
}


void PASRender::CreateBlendState(ID3D11BlendState* p)
{
	m_pDefaultBlendState = p;

	HRESULT hr;
	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	// 0番　上書き
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	// 1番 深度　より小さかったら描画
	BlendDesc.RenderTarget[1].BlendEnable = TRUE;
	BlendDesc.RenderTarget[1].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[1].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_MIN;
	BlendDesc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	// 2番　上書き　ただしアルファ成分だけ最小値を返すように
	memcpy_s(&BlendDesc.RenderTarget[2], sizeof(BlendDesc.RenderTarget[0]),
		&BlendDesc.RenderTarget[0], sizeof(BlendDesc.RenderTarget[0]));
	BlendDesc.RenderTarget[2].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[2].DestBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[2].BlendOpAlpha = D3D11_BLEND_OP_MIN;

	hr = m_pDevice->CreateBlendState(&BlendDesc, &m_pRenderPalamBlendState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("PASRender::CreateBlendState::CreateBlendState"));
		ErrM.SetErrorText(_T("ブレンドステート作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = m_pDevice->CreateBlendState(&BlendDesc, &m_pAddBlendState);
	if (FAILED(hr))
	{
		ErrM.SetClassName(_T("PASRender::CreateBlendState::CreateBlendState"));
		ErrM.SetErrorText(_T("ブレンドステート作成に失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
}


void PASRender::AddPlanet()
{
	if (MaxPlanetNum <= m_PlanetNum)return;

	m_pPlanet[m_PlanetNum] = new PASPlanetStruct;
	m_pPlanet[m_PlanetNum]->Init(m_pDx11, m_pDatabox, NULL, m_pShaderBox);

	//Load Texture
	float* bufferTrans = new float[4 * TRANS_W * TRANS_H];
	float* bufferIrrad = new float[4 * SKY_W * SKY_H];
	float* bufferInsca = new float[4 * RES_MU_S * RES_NU * RES_MU * RES_R];

	CheckLoadTextreFile((void*)bufferTrans, "data/trans.pastex", TRANS_W * TRANS_H, 1);
	CheckLoadTextreFile((void*)bufferIrrad, "data/irrad.pastex", SKY_W * SKY_H, 1);
	CheckLoadTextreFile((void*)bufferInsca, "data/insca.pastex", RES_MU_S * RES_NU * RES_MU, RES_R);

	m_pPlanet[m_PlanetNum]->CreateTexture(bufferTrans, bufferIrrad, bufferInsca);

	SAFE_DELETEARRAY(bufferTrans);
	SAFE_DELETEARRAY(bufferIrrad);
	SAFE_DELETEARRAY(bufferInsca);

	// temp, Set Planet Pos
	vec3 t_pos = vec3(14000 * m_PlanetNum, 0, 0);
	POSINDEX t_idx;
	t_idx.x = t_idx.y = t_idx.z = 0;
	m_pPlanet[m_PlanetNum]->SetPosition(&t_pos, &t_idx);

	m_NowPlanet = m_pPlanet[m_PlanetNum];
	m_PlanetNum++;
}


void PASRender::Move()
{

}


void PASRender::Draw()
{

#ifdef _DEBUG
	DebugDraw();
#endif

}


void PASRender::Render(ID3D11RenderTargetView* pOutRTV)
{
	// 行列計算/////////////////////////////////////////
	vec Determinant;
	mat V = XMLoadFloat4x4(&m_pShaderBox->matView);
	mat P = XMLoadFloat4x4(&m_pShaderBox->matProj);
	V = XMMatrixInverse(&Determinant, V);
	P = XMMatrixInverse(&Determinant, P);
	V = XMMatrixTranspose(V);
	P = XMMatrixTranspose(P);
	//XMStoreFloat4x4(&m_PAS_ConstStruct_VS.matVIxPI, V*P);

	//// 定数バッファ設定 //////////////////////////
	XMStoreFloat4x4(&m_PAS_ConstStruct_VS.matVI, V);
	XMStoreFloat4x4(&m_PAS_ConstStruct_VS.matPI, P);

	//m_matVIxPI = XMMatrixMultiply(m_matViewI, m_matProjI);
	//m_matVIxPI = XMMatrixInverse(&Determinant, mat);

	//m_PAS_ConstStruct_VS.vecCamPos.x = m_pShaderBox->CameraPos.x;
	//m_PAS_ConstStruct_VS.vecCamPos.y = m_pShaderBox->CameraPos.y;
	//m_PAS_ConstStruct_VS.vecCamPos.z = m_pShaderBox->CameraPos.z;
	//m_PAS_ConstStruct_VS.vecCamPos.w = 0.0f;
	//memcpy_s(&m_PAS_ConstStruct_PS.vecCamPos, sizeof(vec4),
	//		&m_PAS_ConstStruct_VS.vecCamPos, sizeof(vec4));

	//memcpy_s(&m_NowPlanet->m_PAS_EPCS_PS.vecPlanetPos, sizeof(vec4),
	//	&m_PAS_ConstStruct_VS.vecCamPos, sizeof(vec4));

	//// 定数バッファ変更 //////////////////////////
	m_pDx11->SetConstBuffer(m_pConstantBafferVS, &m_PAS_ConstStruct_VS, sizeof(PAS_ConstStruct_VS));
	m_pDx11->SetConstBuffer(m_pConstantBafferPS, &m_PAS_ConstStruct_PS, sizeof(PAS_ConstStruct_PS));
	//m_pDx11->SetConstBuffer(m_NowPlanet->m_pConstantBafferPS,
	//						&m_NowPlanet->m_PAS_EPCS_PS, sizeof(m_NowPlanet->m_PAS_EPCS_PS));

	// Set Const Buffer //////////////////////////
	m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBafferVS);
	m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBafferPS);

	ID3D11ShaderResourceView* pnull = nullptr;

	m_pContext->OMSetBlendState(m_pAddBlendState, Colors::Black, 0xffffffff);

	FOR(m_PlanetNum)
	{
		m_pShaderBox->SetDepthState(m_pShaderBox->No);

		// Set Planet Const Buffer //////////////
		m_pPlanet[i]->SetConstStruct(&m_pShaderBox->CameraPos, nullptr);

		// Set Texture //////////////////////////
		m_pDatabox->SetTexture(/*texidRefrectanceTexture*/m_pPlanet[i]->GetTexID_ref(), 0);
		m_pContext->PSSetShaderResources(1, 1, /*&m_TransmittanceView*/m_pPlanet[i]->GetRTV_tra());
		m_pContext->PSSetShaderResources(2, 1, /*&m_IrradianceView*/m_pPlanet[i]->GetRTV_irr());
		m_pContext->PSSetShaderResources(3, 1, /*&m_InscatterView*/m_pPlanet[i]->GetRTV_ins());


		m_pShaderBox->ChangeRenderTarget(0, pOutRTV);
		m_pShaderBox->SetRTsToShader();
		m_pContext->PSSetShaderResources(4, 1, &m_pRenderVarSRView[0]);
		m_pContext->PSSetShaderResources(5, 1, &m_pRenderVarSRView[1]);
		//m_pContext->PSSetShaderResources(6, 1, &pDepthSRV);
		m_p2Dsq->SetPixelShader(idPS_PAS);
		m_p2Dsq->Render();

	}


	m_pContext->PSSetShaderResources(4, 1, &pnull);
	m_pContext->PSSetShaderResources(5, 1, &pnull);

	/// test pass : RefTex ///
	//m_p2Dsq->SetPixelShader(idPS_PAS);
	//m_p2Dsq->Render();

	m_pContext->OMSetBlendState(m_pDefaultBlendState, Colors::Black, 0xffffffff);
}


void PASRender::Render2(ID3D11RenderTargetView* pOutRTV)
{
	// 行列計算/////////////////////////////////////////
	vec Determinant;
	mat V = XMLoadFloat4x4(&m_pShaderBox->matView);
	mat P = XMLoadFloat4x4(&m_pShaderBox->matProj);
	V = XMMatrixInverse(&Determinant, V);
	P = XMMatrixInverse(&Determinant, P);
	V = XMMatrixTranspose(V);
	P = XMMatrixTranspose(P);
	//XMStoreFloat4x4(&m_PAS_ConstStruct_VS.matVIxPI, V*P);

	//// 定数バッファ設定 //////////////////////////
	XMStoreFloat4x4(&m_PAS_ConstStruct_VS.matVI, V);
	XMStoreFloat4x4(&m_PAS_ConstStruct_VS.matPI, P);

	//m_matVIxPI = XMMatrixMultiply(m_matViewI, m_matProjI);
	//m_matVIxPI = XMMatrixInverse(&Determinant, mat);

	//m_PAS_ConstStruct_VS.vecCamPos.x = m_pShaderBox->CameraPos.x;
	//m_PAS_ConstStruct_VS.vecCamPos.y = m_pShaderBox->CameraPos.y;
	//m_PAS_ConstStruct_VS.vecCamPos.z = m_pShaderBox->CameraPos.z;
	//m_PAS_ConstStruct_VS.vecCamPos.w = 0.0f;
	//memcpy_s(&m_PAS_ConstStruct_PS.vecCamPos, sizeof(vec4),
	//		&m_PAS_ConstStruct_VS.vecCamPos, sizeof(vec4));

	//memcpy_s(&m_NowPlanet->m_PAS_EPCS_PS.vecPlanetPos, sizeof(vec4),
	//	&m_PAS_ConstStruct_VS.vecCamPos, sizeof(vec4));

	//// 定数バッファ変更 //////////////////////////
	m_pDx11->SetConstBuffer(m_pConstantBafferVS, &m_PAS_ConstStruct_VS, sizeof(PAS_ConstStruct_VS));
	m_pDx11->SetConstBuffer(m_pConstantBafferPS, &m_PAS_ConstStruct_PS, sizeof(PAS_ConstStruct_PS));
	//m_pDx11->SetConstBuffer(m_NowPlanet->m_pConstantBafferPS,
	//						&m_NowPlanet->m_PAS_EPCS_PS, sizeof(m_NowPlanet->m_PAS_EPCS_PS));

	// Set Const Buffer //////////////////////////
	m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBafferVS);
	m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBafferPS);

	ID3D11ShaderResourceView* pnull = nullptr;
	float Color_Black_RVxyzt[4] = { 0, 0, 0, FLT_MAX };
	m_pContext->ClearRenderTargetView(m_pRenderVarRTView[0], Colors::Black);
	m_pContext->ClearRenderTargetView(m_pRenderVarRTView[1], Color_Black_RVxyzt);

	FOR(m_PlanetNum)
	{
		//m_pShaderBox->SetDepthState(m_pShaderBox->On_Less_WriteOK);
		
		// Clear Value RTV
		//FOR2(m_RVnum)m_pContext->ClearRenderTargetView(m_pRenderVarRTView[j], Colors::Black);

		// Set Planet Const Buffer //////////////
		m_pPlanet[i]->SetConstStruct(&m_pShaderBox->CameraPos, nullptr);

		// Set Texture //////////////////////////
		m_pDatabox->SetTexture(/*texidRefrectanceTexture*/m_pPlanet[i]->GetTexID_ref(), 0);
		m_pContext->PSSetShaderResources(1, 1, /*&m_TransmittanceView*/m_pPlanet[i]->GetRTV_tra());
		m_pContext->PSSetShaderResources(2, 1, /*&m_IrradianceView*/m_pPlanet[i]->GetRTV_irr());
		m_pContext->PSSetShaderResources(3, 1, /*&m_InscatterView*/m_pPlanet[i]->GetRTV_ins());

		/// pass 1 : Render t,T,mu to Texture ///
		m_pContext->OMSetBlendState(m_pRenderPalamBlendState, Colors::Black, 0xffffffff);
		//m_pShaderBox->ChangeRenderTarget(0, pOutRTV);
		m_pShaderBox->ChangeRenderTarget(0, m_pRenderVarRTView[0]);
		m_pShaderBox->ChangeRenderTarget(2, m_pRenderVarRTView[1]);
		m_pShaderBox->SetRTsToShader();
		m_p2Dsq->SetPixelShader(idPS_PrePAS);
		m_p2Dsq->Render();

		m_pShaderBox->ChangeRenderTarget(0, pOutRTV);
		m_pShaderBox->ChangeRenderTarget(2, nullptr);
		//m_pShaderBox->SetRTsToShader();

		//m_pShaderBox->SetDepthState(m_pShaderBox->On_AllOK_WriteNO);

		/// pass 2 : Render Ground ///
		m_pContext->OMSetBlendState(m_pDefaultBlendState, Colors::Black, 0xffffffff);
		m_pShaderBox->ChangeRenderTarget(0, pOutRTV);
		m_pShaderBox->SetRTsToShader();
		m_pContext->PSSetShaderResources(4, 1, &m_pRenderVarSRView[0]);
		m_pContext->PSSetShaderResources(5, 1, &m_pRenderVarSRView[1]);
		//m_pContext->PSSetShaderResources(6, 1, &pDepthSRV);
		m_p2Dsq->SetPixelShader(idPS_PAS_Ground);
		m_p2Dsq->Render();


		/// pass 3 : Render Atmosphere ///
		m_pContext->OMSetBlendState(m_pAddBlendState, Colors::Black, 0xffffffff);
		m_p2Dsq->SetPixelShader(idPS_PAS_Atmosphere);
		m_p2Dsq->Render();

		m_pContext->PSSetShaderResources(4, 1, &pnull);
		m_pContext->PSSetShaderResources(5, 1, &pnull);
		//m_pContext->PSSetShaderResources(6, 1, &pnull);
	}

	// Set Nearest Planet Texture //////////////////////////
	//m_pDatabox->SetTexture(/*texidRefrectanceTexture*/m_pPlanet[0]->GetTexID_ref(), 0);
	m_pContext->PSSetShaderResources(1, 1, /*&m_TransmittanceView*/m_pPlanet[0]->GetRTV_tra());
	m_pContext->PSSetShaderResources(2, 1, /*&m_IrradianceView*/m_pPlanet[0]->GetRTV_irr());
	m_pContext->PSSetShaderResources(3, 1, /*&m_InscatterView*/m_pPlanet[0]->GetRTV_ins());
	m_pContext->PSSetShaderResources(4, 1, &m_pRenderVarSRView[0]);
	m_pContext->PSSetShaderResources(5, 1, &m_pRenderVarSRView[1]);

	/// pass 4 : Render Sun ///
	m_pContext->OMSetBlendState(m_pAddBlendState, Colors::Black, 0xffffffff);
	m_p2Dsq->SetPixelShader(idPS_PAS_Sun);
	m_p2Dsq->Render();

	m_pContext->PSSetShaderResources(4, 1, &pnull);
	m_pContext->PSSetShaderResources(5, 1, &pnull);

	/// test pass : RefTex ///
	//m_p2Dsq->SetPixelShader(idPS_PAS);
	//m_p2Dsq->Render();

	m_pContext->PSSetShaderResources(4, 1, &pnull);
	m_pContext->OMSetBlendState(m_pDefaultBlendState, Colors::Black, 0xffffffff);
}


#ifdef _DEBUG
void PASRender::DebugDraw()
{
	//TCHAR txt[300];
	//_stprintf_s(txt, 299, _T("MainGameVS\ntime : %d"),
	//	pTimeMeasurer->GetTime()
	//	);
	//pDebugComM->Set(txt);
	//pDebugComM->End();
}
#endif


