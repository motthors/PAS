#ifndef _EFFECT_BLURBLOOM_H_
#define _EFFECT_BLURBLOOM_H_

#include"../../base/IncludeAll.h"
#include"../ShaderBase.h"
#include"../../base/2DSQUARE.h"

class BlurBloom : public ShaderBase{

	D3D2DSQUARE *m_p2Dsq;

	// 3分の1を3回行う
	//ブラーレンダリング用ちっちゃいサーフェス
#define BLUR_TEX_NUM 3
	ID3D11Texture2D*			m_pBlurTex[3];
	ID3D11ShaderResourceView*	m_pBlurSRV[3];
	ID3D11UnorderedAccessView*	m_pBlurUAV[3];

	// 作業用テクスチャバッファ　ブラー用テクスチャの最大サイズの正方形
	ID3D11Texture2D*			m_pBlurWorkTex = nullptr;
	ID3D11ShaderResourceView*	m_pBlurWorkSRV = nullptr;
	ID3D11UnorderedAccessView*	m_pBlurWorkUAV = nullptr;

	// CS Shader
	ID3D11ComputeShader* m_pCScpy = nullptr;
	ID3D11ComputeShader* m_pCScpy2 = nullptr;
	ID3D11ComputeShader* m_pCSx = nullptr;
	ID3D11ComputeShader* m_pCSy = nullptr;


	// メイン画面のサイズ
	float m_Width;
	float m_Height;

	// ブレンド重み　定数バッファ
	float m_weight[8];
	ID3D11Buffer* m_pConstBuffer = nullptr;

	float m_Dispersion;

public:
	BlurBloom();
	~BlurBloom();
	
	void Init(DirectX11Base* pDx, ShaderBox* pSB, DebugTextureViewer* pDTV);
	HRESULT CreateSurface();
	void SetConstStruct();
	void Render();

	void up();
	void down();
};

#endif