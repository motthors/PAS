#ifndef _PAS_PLANET_STRUCT_H_
#define _PAS_PLANET_STRUCT_H_

#include"../base/IncludeAll.h"
#include"../Shader/ShaderBox.h"
#include"../base/MatrixBox.h"
#include"../base/DataBox.h"

class PASPlanetStruct{
	// �`��p��{�N���X
	DirectX11Base*	m_pDx11;
	DXDEVICE*		m_pDevice;
	DXCONTEXT*		m_pContext;

	ShaderBox*		m_pShaderBox;
	DataBox*		m_pDataBox;

	// texture //////////////////////
	// �e�e�N�X�`���T�C�Y
	// transmittance T ��C�̓��ߓx
	static const int TRANSMITTANCE_W = 256;
	static const int TRANSMITTANCE_H = 64;
	// irradiance E 
	static const int SKY_W = 64;
	static const int SKY_H = 16;
	// scatter S
	static const int RES_R = 32;
	static const int RES_MU = 128;
	static const int RES_MU_S = 32;
	static const int RES_NU = 8;

	UINT	texidRefrectanceTexture = -1;
	// ���O�v�Z�e�N�X�`��
	ID3D11Texture2D*	m_TransmittanceTexture = nullptr;
	ID3D11Texture2D*	m_IrradianceTexture = nullptr;
	ID3D11Texture3D*	m_InscatterTexture = nullptr;

	// �V�F�[�_�[���\�[�X�r���[
	ID3D11ShaderResourceView* m_TransmittanceView = nullptr;
	ID3D11ShaderResourceView* m_IrradianceView = nullptr;
	ID3D11ShaderResourceView* m_InscatterView = nullptr;


public:
	// �`��p�p�����[�^
	typedef struct{
		vec3 vecPlanetToCam;
		float __Blank;
	} EachPlanetConstStruct_VS;
	typedef struct{
		vec3 vecPlanetToCam;
		float _Blank0;
		// �U���J���[
		vec3 vecColor1;
		float _Blank1;
		vec3 vecColor2;
		float _Blank2;
		vec3 vecColor3;
		float _Blank3;

		// ���C���[�U���W��
		float HR;
		vec3 betaR;

		// �~�[�U���W��
		float HM;
		vec3 betaMSca;
		float mieG;
		vec3 betaMEx;

		// ���a�f�[�^�@
		float Rg; // �n�\�܂ł̔��a
		float Rt; // ��C�̑w�̔��a

		vec2 __Blank;
	} EachPlanetConstStruct_PS;

	EachPlanetConstStruct_VS m_PAS_EPCS_VS;
	EachPlanetConstStruct_PS m_PAS_EPCS_PS;
	// �萔�o�b�t�@
	ID3D11Buffer*	m_pConstantBafferVS = nullptr;
	ID3D11Buffer*	m_pConstantBafferPS = nullptr;


	// position //////////////////////
	vec3		m_PlanetPos;
	POSINDEX	m_posidx;

public:
	PASPlanetStruct();
	~PASPlanetStruct();

	void Init(DirectX11Base* pdx11, DataBox* pDB, MatrixBoxManager* pMB, ShaderBox* pSB);
	void CreateTexture(void* traBuff, void* irrBuff, void* insBuff);
	void SetPosition(vec3* pos, POSINDEX* idx);
	void SetConstStruct(vec3* pvecCampos, POSINDEX* pIndex);

	EachPlanetConstStruct_PS* GetConstStructPS(){ return &m_PAS_EPCS_PS; }

	int							GetTexID_ref(){ return texidRefrectanceTexture; }
	ID3D11Texture2D**			GetTex_tra(){ return &m_TransmittanceTexture; }
	ID3D11Texture2D**			GetTex_irr(){ return &m_IrradianceTexture; }
	ID3D11Texture3D**			GetTex_ins(){ return &m_InscatterTexture; }
	ID3D11ShaderResourceView**	GetRTV_tra(){ return &m_TransmittanceView; }
	ID3D11ShaderResourceView**	GetRTV_irr(){ return &m_IrradianceView; }
	ID3D11ShaderResourceView**	GetRTV_ins(){ return &m_InscatterView; }

};

#endif