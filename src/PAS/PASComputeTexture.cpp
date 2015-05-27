#include"PASComputeTexture.h"
#include"../base/DefineRendering.h"
#include"DDSTextureLoader.h"

PASComputeTexture::PASComputeTexture()
{
	m_p2Dsq = nullptr;
	m_pShaderBox = nullptr;
	pLogM = nullptr;
	ScatterNum = 3;

	PrerenderSwitchFlag = 1;
	prerenderflag = false;

	FOR(10)m_pComputeShader[i] = 0;
}


PASComputeTexture::~PASComputeTexture()
{
	// Texture
	RELEASE(m_RWTransmittanceTexture);
	RELEASE(m_RWIrradianceTexture);
	RELEASE(m_RWInscatterTexture);
	RELEASE(m_deltaETexture);
	RELEASE(m_deltaSRTexture);
	RELEASE(m_deltaSMTexture);
	RELEASE(m_deltaJTexture);
	RELEASE(m_IrradianceAddTemp);
	RELEASE(m_InscatterAddTemp);
	// SR View
	RELEASE(m_RWTransmittanceView);
	RELEASE(m_RWIrradianceView);
	RELEASE(m_RWInscatterView);
	RELEASE(m_deltaEView);
	RELEASE(m_deltaSRView);
	RELEASE(m_deltaSMView);
	RELEASE(m_deltaJView);
	RELEASE(m_IrradViewTemp);
	RELEASE(m_InscatViewTemp);
	// UA View
	RELEASE(m_TransUAV);
	RELEASE(m_IrradUAV);
	RELEASE(m_InscatUAV);
	RELEASE(m_deltaEUAV);
	RELEASE(m_deltaSRUAV);
	RELEASE(m_deltaSMUAV);
	RELEASE(m_deltaJUAV);
	RELEASE(m_IrradUAVTemp);
	RELEASE(m_InscatUAVTemp);
	// Cont Baffer
	RELEASE(m_pConstantBafferVS);
	RELEASE(m_pConstantBafferPS);
	RELEASE(m_pConstantBafferCS2);

	// CS
	FOR(10)RELEASE(m_pComputeShader[i]);


	SAFE_DELETE(m_p2Dsq);
}


void PASComputeTexture::Init( 
	DirectX11Base* pdx11,
	DataBox* pDB, 
	MatrixBoxManager* pMB,
	LogManager*	pLM,
	ShaderBox* pSB
	)
{
	PASRender::Init(pdx11, pDB, pMB, pSB);
	
	prerenderflag = false;
	//TextureTestViewFlag = 0;
	ResetVariable();

	pLogM = pLM;

	SetPlanet();

	//m_pDatabox = pDB;
	//pMatBoxM = pMB;
	//m_pDx11 = pdx11;
	//m_pDevice = m_pDx11->GetDevice();
	//m_pContext = m_pDx11->GetContext();
	//m_pShaderBox = pSB;
	
	// ピクセルシェーダ
	//idPS_PAS = m_pShaderBox->CreatePixelShader(_T("data/hlsl/PAS_PS.cso"));
	//idPS_PrePAS = m_pShaderBox->CreatePixelShader(_T("data/hlsl/PAS_PrePS.cso"));
	//idPS_DefPAS = m_pShaderBox->CreatePixelShader(_T("data/hlsl/PS_RefTex3D.cso"));

	//m_p2Dsq = new D3D2DSQUARE(pdx11, pSB, DefRender.RenderTargetX, DefRender.RenderTargetY);
	//m_p2Dsq->Init();
	//m_p2Dsq->SetVertexShader(m_pShaderBox->CreateVertexShader(_T("data/hlsl/PAS_VS.cso")));
	//m_p2Dsq->SetPixelShader(idPS_PAS);

	// 計算シェーダ
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[0], _T("data/hlsl/PAS_CS0_tra.cso"));
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[1], _T("data/hlsl/PAS_CS1_irr1.cso"));
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[2], _T("data/hlsl/PAS_CS2_ins1.cso"));
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[3], _T("data/hlsl/PAS_CS3_irrCpy.cso"));
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[4], _T("data/hlsl/PAS_CS4_insCpy.cso"));
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[5], _T("data/hlsl/PAS_CS5_CompJ.cso"));
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[6], _T("data/hlsl/PAS_CS6_irrN.cso"));
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[7], _T("data/hlsl/PAS_CS7_insN.cso"));
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[8], _T("data/hlsl/PAS_CS3_irrCpy.cso"));
	m_pShaderBox->CreateComputeShader(&m_pComputeShader[9], _T("data/hlsl/PAS_CS9_insCpyN.cso"));

	//定数バッファ作成
	//m_pDx11->CreateConstantBuffer(&m_pConstantBafferVS, NULL, sizeof(PAS_ConstStruct_VS), D3D11_CPU_ACCESS_WRITE);
	//m_pDx11->CreateConstantBuffer(&m_pConstantBafferPS, NULL, sizeof(PAS_ConstStruct_PS), D3D11_CPU_ACCESS_WRITE);
	m_pDx11->CreateConstantBuffer(&m_pConstantBafferCS2, NULL, sizeof(PAS_ConstStruct_CS2), D3D11_CPU_ACCESS_WRITE);

	PASComputeTexture::CreateTexture();

}

