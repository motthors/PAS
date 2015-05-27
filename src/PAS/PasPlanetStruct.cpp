#include"PASPlanetStruct.h"


PASPlanetStruct::PASPlanetStruct()
{
	ZeroMemory(&m_PlanetPos, sizeof(vec));
	ZeroMemory(&m_posidx, sizeof(POSINDEX));

	m_PAS_EPCS_PS.Rg = 6360.0f;
	m_PAS_EPCS_PS.Rt = 6420.0f;
	m_PAS_EPCS_PS.HR = 6.0f;
	m_PAS_EPCS_PS.betaR = vec3(5.8e-3f, 1.35e-2f, 3.31e-2f);
	m_PAS_EPCS_PS.HM = 1.2f;
	m_PAS_EPCS_PS.betaMSca = vec3(4e-3f, 4e-3f, 4e-3f);
	m_PAS_EPCS_PS.betaMEx = vec3(4e-3f / 0.9f, 4e-3f / 0.9f, 4e-3f / 0.9f);
	m_PAS_EPCS_PS.mieG = 0.8f;

	m_PAS_EPCS_PS.vecColor1 = vec3(1.f, 0.f, 0.f);
	m_PAS_EPCS_PS.vecColor2 = vec3(0.f, 1.f, 0.f);
	m_PAS_EPCS_PS.vecColor3 = vec3(0.f, 0.f, 1.f);
}


PASPlanetStruct::~PASPlanetStruct()
{
	// Texture
	RELEASE(m_TransmittanceTexture);
	RELEASE(m_IrradianceTexture);
	RELEASE(m_InscatterTexture);
	// SR View
	RELEASE(m_TransmittanceView);
	RELEASE(m_IrradianceView);
	RELEASE(m_InscatterView);
	// Cont Baffer
	RELEASE(m_pConstantBafferVS);
	RELEASE(m_pConstantBafferPS);
}


void PASPlanetStruct::Init(DirectX11Base* pdx11, DataBox* pDB, MatrixBoxManager* pMB, ShaderBox* pSB)
{
	m_pDx11 = pdx11;
	m_pDataBox = pDB;
	//pMatBoxM = pMB;
	m_pShaderBox = pSB;
	m_pDevice = m_pDx11->GetDevice();
	m_pContext = m_pDx11->GetContext();
	//定数バッファ作成
	m_pDx11->CreateConstantBuffer(&m_pConstantBafferVS, NULL, sizeof(m_PAS_EPCS_VS), D3D11_CPU_ACCESS_WRITE);
	m_pDx11->CreateConstantBuffer(&m_pConstantBafferPS, NULL, sizeof(m_PAS_EPCS_PS), D3D11_CPU_ACCESS_WRITE);
}


void PASPlanetStruct::CreateTexture(void* traBuff, void* irrBuff, void* insBuff)
{
	texidRefrectanceTexture = m_pDataBox->GetTexture(_T("data/earth.dds"));

	m_pShaderBox->CreateTexture2D(&m_TransmittanceTexture, traBuff, sizeof(FLOAT128), TRANSMITTANCE_W, TRANSMITTANCE_H, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_TransmittanceTexture, &m_TransmittanceView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);

	m_pShaderBox->CreateTexture2D(&m_IrradianceTexture, irrBuff, sizeof(FLOAT128), SKY_W, SKY_H, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_IrradianceTexture, &m_IrradianceView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D);

	m_pShaderBox->CreateTexture3D(&m_InscatterTexture, insBuff, sizeof(FLOAT128), RES_MU_S * RES_NU, RES_MU, RES_R, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE);
	m_pShaderBox->CreateSRV(m_InscatterTexture, &m_InscatterView, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE3D);
}


void PASPlanetStruct::SetPosition(vec3* pos, POSINDEX* idx)
{
	memcpy_s(&m_PlanetPos, sizeof(vec3), pos, sizeof(vec3));
	memcpy_s(&m_posidx, sizeof(POSINDEX), idx, sizeof(POSINDEX));
}


void PASPlanetStruct::SetConstStruct(vec3* pvecCampos, POSINDEX* pIndex)
{
	// pompute campos
	vec cam = XMLoadFloat3(pvecCampos);
	vec pla = XMLoadFloat3(&m_PlanetPos);
	cam = cam - pla;
	XMStoreFloat3(&m_PAS_EPCS_VS.vecPlanetToCam, cam);
	XMStoreFloat3(&m_PAS_EPCS_PS.vecPlanetToCam, cam);
	m_PAS_EPCS_VS.__Blank = m_PlanetPos.x > 100 ? 0.9 : 0.5;
	// Change Const Struct
	m_pDx11->SetConstBuffer(m_pConstantBafferVS, &m_PAS_EPCS_VS, sizeof(m_PAS_EPCS_VS));
	m_pDx11->SetConstBuffer(m_pConstantBafferPS, &m_PAS_EPCS_PS, sizeof(m_PAS_EPCS_PS));
	// Set Const Struct to Shader
	m_pContext->VSSetConstantBuffers(1, 1, &m_pConstantBafferVS);
	m_pContext->PSSetConstantBuffers(1, 1, &m_pConstantBafferPS);
}