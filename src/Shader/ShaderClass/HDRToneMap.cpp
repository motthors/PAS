#include"HDRToneMap.h"
#include"../../base/DefineRendering.h"

HDRToneMap::HDRToneMap()
{
	strcpy_s(m_FileName,100,"data/Effect/2DHDRToneMap.fx");
}

HDRToneMap::~HDRToneMap()
{
	RELEASE(pBlurS1);
	RELEASE(pBlurT1);
	RELEASE(pBlurS2);
	RELEASE(pBlurT2);
	SAFE_DELETE(p2Dsq);
}

void HDRToneMap::Init(ShaderBox* pSV, DebugTextureViewer* pDTV)
{
	ShaderBase::Init(NULL, pSV, pDTV);
	Dispersion = 0.05f;
	Width = (float)DefRender.RenderTargetX / 4;
	Height = (float)DefRender.RenderTargetY / 4;

	//m_p2Dsq = new D3D2DSQUARE(pdx11, pSB, DefRender.RenderTargetX, DefRender.RenderTargetY);
	//p2Dsq = new D3D2DSQUARE(pdev, Width * 4, Height * 4);
	p2Dsq->Init();

	CreateSurface();
}

HRESULT HDRToneMap::CreateSurface()
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

	return S_OK;
}

void HDRToneMap::SetMatrixBlur(float Value)
{
	int Division = 8;
	float total = 0.0f;
	float weight[8];
	//float *weight = new float [Division];
	
	// �K�E�X�֐��ɂ��d�݂̌v�Z
	FOR( Division )
	{
		float pos = (float)i * 2.0f;
		weight[i] = expf( - pos * pos * Dispersion );
		total += weight[i];
	}
	// �d�݂̋K�i��
	FOR(Division)weight[i] = weight[i] / total * 0.5f;

	FOR( Division )
	{
		char name[10];
		sprintf_s(name,10,"Weight%d",i);
		//pEffect->SetFloat(name,weight[i]);
		sprintf_s(name,10,"g_Texel%d",i);
		//pEffect->SetFloat(name, -(float)(i) / Value);
	}

	//pEffect->SetFloat("Length", Value );
	//pEffect->SetFloat("Offset", 8.0f / Value );
	//SAFE_DELETEARRAY(weight);
}

void HDRToneMap::SetMatrixBlurX()
{
	SetMatrixBlur(Width);
}

void HDRToneMap::SetMatrixBlurY()
{
	SetMatrixBlur(Height);
}

void HDRToneMap::Render()
{
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

	//// X�����u���[����
	//pEffect->SetTexture("t0", pShaderM->pHDRBlurTex );
	//pd3ddev->SetRenderTarget( 0, pBlurS1 );
	////pd3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(1, 0, 50, 0), 1.0f, 0);
	//pEffect->BeginPass(1);
	//p2Dsq->Render();
	//pEffect->EndPass();

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
	//
	//pEffect->End();
}


void HDRToneMap::up()
{
	if( Dispersion < 10.0f )
		Dispersion += 0.01f;
}

void HDRToneMap::down()
{
	if( Dispersion > 0.02f )
		Dispersion -= 0.01f;
}