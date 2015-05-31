#ifndef _2DDRAWMANAGER_H_
#define _2DDRAWMANAGER_H_

#include"../base/IncludeAll.h"
#include"../dx11/DirectX11Base.h"
#include"../base/DataBox.h"
#include"../Shader/ShaderBox.h"
#include"../base/2DSQUARE.h"
#include<vector>

typedef struct{
	//頂点シェーダ用変換係数
	vec4 Width_Shift;
	vec2 CenterPos;
	//Databox TextureID
	int texid;
	float alpha;
} m2DDWStruct;


class My2DDrawManager {

	DirectX11Base*		m_pDx11;
	DXCONTEXT*			m_pContext;
	DataBox*			m_pDataBox;
	ShaderBox*			m_pShaderBox;
	D3D2DSQUARE*		m_p2Dsq;

	//頂点宣言
	typedef struct{
		float x, y, z;		// 座標
		float u, v;			// UV座標
	} VERTEX;
	// 定数バッファ
	ID3D11Buffer*	m_pConstantBafferVS;
	ID3D11Buffer*	m_pConstantBafferPS;

	// シェーダー
	UINT	m_idVertexShader;
	UINT	m_idPixelShader[3];

	// 描画するビルボード配列
	std::vector<m2DDWStruct*> list2DBoardArr;

	float per1TexelX, per1TexelY;//1ピクセルのテクセル幅


	//hlslシェーダ定数バッファ定義
	typedef struct{
		vec4 Width_Shift;
		float FakeDepth;
		vec3 __Blank;
	} m2DDWConstVS;

	typedef struct{
		float alpha;
		float XperY;
		vec2 __Blank;
		vec4 Width_Shift;
	} m2DDWConstPS;

	m2DDWConstVS m_ConstVS;
	m2DDWConstPS m_ConstPS;

public:

	My2DDrawManager();
	virtual ~My2DDrawManager();

	void Init(DirectX11Base* pdx11, DataBox* pdb, int WindowSizeW, int WindowSizeH);
	void SetShaderBox(ShaderBox* p){ m_pShaderBox = p; }
	void SetWindowSize();

	//void Draw(float lux, float luy, float rdx, float rdy, int texid);
	void DrawSquare_TexelStd(float centerX, float centerY, float width, float alpha, int texid);
	void Draw(float centerX, float centerY, float width, float height, float alpha, int texid);
	void DrawNumber(float centerX, float centerY, float height, float alpha, float Number, int MaxDigit, int Decimal);

	void Render();		//マネージャ内のすべてのボードの描画
};


#endif