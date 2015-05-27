#ifndef _PAS_RENDER_H_
#define _PAS_RENDER_H_


#include"../base/IncludeAll.h"
#include"../base/DataBox.h"
#include"../base/MatrixBox.h"
#include"../base/CameraController.h"
#include"../dx11/DirectX11Base.h"
#include"../Shader/ShaderBox.h"
#include"../base/2DSQUARE.h"

#include"PASPlanetStruct.h"

#include"ScreenGrab.h"
#include"DDSTextureLoader.h"

#define TRANS_W TRANSMITTANCE_W
#define TRANS_H TRANSMITTANCE_H

class PASRender{
protected:
	// 描画用基本クラスポインタ
	DirectX11Base*		m_pDx11;
	DXDEVICE*			m_pDevice;
	DXCONTEXT*			m_pContext;

	D3D2DSQUARE*		m_p2Dsq;

	ShaderBox*			m_pShaderBox;
	CameraController*	m_pCamera;
	DataBox*			m_pDatabox;

	// 惑星描画パラメータクラス　惑星１つにつきクラス１つ
	static const int	MaxPlanetNum = 3;
	PASPlanetStruct*	m_pPlanet[MaxPlanetNum];
	int					m_PlanetNum = 0;
	PASPlanetStruct*	m_NowPlanet = nullptr;

	//// 惑星共通定数バッファパラメータ
	typedef struct{
		mat4 matVI;
		mat4 matPI;
		//vec4 vecCamPos;
	} PAS_ConstStruct_VS;

	typedef struct{
		//vec4 vecCamPos;
		vec3 SunDir;
		float __Blank;
	} PAS_ConstStruct_PS;

	PAS_ConstStruct_VS m_PAS_ConstStruct_VS;
	PAS_ConstStruct_PS m_PAS_ConstStruct_PS;

	//vec			PlanetPos;
	//POSINDEX	posidx;

	//UINT	texidRefrectanceTexture;
	//int		meshidPlanet;
	// シェーダ
	UINT	idPS_PAS;
	UINT	idPS_PrePAS;
	UINT	idPS_PAS_Ground;
	UINT	idPS_PAS_Atmosphere;
	UINT	idPS_PAS_Sun;

	///////////////////////////////////////


	//---------------------------------------------------------------
	// テクスチャ シェーダ関係変数
	//---------------------------------------------------------------
	
	//UINT	texidRefrectanceTexture = -1;
	// プリレンダリング用テクスチャ/////////////
	//ID3D11Texture2D*	m_TransmittanceTexture = nullptr;
	//ID3D11Texture2D*	m_IrradianceTexture = nullptr;
	//ID3D11Texture3D*	m_InscatterTexture = nullptr;

	//// シェーダーリソースビュー///////////////////
	//ID3D11ShaderResourceView* m_TransmittanceView = nullptr;
	//ID3D11ShaderResourceView* m_IrradianceView = nullptr;
	//ID3D11ShaderResourceView* m_InscatterView = nullptr;

	//親からもらうデプス

	// 定数バッファ///////////////////////////////
	ID3D11Buffer*	m_pConstantBafferVS = nullptr;
	ID3D11Buffer*	m_pConstantBafferPS = nullptr;

	// PAS(t,r,mu)バッファ2つ///////////////////////////////
	#define 					m_RVnum 2
	ID3D11Texture2D*			m_pRenderVarTexture[m_RVnum];
	ID3D11ShaderResourceView*	m_pRenderVarSRView[m_RVnum];
	ID3D11RenderTargetView*		m_pRenderVarRTView[m_RVnum];

	// テクスチャサンプラー　ShaderManagerからもらうからRelease不要
	ID3D11SamplerState* m_pDefaultSampler = nullptr;

	// 親からもらうデフォルトブレンドステート
	ID3D11BlendState* m_pDefaultBlendState = nullptr;
	// 描画用パラメータ保存テクスチャ用ブレンドステート
	ID3D11BlendState* m_pRenderPalamBlendState = nullptr;
	// 各光描画用加算合成ブレンドステート
	ID3D11BlendState* m_pAddBlendState = nullptr;

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

	void LoadPASTextureFromFile(ID3D11Resource* pTex, wchar_t* fname, int DataNnum);

	void CheckLoadTextreFile(void* pData, char* FileName, UINT dataNum, UINT LayerNum);
	virtual void CreateTexture();

public:

	//int TextureTestViewFlag = 5; // temp

	PASRender();
	virtual ~PASRender();

	//int Init();
	void Init(DirectX11Base* pdx11, DataBox* pDB, MatrixBoxManager* pMB, ShaderBox* pSB);
	void SetTexture(TCHAR* transmittanceTex, TCHAR* irradianceTex, TCHAR* inscatterTex);
	void SetSampler(ID3D11SamplerState* p){ m_pDefaultSampler = p; }
	void CreateBlendState(ID3D11BlendState* p);

	//void SetPlanetPos();
	void AddPlanet();

	virtual void Move();
	virtual void Draw();

	void Render(ID3D11RenderTargetView* pOutRTV);
	void Render2(ID3D11RenderTargetView* pOutRTV); //調整中

#ifdef _DEBUG
	void DebugDraw();
#endif

};

#endif