#include"ShaderBase.h"

ShaderBase::ShaderBase()
{
	//pEffect=NULL;
}

ShaderBase::~ShaderBase()
{
	//RELEASE(pEffect);
}


void ShaderBase::Init(DirectX11Base* pDx, ShaderBox* pSV, DebugTextureViewer* pDTV)
{
	m_pDx11 = pDx;
	m_pShaderBox = pSV;
	m_pDebugTex = pDTV;
	m_pDevice = pDx->GetDevice();
	m_pContext = pDx->GetContext();
	////LPD3DXBUFFER ErrorText;

	///*HRESULT hr = D3DXCreateEffectFromFileA(
	//			pdev,
	//			FileName,
	//			NULL,
	//			NULL,
	//			D3DXSHADER_DEBUG,
	//			NULL,
	//			&pEffect,
	//			&ErrorText
	//);*/
	//if(FAILED(hr))
	//{
	//	char* error = new char[ErrorText->GetBufferSize()+70];
	//	sprintf(error,"FileName:%s\n%s", FileName, ErrorText->GetBufferPointer());
	//	MessageBoxA(NULL,error,"シェーダ読み込みorコンパイルエラー",MB_OK);
	//	delete [] error;
	//	return -6;
	//}
}

void  ShaderBase::SetMatrix()
{
	////pEffect->SetMatrix("matWorld",	pShaderM->pmatWorld);

	//// W*V*P
	//memcpy_s(&pShaderM->matWVP,sizeof(D3DXMATRIX),pShaderM->pmatWorld,sizeof(D3DXMATRIX));
	//D3DXMatrixMultiply(&pShaderM->matWVP,&pShaderM->matWVP,&pShaderM->matView);
	//D3DXMatrixMultiply(&pShaderM->matWVP,&pShaderM->matWVP,&pShaderM->matProj);
	//pEffect->SetMatrix("matWVP",	&pShaderM->matWVP);

	//// WI(T)
	//D3DXMatrixInverse(&pShaderM->matWIT,NULL,pShaderM->pmatWorld);
	//M_d3DXMatrixOffset(&pShaderM->matWIT,&D3DXVECTOR3(0.f,0.f,0.f));
	//D3DXMatrixTranspose(&pShaderM->matWIT,&pShaderM->matWIT);
	//pEffect->SetMatrix("matWorldIT", &pShaderM->matWIT);

	//D3DXVECTOR4 light;
	////D3DXVec4Transform( &light, &pShaderM->LightDir, &pShaderM->matWIT);
	////D3DXVec3Normalize((D3DXVECTOR3*)&light, (D3DXVECTOR3*)&light);
	//pEffect->SetVector("LightDir", &pShaderM->LightDir);
	//
	//D3DXVECTOR4 pos = D3DXVECTOR4(pShaderM->CameraPos.x,pShaderM->CameraPos.y,pShaderM->CameraPos.z,0.f);
	////D3DXVec4Transform( &pos, &pos, &pShaderM->matWIT);	
	//pEffect->SetVector("vecCameraPos", &pos);
}

void ShaderBase::Render()
{
	//pEffect->SetTechnique( "Main" );
	//pEffect->Begin( NULL, 0 );
	//pEffect->BeginPass(0);

	//pShaderM->DrawTexture(pEffect); //DrawObjectより先に呼ぶ
	//pShaderM->DrawObject();

	//pEffect->EndPass();
	//pEffect->End();
}


void ShaderBase::Begin()
{
	//pEffect->SetTechnique("Main");
	//pEffect->Begin(NULL, 0);
	//pEffect->BeginPass(0);
}


void ShaderBase::End()
{
	//pEffect->EndPass();
	//pEffect->End();
}