void PASComputeTexture::SetPlanet()
{
	PASPlanetStruct::EachPlanetConstStruct_PS* pCS = m_NowPlanet->GetConstStructPS();
	m_p_Rg = &pCS->Rg;
	m_p_Rt = &pCS->Rt;
	m_p_HR = &pCS->HR;
	m_p_HM = &pCS->HM;
	m_p_betaR[0] = &pCS->betaR.x;
	m_p_betaR[1] = &pCS->betaR.y;
	m_p_betaR[2] = &pCS->betaR.z;
	m_p_betaMSca[0] = &pCS->betaMSca.x;
	m_p_betaMSca[1] = &pCS->betaMSca.y;
	m_p_betaMSca[2] = &pCS->betaMSca.z;
	m_p_Color1[0] = &pCS->vecColor1.x;
	m_p_Color1[1] = &pCS->vecColor1.y;
	m_p_Color1[2] = &pCS->vecColor1.z;
	m_p_Color2[0] = &pCS->vecColor2.x;
	m_p_Color2[1] = &pCS->vecColor2.y;
	m_p_Color2[2] = &pCS->vecColor2.z;
	m_p_Color3[0] = &pCS->vecColor3.x;
	m_p_Color3[1] = &pCS->vecColor3.y;
	m_p_Color3[2] = &pCS->vecColor3.z;
	m_p_mieG = &pCS->mieG;
}

void PASComputeTexture::ResetVariable()
//PAS定数変数初期化
{
	prerenderflag = false;
	m_NowPlanet->m_PAS_EPCS_PS.Rg = 6360.0f;
	m_NowPlanet->m_PAS_EPCS_PS.Rt = 6420.0f;
	m_NowPlanet->m_PAS_EPCS_PS.HR = 6.0f;
	m_NowPlanet->m_PAS_EPCS_PS.betaR = vec3(5.8e-3f, 1.35e-2f, 3.31e-2f);
	m_PAS_ConstStruct_PS.SunDir = vec3(1.f, 1.f, 1.f);
	SimpleSetting_Default();
}

void PASComputeTexture::SimpleSetting_ClearSky()
{
	m_NowPlanet->m_PAS_EPCS_PS.HM = 1.2f;
	m_NowPlanet->m_PAS_EPCS_PS.betaMSca = vec3(20e-3f, 20e-3f, 20e-3f);
	m_NowPlanet->m_PAS_EPCS_PS.betaMEx = vec3(20e-3f / 0.9f, 20e-3f / 0.9f, 20e-3f / 0.9f);
	m_NowPlanet->m_PAS_EPCS_PS.mieG = 0.76;
}
void PASComputeTexture::SimpleSetting_Default()
{
	m_NowPlanet->m_PAS_EPCS_PS.HM = 1.2f;
	m_NowPlanet->m_PAS_EPCS_PS.betaMSca = vec3(4e-3f, 4e-3f, 4e-3f);
	m_NowPlanet->m_PAS_EPCS_PS.betaMEx = vec3(4e-3f / 0.9f, 4e-3f / 0.9f, 4e-3f / 0.9f);
	m_NowPlanet->m_PAS_EPCS_PS.mieG = 0.8f;
}
void PASComputeTexture::SimpleSetting_PartryCloudy()
{
	m_NowPlanet->m_PAS_EPCS_PS.HM = 3.0f;
	m_NowPlanet->m_PAS_EPCS_PS.betaMSca = vec3(3e-3f, 3e-3f, 3e-3f);
	m_NowPlanet->m_PAS_EPCS_PS.betaMEx = vec3(3e-3f / 0.9f, 3e-3f / 0.9f, 3e-3f / 0.9f);
	m_NowPlanet->m_PAS_EPCS_PS.mieG = 0.65;
}


