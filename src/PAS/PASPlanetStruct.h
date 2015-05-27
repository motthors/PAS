#ifndef _PAS_PLANET_STRUCT_H_
#define _PAS_PLANET_STRUCT_H_

#include"../base/IncludeAll.h"
#include"../Shader/ShaderBox.h"
#include"../base/MatrixBox.h"
#include"../base/DataBox.h"

class PASPlanetStruct{
	// 描画用基本クラス
	DirectX11Base*	m_pDx11;
	DXDEVICE*		m_pDevice;
	DXCONTEXT*		m_pContext;

	ShaderBox*		m_pShaderBox;
	DataBox*		m_pDataBox;

	// texture //////////////////////
	// 各テクスチャサイズ
	// transmittance T 大気の透過度
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
	// 事前計算テクスチャ
	ID3D11Texture2D*	m_TransmittanceTexture = nullptr;
	ID3D11Texture2D*	m_IrradianceTexture = nullptr;
	ID3D11Texture3D*	m_InscatterTexture = nullptr;

	// シェーダーリソースビュー
	ID3D11ShaderResourceView* m_TransmittanceView = nullptr;
	ID3D11ShaderResourceView* m_IrradianceView = nullptr;
	ID3D11ShaderResourceView* m_InscatterView = nullptr;


public:
	// 描画用パラメータ
	typedef struct{
		vec3 vecPlanetToCam;
		float __Blank;
	} EachPlanetConstStruct_VS;
	typedef struct{
		vec3 vecPlanetToCam;
		float _Blank0;
		// 散乱カラー
		vec3 vecColor1;
		float _Blank1;
		vec3 vecColor2;
		float _Blank2;
		vec3 vecColor3;
		float _Blank3;

		// レイリー散乱係数
		float HR;
		vec3 betaR;

		// ミー散乱係数
		float HM;
		vec3 betaMSca;
		float mieG;
		vec3 betaMEx;

		// 半径データ　
		float Rg; // 地表までの半径
		float Rt; // 大気の層の半径

		vec2 __Blank;
	} EachPlanetConstStruct_PS;

	EachPlanetConstStruct_VS m_PAS_EPCS_VS;
	EachPlanetConstStruct_PS m_PAS_EPCS_PS;
	// 定数バッファ
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