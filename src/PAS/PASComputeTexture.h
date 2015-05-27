#ifndef _PAS_COMPUTETEXTURE_H_
#define _PAS_COMPUTETEXTURE_H_

//#include"../base/IncludeAll.h"
#include"LogManager.h"
//#include"../base/DataBox.h"
//#include"../base/MatrixBox.h"
//#include"../base/CameraController.h"
//#include"../dx11/DirectX11Base.h"
//#include"../Shader/ShaderBox.h"
//#include"../base/2DSQUARE.h"
#include"PASRender.h"

//#define TRANS_W TRANSMITTANCE_W
//#define TRANS_H TRANSMITTANCE_H

class PASComputeTexture : public PASRender{

	//// 描画用基本クラスポインタ
	//DirectX11Base*		m_pDx11;
	//DXDEVICE*			m_pDevice;
	//DXCONTEXT*			m_pContext;

	//D3D2DSQUARE*		m_p2Dsq;

	//ShaderBox*			m_pShaderBox;
	//CameraController*	m_pCamera;
	//DataBox*			m_pDatabox;

	//// 描画用パラメータ
	//typedef struct{
	//	mat4 matVI;
	//	mat4 matPI;
	//	vec4 vecCamPos;
	//} PAS_ConstStruct_VS;
	//typedef struct{
	//	vec4 vecCamPos;

	//	// 半径データ　
	//	float Rg; // 地表までの半径
	//	float Rt; // 大気の層の半径
	//	float RL;

	//	// レイリー散乱係数
	//	float HR;
	//	vec3 betaR;

	//	// ミー散乱係数
	//	float HM;
	//	vec3 betaMSca;
	//	float mieG;
	//	vec3 betaMEx;

	//	float coord;
	//	vec3 SunDir;
	//	float __Blank;
	//} PAS_ConstStruct_PS;

	typedef struct{
		float k;
		vec3 __Blank;
	} PAS_ConstStruct_CS2;

	//PAS_ConstStruct_VS m_PAS_ConstStruct_VS;
	//PAS_ConstStruct_PS m_PAS_ConstStruct_PS;
	PAS_ConstStruct_CS2 m_PAS_ConstStruct_CS2;

	//vec			PlanetPos;
	//POSINDEX	posidx;

	//UINT	texidRefrectanceTexture;
	//int		meshidPlanet;
	//// シェーダ
	//UINT	idPS_PAS;
	//UINT	idPS_PrePAS;
	//UINT	idPS_DefPAS;

	///////////////////////////////////////
	// 外部が見に来るfloat* の構造体
	float*	m_p_Rg;
	float*	m_p_Rt; 
	float*	m_p_HR; 
	float*	m_p_betaR[3];
	float*	m_p_HM; 
	float*	m_p_betaMSca[3];
	float*	m_p_mieG; 
	float*	m_p_betaMEx[3];

	float*	m_p_Color1[3];
	float*	m_p_Color2[3];
	float*	m_p_Color3[3];

	///////////////////////////////////////

	// シミュレータ用クラス
	LogManager*			pLogM;
	
	// シミュレータ用パラメータ

	// --------------------------
	// プリレンダ管理変数
	int PrerenderSwitchFlag;
	int ScatterCounter;
	bool prerenderflag;
	enum enumPrerenderFLAG{
		PF_TRNAS1,
		PF_IRRAD1,
		PF_INSCAT1,
		PF_COPY_IRRAD,
		PF_COPY_INSCAT,
		PF_L_DELTAJ,
		PF_L_IRRADN,
		PF_L_INSCATN,
		PF_L_COPY_Irr,
		PF_L_COPY_Ins,
		PF_TEXTURE_DRAW
	};
	//---------------------------------------------------------------
	// 物理量定数
	//---------------------------------------------------------------
	// 大気の散乱回数
	int ScatterNum;

	//---------------------------------------------------------------
	// テクスチャ シェーダ関係変数
	//---------------------------------------------------------------
	// Compute Shader
	ID3D11ComputeShader* m_pComputeShader[10];