void PASComputeTexture::CreateTexture()
{
	//PASRender::CreateTexture();
	//texidRefrectanceTexture = m_pDatabox->GetTexture(_T("data/earth.dds"));

	//ID3D11Resource* TexTra = reinterpret_cast<ID3D11Resource*>(m_SelectingPlanet->m_TransmittanceTexture);
	//ID3D11Resource* TexIrr = reinterpret_cast<ID3D11Resource*>(m_SelectingPlanet->m_IrradianceTexture);
	//ID3D11Resource* TexIns = reinterpret_cast<ID3D11Resource*>(m_SelectingPlanet->m_InscatterTexture);
	//ID3D11Resource* TexTra = m_TransmittanceTexture;
	//ID3D11Resource* TexIrr = m_IrradianceTexture;
	//ID3D11Resource* TexIns = m_InscatterTexture;


	m_pShaderBox->CreateTexture2D(&m_RWTransmittanceTexture, nullptr, 0, TRANS_W, TRANS_H, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_RWTransmittanceTexture, &m_RWTransmittanceView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);
	m_pShaderBox->CreateUAV(m_RWTransmittanceTexture, &m_TransUAV, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_UAV_DIMENSION_TEXTURE2D);
	////CreateTexture_SRV_UAV(L"trans.dds", &m_TransmittanceView, &m_TransUAV);

	m_pShaderBox->CreateTexture2D(&m_RWIrradianceTexture, nullptr, 0, SKY_W, SKY_H, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_RWIrradianceTexture, &m_RWIrradianceView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);
	m_pShaderBox->CreateUAV(m_RWIrradianceTexture, &m_IrradUAV, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_UAV_DIMENSION_TEXTURE2D);
	////CreateTexture_SRV_UAV(L"irrad.dds", &m_IrradianceView, &m_IrradUAV);

	m_pShaderBox->CreateTexture3D(&m_RWInscatterTexture, nullptr, 0, RES_MU_S * RES_NU, RES_MU, RES_R, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_RWInscatterTexture, &m_RWInscatterView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE3D);
	m_pShaderBox->CreateUAV(m_RWInscatterTexture, &m_InscatUAV, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_UAV_DIMENSION_TEXTURE3D);
	////CreateTexture_SRV_UAV(L"inscat.dds", &m_InscatterView, &m_InscatUAV, 3);

	//////////////////////////////////////////////////////////////////////////////////////////

	m_pShaderBox->CreateTexture2D(&m_deltaETexture, nullptr, 0, SKY_W, SKY_H, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_deltaETexture, &m_deltaEView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);
	m_pShaderBox->CreateUAV(m_deltaETexture, &m_deltaEUAV, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_UAV_DIMENSION_TEXTURE2D);

	m_pShaderBox->CreateTexture3D(&m_deltaSRTexture, nullptr, 0, RES_MU_S * RES_NU, RES_MU, RES_R, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_deltaSRTexture, &m_deltaSRView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE3D);
	m_pShaderBox->CreateUAV(m_deltaSRTexture, &m_deltaSRUAV, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_UAV_DIMENSION_TEXTURE3D);

	m_pShaderBox->CreateTexture3D(&m_deltaSMTexture, nullptr, 0, RES_MU_S * RES_NU, RES_MU, RES_R, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_deltaSMTexture, &m_deltaSMView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE3D);
	m_pShaderBox->CreateUAV(m_deltaSMTexture, &m_deltaSMUAV, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_UAV_DIMENSION_TEXTURE3D);

	m_pShaderBox->CreateTexture3D(&m_deltaJTexture, nullptr, 0, RES_MU_S * RES_NU, RES_MU, RES_R, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_deltaJTexture, &m_deltaJView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE3D);
	m_pShaderBox->CreateUAV(m_deltaJTexture, &m_deltaJUAV, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_UAV_DIMENSION_TEXTURE3D);

	//////////////////////////////////////////////////////////////////////////////////////////
	
	m_pShaderBox->CreateTexture2D(&m_IrradianceAddTemp, nullptr, 0, SKY_W, SKY_H, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_IrradianceAddTemp, &m_IrradViewTemp, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);
	m_pShaderBox->CreateUAV(m_IrradianceAddTemp, &m_IrradUAVTemp, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_UAV_DIMENSION_TEXTURE2D);

	m_pShaderBox->CreateTexture3D(&m_InscatterAddTemp, nullptr, 0, RES_MU_S * RES_NU, RES_MU, RES_R, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_InscatterAddTemp, &m_InscatViewTemp, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE3D);
	m_pShaderBox->CreateUAV(m_InscatterAddTemp, &m_InscatUAVTemp, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_UAV_DIMENSION_TEXTURE3D);
	
}

