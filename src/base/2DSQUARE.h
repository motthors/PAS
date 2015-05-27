#ifndef _2DSQ_H_
#define _2DSQ_H_
////////////////////////////////////////////////////////////////
//ポストエフェクト用のウィンドウと同じ大きさのポリゴン描画クラス
////////////////////////////////////////////////////////////////

#include"IncludeAll.h"
#include"../dx11/DirectX11Base.h"
#include"../Shader/ShaderBox.h"

class D3D2DSQUARE
{
private:
	//頂点フォーマット
	DirectX11Base*	m_pDx11;
	ShaderBox*		m_pShaderBox;

	//頂点宣言
	typedef struct{
		float x, y, z;		// 座標
		float u, v;			// UV座標
	} VERTEX;
	// バッファ
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	// シェーダー
	UINT	m_idVertexShader;
	UINT	m_idPixelShader;
	// レイアウト

	DXDEVICE*		m_pDevice;
	DXCONTEXT*		m_pContext;

	//２Ｄポリゴンのサイズ
	UINT Width, Height;

public:
	D3D2DSQUARE();
	D3D2DSQUARE(DirectX11Base* pdx11, ShaderBox* psb, UINT Width, UINT Height);
	~D3D2DSQUARE();
	//初期化
	void Init();
	void Invalidate();
	//2Dポリゴンのサイズを取得
	UINT GetWidth();
	UINT GetHeight();
   //シェーダを登録
	void SetVertexShader(UINT idpv);
	void SetPixelShader(UINT idps);
	//2Dポリゴンをレンダリング
	void Render();
};


#endif