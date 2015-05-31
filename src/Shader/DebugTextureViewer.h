#ifndef _DEBUG_TEXTURE_VIEWER_H_
#define _DEBUG_TEXTURE_VIEWER_H_


#include"../base/IncludeAll.h"
#include"../base/2DSQUARE.h"


class DebugTextureViewer{

	D3D2DSQUARE*				m_p2Dsq;
	DirectX11Base*				m_pDx11;
	DXCONTEXT*					m_pContext;
	ShaderBox*					m_pShaderBox;

	int							m_SetSRVNum;
	static const int			MAX_SRV_NUM = 10;
	ID3D11ShaderResourceView*	m_arrSRVs[MAX_SRV_NUM];

	bool						m_RenderFlag = false;

	//hlslシェーダ定数バッファ定義
	typedef struct{
		vec4 Width_Shift;
		float FakeDepth;
		vec3 __Blank;
	} m2DDWConstVS;
	m2DDWConstVS	m_ConstVS;
	ID3D11Buffer*	m_pConstantBafferVS;

	float			m_alpha=1.f;
	ID3D11Buffer*	m_pConstantBafferPS;

public:
	DebugTextureViewer();
	virtual ~DebugTextureViewer();

	void Init(DirectX11Base* pdx11, ShaderBox* m_pShaderBox);
	void Set(ID3D11ShaderResourceView* p);
	void TurnOnOff();
	void Render();
};

#endif