	// プリレンダリング用テクスチャ/////////////
	ID3D11Texture2D*	m_RWTransmittanceTexture	= nullptr;
	ID3D11Texture2D*	m_RWIrradianceTexture		= nullptr;
	ID3D11Texture3D*	m_RWInscatterTexture		= nullptr;
	// 一時テクスチャ
	ID3D11Texture2D*	m_deltaETexture			= nullptr;
	ID3D11Texture3D*	m_deltaSRTexture		= nullptr;
	ID3D11Texture3D*	m_deltaSMTexture		= nullptr;
	ID3D11Texture3D*	m_deltaJTexture			= nullptr;
	// 加算合成用一時テクスチャ
	ID3D11Texture2D*	m_IrradianceAddTemp		= nullptr;
	ID3D11Texture3D*	m_InscatterAddTemp		= nullptr;
	// シェーダーリソースビュー///////////////////
	ID3D11ShaderResourceView* m_RWTransmittanceView	= nullptr;
	ID3D11ShaderResourceView* m_RWIrradianceView	= nullptr;
	ID3D11ShaderResourceView* m_RWInscatterView		= nullptr;

	ID3D11ShaderResourceView* m_deltaEView		= nullptr;
	ID3D11ShaderResourceView* m_deltaSRView		= nullptr;
	ID3D11ShaderResourceView* m_deltaSMView		= nullptr;
	ID3D11ShaderResourceView* m_deltaJView		= nullptr;

	ID3D11ShaderResourceView* m_IrradViewTemp	= nullptr;
	ID3D11ShaderResourceView* m_InscatViewTemp	= nullptr;
	// CS用UAV////////////////////////////////////
	ID3D11UnorderedAccessView* m_TransUAV		= nullptr;
	ID3D11UnorderedAccessView* m_IrradUAV		= nullptr;
	ID3D11UnorderedAccessView* m_InscatUAV		= nullptr;

	ID3D11UnorderedAccessView* m_deltaEUAV		= nullptr;
	ID3D11UnorderedAccessView* m_deltaSRUAV		= nullptr;
	ID3D11UnorderedAccessView* m_deltaSMUAV		= nullptr;
	ID3D11UnorderedAccessView* m_deltaJUAV		= nullptr;

	ID3D11UnorderedAccessView* m_IrradUAVTemp	= nullptr;
	ID3D11UnorderedAccessView* m_InscatUAVTemp	= nullptr;
	// テクスチャ保存用CPUアクセス可バッファ//////
	ID3D11Texture2D*	m_TransSaveTex = nullptr;
	ID3D11Texture2D*	m_IrradSaveTex = nullptr;
	ID3D11Texture3D*	m_InscaSaveTex = nullptr;
	// 定数バッファ///////////////////////////////
	//ID3D11Buffer*	m_pConstantBafferVS		= nullptr;
	//ID3D11Buffer*	m_pConstantBafferPS		= nullptr;
	ID3D11Buffer*	m_pConstantBafferCS2	= nullptr;

	// PAS(t,r,mu)バッファ///////////////////////////////
	//ID3D11Texture2D*			m_pZbuffer_trmu		= nullptr;
	//ID3D11ShaderResourceView*	m_pZbufferSRView	= nullptr;
	//ID3D11RenderTargetView*		m_pZbufferRTView	= nullptr;

	// テクスチャサンプラー　ShaderManagerからもらうからRelease不要
	//ID3D11SamplerState* m_pSampler;

	//// 各テクスチャサイズ
	//// transmittance T 大気の透過度
	//static const int TRANSMITTANCE_W = 256;
	//static const int TRANSMITTANCE_H = 64;
	//// irradiance E 
	//static const int SKY_W = 64;
	//static const int SKY_H = 16;
	//// scatter S
	//static const int RES_R = 32;
	//static const int RES_MU = 128;
	//static const int RES_MU_S = 32;
	//static const int RES_NU = 8;

	//void LoadPASTextureFromFile(ID3D11Resource* pTex, wchar_t* fname, int DataNnum);

	//---------------------------------------------------------------

