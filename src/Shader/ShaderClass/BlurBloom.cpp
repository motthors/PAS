#include"BlurBloom.h"
#include"../../base/DefineRendering.h"

BlurBloom::BlurBloom()
{
	FOR(BLUR_TEX_NUM)m_pBlurTex[i];
	FOR(BLUR_TEX_NUM)m_pBlurSRV[i];
	FOR(BLUR_TEX_NUM)m_pBlurUAV[i];
}

BlurBloom::~BlurBloom()
{
	FOR(BLUR_TEX_NUM)RELEASE(m_pBlurTex[i]);
	FOR(BLUR_TEX_NUM)RELEASE(m_pBlurSRV[i]);
	FOR(BLUR_TEX_NUM)RELEASE(m_pBlurUAV[i]);

	RELEASE(m_pBlurWorkTex);
	RELEASE(m_pBlurWorkSRV);
	RELEASE(m_pBlurWorkUAV);

	SAFE_DELETE(m_p2Dsq);

	RELEASE(m_pCScpy);
	RELEASE(m_pCScpy2);
	RELEASE(m_pCSx);
	RELEASE(m_pCSy);

	RELEASE(m_pConstBuffer);
}

void BlurBloom::Init(DirectX11Base* pDx, ShaderBox* pSB, DebugTextureViewer* pDTV)
{
	ShaderBase::Init(pDx, pSB, pDTV);
	m_Dispersion = 0.05f;
	m_Width = (float)DefRender.RenderTargetX;
	m_Height = (float)DefRender.RenderTargetY;

	//m_p2Dsq = new D3D2DSQUARE(pdx11, pSB, DefRender.RenderTargetX, DefRender.RenderTargetY);
	//p2Dsq = new D3D2DSQUARE(pdev, Width * 4, Height * 4);
	m_p2Dsq = new D3D2DSQUARE(pDx, pSB, DefRender.RenderTargetX, DefRender.RenderTargetY);
	m_p2Dsq->Init();
	m_p2Dsq->SetPixelShader(m_pShaderBox->CreatePixelShader(_T("data/hlsl/PS_Blur_Add.cso")));


	m_pShaderBox->CreateComputeShader(&m_pCScpy, _T("data/hlsl/CS_Blur_Copy.cso"));
	m_pShaderBox->CreateComputeShader(&m_pCScpy2, _T("data/hlsl/CS_Blur_Copy2.cso"));
	m_pShaderBox->CreateComputeShader(&m_pCSx, _T("data/hlsl/CS_Blur_X.cso"));
	m_pShaderBox->CreateComputeShader(&m_pCSy, _T("data/hlsl/CS_Blur_Y.cso"));

	m_pDx11->CreateConstantBuffer(&m_pConstBuffer, nullptr, sizeof(float) * 8, D3D11_CPU_ACCESS_WRITE);

	CreateSurface();
	SetConstStruct();
}