//
//void PASComputeTexture::LoadPASTextureFromFile(ID3D11Resource* pTex, wchar_t* fname, int DataNnum)
//{
//	HRESULT hr;
//
//	// テクスチャデータファイルオープン
//	FILE* fp;
//	_wfopen_s(&fp, fname, L"wb");
//	if (fp == NULL)
//	{
//		ErrM.SetClassName(_T("PASComputeTexture::CreatePASView::fopen"));
//		ErrM.SetErrorText(_T("テクスチャデータファイルのオープンに失敗"));
//		ErrM.SetHResult(USER_ERROR);
//		throw &ErrM;
//	}
//
//
//	// テクスチャをロック
//	D3D11_MAPPED_SUBRESOURCE LockedRect;
//	hr = m_pContext->Map(pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &LockedRect); 
//	
//	ZeroMemory(LockedRect.pData, DataNnum*sizeof(vec4));
//	vec4 *pTexture = reinterpret_cast<vec4*>(LockedRect.pData);
//
//	// 読み込み
//	FOR(DataNnum)
//	{
//		fread_s(&pTexture[i], sizeof(vec4)*DataNnum, sizeof(vec4), DataNnum, fp);
//	}
//	
//
//	// テクスチャのロック解除
//	m_pContext->Unmap(pTex, 0);
//
//	fclose(fp);
//}


void PASComputeTexture::SaveTexture()
{
	//Create Buffer(Texture)
	m_pShaderBox->CreateTexture2D(&m_TransSaveTex, nullptr, 0, TRANS_W, TRANS_H, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_USAGE_STAGING);
	m_pShaderBox->CreateTexture2D(&m_IrradSaveTex, nullptr, 0, SKY_W, SKY_H, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_USAGE_STAGING);
	m_pShaderBox->CreateTexture3D(&m_InscaSaveTex, nullptr, 0, RES_MU_S * RES_NU, RES_MU, RES_R, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_USAGE_STAGING);

	//Copy Buffer
	m_pContext->CopyResource(m_TransSaveTex, *m_NowPlanet->GetTex_tra());
	m_pContext->CopyResource(m_IrradSaveTex, *m_NowPlanet->GetTex_irr());
	m_pContext->CopyResource(m_InscaSaveTex, *m_NowPlanet->GetTex_ins());

	//Save Buffer
	SaveTexture_Subfunction(m_TransSaveTex, "data/trans.pastex", TRANS_W * TRANS_H);
	SaveTexture_Subfunction(m_IrradSaveTex, "data/irrad.pastex", SKY_W * SKY_H);
	SaveTexture_Subfunction(m_InscaSaveTex, "data/insca.pastex", RES_MU_S * RES_NU * RES_MU, RES_R);

	//release
	RELEASE(m_TransSaveTex);
	RELEASE(m_IrradSaveTex);
	RELEASE(m_InscaSaveTex);
}


void PASComputeTexture::SaveTexture_Subfunction(ID3D11Resource* pTex, char* FileName, UINT TexSize, UINT DivWrite)
{
	FILE* fp = 0;
	fopen_s(&fp, FileName, "wb");
	if (fp == 0)
	{
		ErrM.SetClassName(_T("PASComputeTexture::SaveTexture::fopen"));
		ErrM.SetErrorText(_T("保存用テクスチャが開けない"));
		ErrM.SetHResult(USER_ERROR);
		throw &ErrM;
	}
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hr = m_pContext->Map(pTex, 0, D3D11_MAP_READ, 0, &resource);
	if (SUCCEEDED(hr))
	{
		float* param = reinterpret_cast<float*>(resource.pData);
		FOR(DivWrite)
		{
			fwrite(param + 4 * i*TexSize, sizeof(float), 4*TexSize, fp);
		}
		m_pContext->Unmap(pTex, 0);
	}
	else
	{
		ErrM.SetClassName(_T("PASComputeTexture::SaveTexture::Map"));
		ErrM.SetErrorText(_T("保存テクスチャのMapに失敗"));
		ErrM.SetHResult(hr);
		throw &ErrM;
	}
	fclose(fp);
}

void PASComputeTexture::StartPrerender()
{
	if (!prerenderflag)
	{
		prerenderflag = true;
		PrerenderSwitchFlag = PF_TRNAS1;
		ScatterCounter = 2;
		ScatterNum = 3;
		pLogM->AddComment(_T("start"));
	}
}

void PASComputeTexture::AddMultiScatter()
{
	if (!prerenderflag)
	{
		prerenderflag = true;
		PrerenderSwitchFlag = PF_L_DELTAJ;
		ScatterNum++;
		pLogM->AddComment(_T("start"));
	}
}

