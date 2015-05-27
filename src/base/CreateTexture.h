#ifndef _CREATE_TEXTURE_H_
#define _CREATE_TEXTURE_H_

#include"IncludeAll.h"

//今のところ、完全文字列からテクスチャを作る専用クラス
//DirectXデバイスとゲームの間のクッション

class CreateTexture
{

	DXDEVICE*	m_pDevice;
	DXCONTEXT*	m_pContext;
	HFONT hFont;			//テクスチャ用フォントデバイス
	UINT m_FontSize;

public:

	CreateTexture();
	~CreateTexture();

	void SetD3Ddev(DXDEVICE* dev, DXCONTEXT* pcont){ m_pDevice = dev; m_pContext = pcont; }
	void Init(int fontsize=200);

	void FromString(TCHAR* text, DXTEXTURE2D** pTex, ID3D11ShaderResourceView** pView, float *pRatio, int fontsize = 200);

};

#endif