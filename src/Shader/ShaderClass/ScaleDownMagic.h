#ifndef _SCALEDOWNMAGIC_H_
#define _SCALEDOWNMAGIC_H_

#include"../../base/IncludeAll.h"
#include"../ShaderBase.h"
#include"../../base/2DSQUARE.h"

class ScaleDownMagic : public ShaderBase{

	D3D2DSQUARE *m_p2Dsq;

public:
	ScaleDownMagic();
	~ScaleDownMagic();

	void Init(DirectX11Base* pDx, ShaderBox* pSB, DebugTextureViewer* pDTV);
	void SetMatrix(){}
	void Render(ID3D11RenderTargetView* pOutRTV, ID3D11ShaderResourceView* pInSRV);
};

#endif