void PASComputeTexture::prerender_FlagManage()
{
	if (PrerenderSwitchFlag == PF_L_COPY_Ins) // 多重散乱ループの最後だったら
	{
		if (ScatterCounter < ScatterNum) // まだ散乱計算をするならこっち
		{
			PrerenderSwitchFlag = PF_L_DELTAJ; // 散乱計算の最初
			++ScatterCounter;
		}
		else // 散乱計算はもういいならテクスチャに書き込み
		{
			PrerenderSwitchFlag = PF_TEXTURE_DRAW;
		}
	}
	else ++PrerenderSwitchFlag;
}


void PASComputeTexture::prerender()
{
	// D3D11に怒られるからシェーダからテクスチャをはずす
	ID3D11ShaderResourceView* blank = nullptr;
	m_pContext->PSSetShaderResources(0, 1, &blank);
	m_pContext->PSSetShaderResources(1, 1, &blank);
	m_pContext->PSSetShaderResources(2, 1, &blank);
	m_pContext->PSSetShaderResources(3, 1, &blank);
	m_pContext->CSSetShaderResources(0, 1, &blank);
	m_pContext->CSSetShaderResources(1, 1, &blank);
	m_pContext->CSSetShaderResources(2, 1, &blank);
	m_pContext->CSSetShaderResources(3, 1, &blank);
	m_pContext->CSSetShaderResources(4, 1, &blank);
	// CSにサンプラーセット
	m_pContext->CSSetSamplers(0, 1, &m_pDefaultSampler);


	// Precomputed Atmospheric Scattering アルゴリズム処理
	switch (PrerenderSwitchFlag)
	{
	case PF_TRNAS1:			prerender_trans1();	break;
	case PF_IRRAD1:			prerender_irrad1(); break;
	case PF_INSCAT1:		prerender_insct1(); break;
	case PF_COPY_IRRAD:		prerender_CpyIr1(); break;
	case PF_COPY_INSCAT:	prerender_CpyIs1(); break;
		//for (int i = 2; i < ScatterNum; ++i)
	case PF_L_DELTAJ:		prerender_deltaJ(); break;
	case PF_L_IRRADN:		prerender_IrradN(); break;
	case PF_L_INSCATN:		prerender_insctN(); break;
	case PF_L_COPY_Irr:		prerender_CpyIrN(); break;
	case PF_L_COPY_Ins:		prerender_CpyIsN(); break;
	case PF_TEXTURE_DRAW:

		//hr = D3DXFillTexture(TransmittanceTexture, FillTex2, (LPVOID)TransmittanceArr);
		//if (FAILED(hr))goto ERR;
		//hr = D3DXFillTexture(IrradianceTexture, FillTex2, (LPVOID)IrradianceArr);
		//if (FAILED(hr))goto ERR;
		//hr = D3DXFillVolumeTexture(InscatterTexture, FillTex3, (LPVOID)InscatterArr);
		//if (FAILED(hr))goto ERR;

		//D3DXSaveTextureToFile(_T("trans.dds"), D3DXIFF_DDS, TransmittanceTexture, NULL);
		//D3DXSaveTextureToFile(_T("irrad.dds"), D3DXIFF_DDS, IrradianceTexture, NULL);
		//LPDIRECT3DVOLUME9 pInscatVolume;
		//InscatterTexture->GetVolumeLevel(0, &pInscatVolume);
		//D3DXSaveVolumeToFile(_T("inscat.dds"), D3DXIFF_DDS, pInscatVolume, NULL, NULL);
		//RELEASE(pInscatVolume);

		pLogM->AddComment(_T("**** Complete Computing All ****"));
		prerenderflag = false;
		break;
	}

	prerender_FlagManage();

	return;
}