	void CreateTexture();

	void prerender();
	void prerender_FlagManage();
	void prerender_trans1();
	void prerender_irrad1();
	void prerender_insct1();
	void prerender_CpyIr1();
	void prerender_CpyIs1();
	void prerender_deltaJ();
	void prerender_IrradN();
	void prerender_insctN();
	void prerender_CpyIrN();
	void prerender_CpyIsN();

public:

	//int TextureTestViewFlag=5; // temp

	PASComputeTexture();
	~PASComputeTexture();

	//int Init();
	void Init(
		DirectX11Base* pdx11,
		DataBox* pDB,
		MatrixBoxManager* pMB,
		LogManager*	pLM,
		ShaderBox* pSB
		);
	//void SetSampler(ID3D11SamplerState* p){ m_pSampler = p; }
	
	void SetPlanet();

	void ResetVariable();
	void StartPrerender();
	void AddMultiScatter();

	void SimpleSetting_ClearSky();
	void SimpleSetting_Default();
	void SimpleSetting_PartryCloudy();

	void SaveTexture();
	void SaveTexture_Subfunction(ID3D11Resource* pTex, char* FileName, UINT TexSize, UINT DivWrite = 1);
	//void SetPlanetPos();

	void Move();
	void Draw();

	///////////////////////////////////
	// 値・ポインタ取得関数  仕様非推奨
	float	GetRg(){ return m_NowPlanet->m_PAS_EPCS_PS.Rg; }
	float*	GetpRg(){ return &m_NowPlanet->m_PAS_EPCS_PS.Rg; }
	float*	GetpRt(){ return &m_NowPlanet->m_PAS_EPCS_PS.Rt; }
	float*	GetpHR(){ return &m_NowPlanet->m_PAS_EPCS_PS.HR; }
	float*	GetpHM(){ return &m_NowPlanet->m_PAS_EPCS_PS.HM; }
	float*	GetpBetaR_red(){ return &m_NowPlanet->m_PAS_EPCS_PS.betaR.x; }
	float*	GetpBetaR_gle(){ return &m_NowPlanet->m_PAS_EPCS_PS.betaR.y; }
	float*	GetpBetaR_blu(){ return &m_NowPlanet->m_PAS_EPCS_PS.betaR.z; }
	vec3*	GetpBetaR_vec(){ return &m_NowPlanet->m_PAS_EPCS_PS.betaR; }
	float*	GetpBetaMSca_red(){ return &m_NowPlanet->m_PAS_EPCS_PS.betaMSca.x; }
	float*	GetpBetaMSca_gle(){ return &m_NowPlanet->m_PAS_EPCS_PS.betaMSca.y; }
	float*	GetpBetaMSca_blu(){ return &m_NowPlanet->m_PAS_EPCS_PS.betaMSca.z; }
	vec3*	GetpbetaMSca_vec(){ return &m_NowPlanet->m_PAS_EPCS_PS.betaMSca; }
	float*	GetpMieG(){ return &m_NowPlanet->m_PAS_EPCS_PS.mieG; }
	vec3*	GetpSunDir(){ return &m_PAS_ConstStruct_PS.SunDir; }
	//////////////////////////
	float**	GetppRg(){ return &m_p_Rg; }
	float**	GetppRt(){ return &m_p_Rt; }
	float**	GetppHR(){ return &m_p_HR; }
	float**	GetppBetaR(int i){ return &m_p_betaR[i]; }
	float**	GetppHM(){ return &m_p_HM; }
	float**	GetppBetaMSca(int i){ return &m_p_betaMSca[i]; }
	float**	GetppMieG(){ return &m_p_mieG; }
	float**	GetppBetaMEx(int i){ return &m_p_betaMEx[i]; }
	float**	GetppColor1(int i){ return &m_p_Color1[i]; }
	float**	GetppColor2(int i){ return &m_p_Color2[i]; }
	float**	GetppColor3(int i){ return &m_p_Color3[i]; }
	//////////////////////////////////////
#ifdef _DEBUG
	void DebugDraw();
#endif

};

#endif