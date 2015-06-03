#include"DebugTextureViewer.h"
#include"../base/DefineRendering.h"


DebugTextureViewer::DebugTextureViewer()
{
	m_SetSRVNum = 0;
	FOR(MAX_SRV_NUM)m_arrSRVs[i] = nullptr;
}

DebugTextureViewer::~DebugTextureViewer()
{
	SAFE_DELETE(m_p2Dsq);
	RELEASE(m_pConstantBafferVS);
	RELEASE(m_pConstantBafferPS);
}


void DebugTextureViewer::Init(DirectX11Base* pdx11, ShaderBox* pSB)
{
	m_pDx11 = pdx11;
	m_pContext = pdx11->GetContext();
	m_pShaderBox = pSB;

	// シェーダ作成
	m_p2Dsq = new D3D2DSQUARE(pdx11, pSB, DefRender.RenderTargetX, DefRender.RenderTargetY);
	m_p2Dsq->Init();
	m_p2Dsq->SetVertexShader(m_pShaderBox->CreateVertexShader(_T("data/hlsl/2DDrawer_VS.cso")));
	m_p2Dsq->SetPixelShader(m_pShaderBox->CreatePixelShader(_T("data/hlsl/PS_RefTex.cso")));
	
	//定数バッファ作成
	m_pDx11->CreateConstantBuffer(&m_pConstantBafferVS, NULL, sizeof(m2DDWConstVS), D3D11_CPU_ACCESS_WRITE);
	m_pDx11->CreateConstantBuffer(&m_pConstantBafferPS, NULL, sizeof(vec4), D3D11_CPU_ACCESS_WRITE);
}


void DebugTextureViewer::Set(ID3D11ShaderResourceView* p)
{
	if (m_SetSRVNum >= MAX_SRV_NUM)return;

	m_arrSRVs[m_SetSRVNum] = p;
	++m_SetSRVNum;
}


void DebugTextureViewer::TurnOnOff()
{
	m_RenderFlag = !m_RenderFlag;
}


void DebugTextureViewer::Render()
{
	if (m_RenderFlag == false)return;

	FOR(m_SetSRVNum)
	{
		//パラメータ計算
		m_ConstVS.Width_Shift.x = 1.f / (float)m_SetSRVNum * 0.9;
		m_ConstVS.Width_Shift.y = 1.f / (float)m_SetSRVNum * 0.9;
		m_ConstVS.Width_Shift.z = (2 * i + 1 - m_SetSRVNum) * 1.f / (float)m_SetSRVNum;
		m_ConstVS.Width_Shift.w = -1.f + m_ConstVS.Width_Shift.x;
		m_ConstVS.FakeDepth = 0;

		// VSへ定数バッファ設定
		m_pDx11->SetConstBuffer(m_pConstantBafferVS, reinterpret_cast<void*>(&m_ConstVS), sizeof(m_ConstVS));
		m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBafferVS);
		// Set Texture 
		m_pDx11->SetConstBuffer(m_pConstantBafferPS, reinterpret_cast<void*>(&m_alpha), sizeof(float));
		m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBafferPS);
		m_pContext->PSSetShaderResources(0, 1, &m_arrSRVs[i]);
		// 描画
		m_p2Dsq->Render();
	}

	m_SetSRVNum = 0;
}