HRESULT BlurBloom::CreateSurface()
{
	//HRESULT hr;
	////�T�[�t�F�X����
	//hr = pd3ddev->CreateTexture( 
	//	Width, Height, 1,
	//	D3DUSAGE_RENDERTARGET,	//�K�{
	//	D3DFMT_A8R8G8B8,		//�t�H�[�}�b�g
	//	D3DPOOL_DEFAULT,		//���̂܂�
	//	&pBlurT1, 0				//NULL�Œ�
	//);
	//if(FAILED(hr))
	//	return hr;
	//hr = pBlurT1->GetSurfaceLevel( 0, &pBlurS1 );
	//if(FAILED(hr))
	//	return hr;

	//hr = pd3ddev->CreateTexture( 
	//	Width, Height, 1,
	//	D3DUSAGE_RENDERTARGET,	//�K�{
	//	D3DFMT_A8R8G8B8,		//�t�H�[�}�b�g
	//	D3DPOOL_DEFAULT,		//���̂܂�
	//	&pBlurT2, 0				//NULL�Œ�
	//);
	//if(FAILED(hr))
	//	return hr;
	//hr = pBlurT2->GetSurfaceLevel( 0, &pBlurS2 );
	//if(FAILED(hr))
	//	return hr;

	UINT t_width = 16 * 3 * 3 * 3;
	UINT t_height = 9 * 3 * 3 * 3;
	m_pShaderBox->CreateTexture2D(&m_pBlurWorkTex, nullptr, 0, t_width, t_width, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
	m_pShaderBox->CreateSRV(m_pBlurWorkTex, &m_pBlurWorkSRV, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_SRV_DIMENSION_TEXTURE2D);
	m_pShaderBox->CreateUAV(m_pBlurWorkTex, &m_pBlurWorkUAV, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_UAV_DIMENSION_TEXTURE2D);
	FOR(BLUR_TEX_NUM)
	{
		m_pShaderBox->CreateTexture2D(&m_pBlurTex[i], nullptr, 0, t_width, t_height, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
		m_pShaderBox->CreateSRV(m_pBlurTex[i], &m_pBlurSRV[i], DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_SRV_DIMENSION_TEXTURE2D);
		m_pShaderBox->CreateUAV(m_pBlurTex[i], &m_pBlurUAV[i], DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_UAV_DIMENSION_TEXTURE2D);
		t_width /= 3;
		t_height /= 3;
	}

	return S_OK;
}

void BlurBloom::SetConstStruct()
{
	int Division = 8;
	float total = 0.0f;
	
	// �K�E�X�֐��ɂ��d�݂̌v�Z
	FOR( Division )
	{
		float pos = (float)i * 2.0f;
		m_weight[i] = expf(-pos * pos * m_Dispersion);
		total += m_weight[i];
	}
	// �d�݂̋K�i��
	FOR(Division)m_weight[i] = m_weight[i] / total * 0.5f;
	m_pDx11->SetConstBuffer(m_pConstBuffer, m_weight, sizeof(float)*8);
	m_pContext->CSSetConstantBuffers(0, 1, &m_pConstBuffer);
}


void BlurBloom::Render()
{
	ID3D11UnorderedAccessView*	pNull = NULL;

	//pEffect->SetTechnique( "Main" );
	//UINT i;
	//pEffect->Begin( &i, 0 );

	////���P�x����
	//pEffect->SetTexture("t0", pShaderM->pFixedSizeTex );
	//pd3ddev->SetRenderTarget(0, pShaderM->pHDRBlurSurface );
	//pEffect->BeginPass(0);
	//p2Dsq->Render();
	//pEffect->EndPass();
	//
	//p2Dsq->Resize(Width,Height);
	//SetMatrixBlurX();
	//m_pContext->CopyResource(m_pShaderBox->pum_pTexF32[4], m_pShaderBox->pum_pTexF32[0]);
	m_pShaderBox->ChangeRenderTarget(0, nullptr);
	m_pShaderBox->SetRTsToShader();

	UINT sizeX = 16 * 3 * 3 * 3;
	UINT sizeY = 9 * 3 * 3 * 3;

	////////////////////////////////////////////////////////////////////////////////

	// copy  Src -> 1
	m_pContext->CSSetShaderResources(0, 1, &m_pShaderBox->pum_pSRVF32[0]);
	m_pContext->CSSetShader(m_pCScpy, NULL, 0);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pBlurUAV[0], NULL);
	m_pContext->Dispatch(1 * 3 * 3 * 3, 1 * 3 * 3 * 3, 1);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);

	// copy  Src -> 2
	m_pContext->CSSetShaderResources(0, 1, &m_pBlurSRV[0]);
	m_pContext->CSSetShader(m_pCScpy2, NULL, 0);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pBlurUAV[1], NULL);
	m_pContext->Dispatch(1 * 3 * 3 * 3, 1 * 3 * 3 * 3, 1);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);

	// copy  Src -> 3
	m_pContext->CSSetShaderResources(0, 1, &m_pBlurSRV[1]);
	//m_pContext->CSSetShader(m_pCScpy2, NULL, 0);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pBlurUAV[2], NULL);
	m_pContext->Dispatch(1 * 3 * 3 * 3, 1 * 3 * 3 * 3, 1);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);

	
	//// X�����u���[����
	//pEffect->SetTexture("t0", pShaderM->pHDRBlurTex );
	//pd3ddev->SetRenderTarget( 0, pBlurS1 );
	////pd3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(1, 0, 50, 0), 1.0f, 0);
	//pEffect->BeginPass(1);
	//p2Dsq->Render();
	//pEffect->EndPass();

	m_pContext->CSSetShader(m_pCSx, NULL, 0);

	// Xaxis Blur 1
	m_pContext->CSSetShaderResources(0, 1, &m_pBlurSRV[0]);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pBlurWorkUAV, NULL);
	m_pContext->Dispatch(sizeX, sizeY, 1);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);
	
	// Yaxis Blur 1
	m_pContext->CSSetShaderResources(0, 1, &m_pBlurWorkSRV);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pBlurUAV[0], NULL);
	m_pContext->Dispatch(sizeY, sizeX, 1);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);

	//m_pContext->CopyResource(m_pBlurTex[1], m_pBlurTex[0]);
	sizeX /= 3;
	sizeY /= 3;

	// Xaxis Blur 2
	m_pContext->CSSetShaderResources(0, 1, &m_pBlurSRV[1]);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pBlurWorkUAV, NULL);
	m_pContext->Dispatch(sizeX, sizeY, 1);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);

	// Yaxis Blur 2
	m_pContext->CSSetShaderResources(0, 1, &m_pBlurWorkSRV);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pBlurUAV[1], NULL);
	m_pContext->Dispatch(sizeY, sizeX, 1);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);

	//m_pContext->CopyResource(m_pBlurTex[2], m_pBlurTex[1]);
	sizeX /= 3;
	sizeY /= 3;

	// Xaxis Blur 3
	m_pContext->CSSetShaderResources(0, 1, &m_pBlurSRV[2]);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pBlurWorkUAV, NULL);
	m_pContext->Dispatch(sizeX, sizeY, 1);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);

	// Yaxis Blur 3
	m_pContext->CSSetShaderResources(0, 1, &m_pBlurWorkSRV);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &m_pBlurUAV[2], NULL);
	m_pContext->Dispatch(sizeY, sizeX, 1);
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pNull, (UINT*)&pNull);
	
	//SetMatrixBlurY();

	//// Y�����u���[����
	//pEffect->SetTexture("t0", pBlurT1 );
	//pd3ddev->SetRenderTarget( 0, pBlurS2 );
	////pd3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(1, 0, 50, 0), 1.0f, 0);
	//pEffect->BeginPass(2);
	//p2Dsq->Render();
	//pEffect->EndPass();

	//p2Dsq->Resize(Width*4,Height*4);

	////�u���[�ƌ��摜�����Z����
	//pEffect->SetTexture("t0", pShaderM->pFixedSizeTex );
	//pEffect->SetTexture("t1", pBlurT2 );
	//pd3ddev->SetRenderTarget(0, pShaderM->pHDRBlurSurface );
	//pd3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(1, 0, 0, 0), 1.0f, 0);
	//pEffect->BeginPass(3);
	//p2Dsq->Render();
	//pEffect->EndPass();
	
	m_pContext->PSSetShaderResources(0, 1, &m_pShaderBox->pum_pSRVF32[0]);
	m_pContext->PSSetShaderResources(1, 3, m_pBlurSRV);
	m_pShaderBox->ChangeRenderTarget(0, m_pShaderBox->pum_pRTVF32[3]);
	m_pShaderBox->SetRTsToShader();
	m_p2Dsq->Render();

	FOR(BLUR_TEX_NUM)m_pDebugTex->Set(m_pBlurSRV[i]);
	//pEffect->End();
}


void BlurBloom::up()
{
	if( m_Dispersion < 10.0f )
		m_Dispersion += 0.01f;
	SetConstStruct();
}

void BlurBloom::down()
{
	if( m_Dispersion > 0.02f )
		m_Dispersion -= 0.01f;
	SetConstStruct();
}