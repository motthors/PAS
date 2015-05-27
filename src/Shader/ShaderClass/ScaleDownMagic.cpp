#include"ScaleDownMagic.h"
#include"../../base/DefineRendering.h"

ScaleDownMagic::ScaleDownMagic()
{
	strcpy_s(m_FileName,100,"data/Effect/2DPostEffect.fx");
}

ScaleDownMagic::~ScaleDownMagic()
{
	SAFE_DELETE(m_p2Dsq);
}

void ScaleDownMagic::Init(DirectX11Base* pDx, ShaderBox* pSB, DXDEVICE* pdev, DXCONTEXT* pcnt)
{
	ShaderBase::Init(m_pDx11, pSB, pdev, pcnt);
	m_p2Dsq = new D3D2DSQUARE(pDx, pSB, DefRender.WindowX, DefRender.WindowY);
	m_p2Dsq->Init();
	m_p2Dsq->SetPixelShader(m_pShaderBox->CreatePixelShader(_T("data/hlsl/PS_ScaleDown.cso")));
}

void ScaleDownMagic::Render(ID3D11RenderTargetView* pOutRTV, ID3D11ShaderResourceView* pInSRV)
{



	m_pShaderBox->ChangeRenderTarget(0, pOutRTV);
	m_pShaderBox->ChangeRenderTarget(1, nullptr);
	m_pShaderBox->SetRTsToShader();

	m_pContext->PSSetShaderResources(0, 1, &pInSRV);
	//pEffect->SetTexture("t0", pShaderM->pHDRBlurTex);
	//pEffect->SetTexture("t0", pShaderM->pFixedSizeTex);
	//pd3ddev->SetRenderTarget(0, pShaderM->pDefaultBackBuffer);
	//pd3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(1, 0, 0, 0), 1.0f, 0);
	//pEffect->SetTechnique("Main");
	//pEffect->Begin( NULL, 0 );
	//pEffect->BeginPass(0);

	m_p2Dsq->Render();

	//pEffect->EndPass();
	//pEffect->End();
}
