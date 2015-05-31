#ifndef _EFFECT_HDRTONEMAP_H_
#define _EFFECT_HDRTONEMAP_H_

#include"../../base/IncludeAll.h"
#include"../ShaderBase.h"
#include"../../base/2DSQUARE.h"

class HDRToneMap : public ShaderBase{

	D3D2DSQUARE *p2Dsq;
	//ブラーレンダリング用ちっちゃいサーフェス
	IDirect3DSurface9 *pBlurS1;
	IDirect3DTexture9 *pBlurT1;

	IDirect3DSurface9 *pBlurS2;
	IDirect3DTexture9 *pBlurT2;

	// ブラーをレンダするサーフェスのサイズ
	float Width;
	float Height;

	float Dispersion;

public:
	HDRToneMap();
	~HDRToneMap();
	
	void Init(ShaderBox* pSV, DebugTextureViewer* pDTV);
	HRESULT CreateSurface();
	void SetMatrixDiv();
	void SetMatrixBlurX();
	void SetMatrixBlurY();
	void SetMatrixBlur(float Value);
	void Render();

	void up();
	void down();
};

#endif