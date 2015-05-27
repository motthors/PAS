#ifndef _DIRECTX9_BASE_H_
#define _DIRECTX9_BASE_H_


#include"IncludeAll.h"

class DirectXBase{
public:
	DirectXBase(){}
	virtual ~DirectXBase(){}
	virtual HRESULT Init(HWND hWnd, bool flscflag){ return E_FAIL; }
	virtual DXDEVICE* GetDevice(){ return nullptr; }
	virtual DXCONTEXT* GetContext(){ return nullptr; }
	virtual void BeginDraw(){}
	virtual void EndDraw(){}
};


class DirectX9Base : public DirectXBase {

	LPDIRECT3D9			pD3D;
	LPDIRECT3DDEVICE9	pd3ddev;//デバイス
	//LPD3DXFONT			pd3dfont;	//デバッグ用フォントデバイス
	//ID3DXEffect*		pEffect;
	D3DLIGHT9			light;
public:
	DirectX9Base();
	virtual ~DirectX9Base();

	HRESULT Init(HWND hWnd, bool flscflag);

	LPDIRECT3DDEVICE9	GetpD3DDev(){ return pd3ddev; }
	//LPD3DXFONT			GetpD3DFont(){return pd3dfont;}

	void BeginDraw(){}
	void EndDraw(){}
};


#endif