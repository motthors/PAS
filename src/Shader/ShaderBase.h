#ifndef _SHADER_BASE_H_
#define _SHADER_BASE_H_

#include"../base/IncludeAll.h"
#include"ShaderBox.h"

class ShaderBase{
protected:
	DirectX11Base*		m_pDx11;
	ShaderBox*			m_pShaderBox;
	DXDEVICE*			m_pDevice;
	DXCONTEXT*			m_pContext;
	char				m_FileName[100];
public:
	ShaderBase();
	virtual ~ShaderBase();

	virtual void Init(DirectX11Base* pDx, ShaderBox* pSV, DXDEVICE* pdev, DXCONTEXT* pcnt);
	virtual void SetMatrix();
	virtual void Render(ID3D11RenderTargetView* pOutRTV, ID3D11ShaderResourceView* pInSRV);
				 
	virtual void Begin();
	virtual void End();
};

#endif