void PASComputeTexture::prerender_trans1()
{
	m_pContext->CSSetShader(m_pComputeShader[0], NULL, 0);
	m_pContext->CSSetConstantBuffers(0, 1, &m_pConstantBafferPS);
	m_pContext->CSSetConstantBuffers(1, 1, &m_NowPlanet->m_pConstantBafferPS);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_TransUAV, NULL);
	m_pContext->Dispatch(1, 16, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);
	//m_pContext->CopyResource(m_TransmittanceTexture, m_RWTransmittanceTexture);
	m_pContext->CopyResource(*m_NowPlanet->GetTex_tra(), m_RWTransmittanceTexture);
}
void PASComputeTexture::prerender_irrad1()
{
	m_pContext->CSSetShader(m_pComputeShader[1], NULL, 0);
	m_pContext->CSSetConstantBuffers(0, 1, &m_pConstantBafferPS);
	m_pContext->CSSetConstantBuffers(1, 1, &m_NowPlanet->m_pConstantBafferPS);
	m_pContext->CSSetShaderResources(1, 1, m_NowPlanet->GetRTV_tra());
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_deltaEUAV, NULL);
	m_pContext->Dispatch(1, 1, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);
}
void PASComputeTexture::prerender_insct1()
{
	m_pContext->CSSetShader(m_pComputeShader[2], NULL, 0);
	m_pContext->CSSetConstantBuffers(0, 1, &m_pConstantBafferPS);
	m_pContext->CSSetConstantBuffers(1, 1, &m_NowPlanet->m_pConstantBafferPS);
	m_pContext->CSSetShaderResources(1, 1, m_NowPlanet->GetRTV_tra());
	m_pContext->CSSetShaderResources(2, 1, m_NowPlanet->GetRTV_irr());
	m_pContext->CSSetUnorderedAccessViews(2, 1, &m_deltaSRUAV, NULL);
	m_pContext->CSSetUnorderedAccessViews(3, 1, &m_deltaSMUAV, NULL);
	m_pContext->Dispatch(RES_NU, RES_MU, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(2, 1, &pNull, (UINT*)&pNull);
	m_pContext->CSSetUnorderedAccessViews(3, 1, &pNull, (UINT*)&pNull);
}
void PASComputeTexture::prerender_CpyIr1()
{
	m_PAS_ConstStruct_CS2.k = 0;
	m_pDx11->SetConstBuffer(m_pConstantBafferCS2, &m_PAS_ConstStruct_CS2, sizeof(PAS_ConstStruct_CS2));
	m_pContext->CSSetConstantBuffers(1, 1, &m_NowPlanet->m_pConstantBafferPS);
	m_pContext->CSSetConstantBuffers(2, 1, &m_pConstantBafferCS2);

	m_pContext->CSSetShader(m_pComputeShader[3], NULL, 0);
	m_pContext->CSSetShaderResources(0, 1, &m_deltaEView);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_IrradUAV, NULL);
	m_pContext->Dispatch(1, 1, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);
	//m_pContext->CopyResource(m_IrradianceTexture, m_RWIrradianceTexture);
	m_pContext->CopyResource(*m_NowPlanet->GetTex_irr(), m_RWIrradianceTexture);
}
void PASComputeTexture::prerender_CpyIs1()
{
	m_pContext->CSSetShader(m_pComputeShader[4], NULL, 0);
	m_pContext->CSSetShaderResources(3, 1, &m_deltaSRView);
	m_pContext->CSSetShaderResources(4, 1, &m_deltaSMView);
	m_pContext->CSSetUnorderedAccessViews(2, 1, &m_InscatUAV, NULL);
	m_pContext->Dispatch(RES_NU, RES_MU, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(2, 1, &pNull, (UINT*)&pNull);
	//m_pContext->CopyResource(m_InscatterTexture, m_RWInscatterTexture);
	m_pContext->CopyResource(*m_NowPlanet->GetTex_ins(), m_RWInscatterTexture);
}
void PASComputeTexture::prerender_deltaJ()
{
	m_PAS_ConstStruct_CS2.k = (ScatterCounter == 2 ? 1.0f : 0.0f);
	m_pDx11->SetConstBuffer(m_pConstantBafferCS2, &m_PAS_ConstStruct_CS2, sizeof(PAS_ConstStruct_CS2));
	m_pContext->CSSetConstantBuffers(1, 1, &m_NowPlanet->m_pConstantBafferPS);
	m_pContext->CSSetConstantBuffers(2, 1, &m_pConstantBafferCS2);

	m_pContext->CSSetShader(m_pComputeShader[5], NULL, 0);
	m_pContext->CSSetShaderResources(1, 1, m_NowPlanet->GetRTV_tra());
	m_pContext->CSSetShaderResources(2, 1, &m_deltaEView);
	m_pContext->CSSetShaderResources(3, 1, &m_deltaSRView);
	m_pContext->CSSetShaderResources(4, 1, &m_deltaSMView);
	m_pContext->CSSetUnorderedAccessViews(2, 1, &m_deltaJUAV, NULL);
	m_pContext->Dispatch(RES_NU, RES_MU, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(2, 1, &pNull, (UINT*)&pNull);
}
void PASComputeTexture::prerender_IrradN()
{
	m_PAS_ConstStruct_CS2.k = (ScatterCounter == 2 ? 1.0f : 0.0f);
	m_pDx11->SetConstBuffer(m_pConstantBafferCS2, &m_PAS_ConstStruct_CS2, sizeof(PAS_ConstStruct_CS2));
	m_pContext->CSSetConstantBuffers(1, 1, &m_NowPlanet->m_pConstantBafferPS);
	m_pContext->CSSetConstantBuffers(2, 1, &m_pConstantBafferCS2);

	m_pContext->CSSetShader(m_pComputeShader[6], NULL, 0);
	m_pContext->CSSetShaderResources(0, 1, m_NowPlanet->GetRTV_irr());
	m_pContext->CSSetShaderResources(1, 1, m_NowPlanet->GetRTV_tra());
	m_pContext->CSSetShaderResources(3, 1, &m_deltaSRView);
	m_pContext->CSSetShaderResources(4, 1, &m_deltaSMView);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_deltaEUAV, NULL);
	m_pContext->CSSetUnorderedAccessViews(1, 1, &m_IrradUAVTemp, NULL);
	m_pContext->Dispatch(1, 1, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);
	m_pContext->CSSetUnorderedAccessViews(1, 1, &pNull, (UINT*)&pNull);
}
void PASComputeTexture::prerender_insctN()
{
	m_PAS_ConstStruct_CS2.k = (ScatterCounter == 2 ? 1.0f : 0.0f);
	m_pDx11->SetConstBuffer(m_pConstantBafferCS2, &m_PAS_ConstStruct_CS2, sizeof(PAS_ConstStruct_CS2));
	m_pContext->CSSetConstantBuffers(1, 1, &m_NowPlanet->m_pConstantBafferPS);
	m_pContext->CSSetConstantBuffers(2, 1, &m_pConstantBafferCS2);

	m_pContext->CSSetShader(m_pComputeShader[7], NULL, 0);
	m_pContext->CSSetShaderResources(1, 1, m_NowPlanet->GetRTV_tra());
	m_pContext->CSSetShaderResources(3, 1, &m_deltaJView);
	m_pContext->CSSetShaderResources(4, 1, m_NowPlanet->GetRTV_ins());
	m_pContext->CSSetUnorderedAccessViews(2, 1, &m_deltaSRUAV, NULL);
	m_pContext->CSSetUnorderedAccessViews(3, 1, &m_InscatUAVTemp, NULL);
	m_pContext->Dispatch(RES_NU, RES_MU, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(2, 1, &pNull, (UINT*)&pNull);
	m_pContext->CSSetUnorderedAccessViews(3, 1, &pNull, (UINT*)&pNull);
}
void PASComputeTexture::prerender_CpyIrN()
{
	m_PAS_ConstStruct_CS2.k = 1.0f;
	m_pDx11->SetConstBuffer(m_pConstantBafferCS2, &m_PAS_ConstStruct_CS2, sizeof(PAS_ConstStruct_CS2));
	m_pContext->CSSetConstantBuffers(1, 1, &m_NowPlanet->m_pConstantBafferPS);
	m_pContext->CSSetConstantBuffers(2, 1, &m_pConstantBafferCS2);

	m_pContext->CSSetShader(m_pComputeShader[8], NULL, 0);
	m_pContext->CSSetShaderResources(0, 1, &m_deltaEView);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_IrradUAV, NULL);
	m_pContext->Dispatch(1, 1, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);
	//m_pContext->CopyResource(m_IrradianceTexture, m_RWIrradianceTexture);
	m_pContext->CopyResource(*m_NowPlanet->GetTex_irr(), m_RWIrradianceTexture);
}
void PASComputeTexture::prerender_CpyIsN()
{
	m_pContext->CSSetShader(m_pComputeShader[9], NULL, 0);
	m_pContext->CSSetShaderResources(3, 1, &m_deltaSRView);
	m_pContext->CSSetShaderResources(4, 1, &m_InscatViewTemp);
	m_pContext->CSSetUnorderedAccessViews(2, 1, &m_InscatUAV, NULL);
	m_pContext->Dispatch(RES_NU, RES_MU, 1);

	ID3D11UnorderedAccessView*	pNull = NULL;
	m_pContext->CSSetUnorderedAccessViews(2, 1, &pNull, (UINT*)&pNull);
	//m_pContext->CopyResource(m_InscatterTexture, m_RWInscatterTexture);
	m_pContext->CopyResource(*m_NowPlanet->GetTex_ins(), m_RWInscatterTexture);
}



void PASComputeTexture::Move()
{
	if (prerenderflag)
	{
		prerender();
	}
}


void PASComputeTexture::Draw()
{

#ifdef _DEBUG
	DebugDraw();
#endif

}

//
//void PASComputeTexture::SetCameraData(vec* pCam)
//{
//	//vec vector;
//	//vector = pCamP;
//	//pEffect->SetVector("cam", &vector);
//	//vec3 sun = vec3(1, 0, 0);
//	//D3DXVec3Normalize(&sun, &sun);
//	//vector.x = sun.x;
//	//vector.y = sun.y;
//	//vector.z = sun.z;
//	//vector.w = 1.0;
//	//pEffect->SetVector("s", &vector);
//
//	//////// W*V*P
//	////pEffect->SetMatrix("matWorld", &matBoard);
//	////D3DXMatrixMultiply(pmatWVP, &matBoard, pmatView);
//	////D3DXMatrixMultiply(pmatWVP, pmatWVP, pmatProj);
//	////pEffect->SetMatrix("matWVP", pmatWVP);
//
//
//	////D3DXMatrixIdentity(&matBoard);
//	////memcpy_s(pmatWVP, sizeof(D3DXMATRIX), &matBoard, sizeof(D3DXMATRIX));
//	//D3DXMatrixInverse(&matBoard, nullptr, pmatView);
//	//pEffect->SetMatrix("matViewI", &matBoard);
//	//D3DXMatrixInverse(&matBoard, nullptr, pmatProj);
//	//pEffect->SetMatrix("matProjI", &matBoard);
//	//
//	m_vecCameraPos = *pCam;
//}



//void PASComputeTexture::Render()
//{
//	// 行列計算/////////////////////////////////////////
//	vec Determinant;
//	mat V = XMLoadFloat4x4(&m_pShaderBox->matView);
//	mat P = XMLoadFloat4x4(&m_pShaderBox->matProj);
//	V = XMMatrixInverse(&Determinant, V);
//	P = XMMatrixInverse(&Determinant, P);
//	V = XMMatrixTranspose(V);
//	P = XMMatrixTranspose(P);
//	//XMStoreFloat4x4(&m_PAS_ConstStruct_VS.matVIxPI, V*P);
//	XMStoreFloat4x4(&m_PAS_ConstStruct_VS.matVI, V);
//	XMStoreFloat4x4(&m_PAS_ConstStruct_VS.matPI, P);
//
//	//m_matVIxPI = XMMatrixMultiply(m_matViewI, m_matProjI);
//	//m_matVIxPI = XMMatrixInverse(&Determinant, mat);
//
//	m_PAS_ConstStruct_VS.vecCamPos.x = m_pShaderBox->CameraPos.x;
//	m_PAS_ConstStruct_VS.vecCamPos.y = m_pShaderBox->CameraPos.y;
//	m_PAS_ConstStruct_VS.vecCamPos.z = m_pShaderBox->CameraPos.z;
//	m_PAS_ConstStruct_VS.vecCamPos.w = 0.0f;
//	memcpy_s(&m_PAS_EPCS_PS.vecCamPos, sizeof(vec4),
//				&m_PAS_ConstStruct_VS.vecCamPos, sizeof(vec4));
//	m_PAS_EPCS_PS.coord = float(TextureTestViewFlag / 10) / (float)RES_MU_S;
//	
//
//	//// コンスタントバッファ変更 //////////////////////////
//	m_pDx11->SetConstBuffer(m_pConstantBafferVS, &m_PAS_ConstStruct_VS, sizeof(PAS_ConstStruct_VS));
//	m_pDx11->SetConstBuffer(m_pConstantBafferPS, &m_PAS_ConstStruct_PS, sizeof(PAS_ConstStruct_PS));
//
//
//	/// pass 1 : Render tTmur///
//
//
//
//	// Set Texture
//	m_pDatabox->SetTexture(texidRefrectanceTexture, 0);
//	m_pContext->PSSetShaderResources(1, 1, &m_TransmittanceView);
//	m_pContext->PSSetShaderResources(2, 1, &m_IrradianceView);
//	m_pContext->PSSetShaderResources(3, 1, &m_InscatterView);
//	m_p2Dsq->SetPixelShader(idPS_PAS);
//
//
//	// Set Const Buffer
//	m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBafferVS);
//	m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBafferPS);
//
//	m_p2Dsq->Render();
//}


#ifdef _DEBUG
void PASComputeTexture::DebugDraw()
{
	//TCHAR txt[300];
	//_stprintf_s(txt, 299, _T("MainGameVS\ntime : %d"),
	//	pTimeMeasurer->GetTime()
	//	);
	//pDebugComM->Set(txt);
	//pDebugComM->End();
}
#endif


