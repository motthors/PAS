#ifndef _EFFECT_MANAGER_H_
#define _EFFECT_MANAGER_H_

#include"../base/IncludeAll.h"

#include"../dx11/DirectX11Base.h"
#include"../base/DataBox.h"
#include"../base/MatrixBox.h"
#include"../base/BillBoardBeamManager.h"
#include"../PAS/PASComputeTexture.h"
#include"../2DDrawer/2DDrawManager.h"
#include"../base/Input.h"

//#include"2DSQUARE.h"
#include<CommonStates.h>

#include"ShaderBase.h"
#include"ShaderBox.h"

#define EFFECT_NUM		2

#define EFFECT_DEF		0
#define EFFECT_AR		1
#define EFFECT_SPACE	2

class ShaderManager{

	DirectX11Base*	m_pDx11;
	DXDEVICE*		m_pDevice;
	DXCONTEXT*		m_pContext;
	DXSC*			m_pSwapChain;

	ShaderBox*		m_pShaderBox;
	
	Input*			pInput;

	MatrixBox*		PS[MATBOX_TOTALNUM];
	UINT			EffectIdx[MATBOX_TOTALNUM];

	DataBox*				pDataBox;
	MatrixBoxManager*		pMatBoxM;
	BillBoardBeamManager*	pBBBM;
	PASComputeTexture*		pPASCT;
	My2DDrawManager*		p2DDrawer;

	ShaderBase*		aEffect[EFFECT_NUM];

	// �|�X�g�G�t�F�N�g�V�F�[�_�[�N���X 
	ShaderBase*		pHDR;
	ShaderBase*		pBlurBloom;
	ShaderBase*		pScaleDown;

	MatrixBox*		pMB;

	D3D11_VIEWPORT	m_vp;

	// ���ʃX�e�[�g DXTK
	ID3D11RasterizerState*		m_pCommonRasterizerState;
	ID3D11BlendState*			m_pCommonTextureBlendState;
	ID3D11DepthStencilState*	m_pCommonDepthStencilState;
	ID3D11SamplerState*			m_pSamplerState[2];

	void CreateCommonState();
	void ResetState();

public:

	////�T�[�t�F�X
	//IDirect3DSurface9 *pDefaultBackBuffer;
	//IDirect3DSurface9 *pFixedSizeSurface;
	//IDirect3DSurface9 *pFixedSizeDepthStencil;
	//IDirect3DSurface9 *pHDRBlurSurface;
	// �Œ�T�C�YHDR�^�[�Q�b�g
	ID3D11Texture2D*			pFixedSizeTex = nullptr;
	ID3D11RenderTargetView*		pFixedSizeRTV = nullptr;
	ID3D11ShaderResourceView*	pFixedSizeSRV = nullptr;

	//ID3D11Texture2D *pHDRBlurTex;
	//ID3D11UnorderedAccessView* pHDRBlurUAV;
	//ID3D11ShaderResourceView*	pFixedSizeSRV;

	ID3D11RenderTargetView*	m_pRenderTargetView;	// �ŏI�`��^�[�Q�b�g
	ID3D11Texture2D*		m_pDepthStencil;
	ID3D11DepthStencilView*	m_pDepthStencilView;
	
	// ���OZ�o�b�t�@
	ID3D11Texture2D*			m_pMyDepthTexture = nullptr;
	ID3D11RenderTargetView*		m_pMyDepthRTView = nullptr;
	ID3D11ShaderResourceView*	m_pMyDepthSRView = nullptr;

private:
	void CreateSurface();
	void CreateTexture(ID3D11Texture2D** pTex, UINT x, UINT y, DXGI_FORMAT Format);
	void CreateSRV(ID3D11Texture2D* pTex, ID3D11ShaderResourceView** pView, DXGI_FORMAT Format);		// �V�F�[�_����ǂݍ��ސ�pView
	void CreateRTV(ID3D11Texture2D* pTex, ID3D11RenderTargetView** pView, DXGI_FORMAT Format);		// �������݂̂��߂�View
	void CreateUAV(ID3D11Texture2D* pTex, ID3D11UnorderedAccessView** pView, DXGI_FORMAT Format);	// �ǂݏ����ėpView
	void CreateRenderTarget();
	void CreateDepthStencil();

public:
	ShaderManager();
	~ShaderManager();

	void SetPointerSet(MatrixBox** pPS);

	void Init(DirectX11Base* pDx11, ShaderBox* pshader);
	void SetDataBoxPointer(DataBox* p){pDataBox=p;}
	void SetMatrixBoxManager(MatrixBoxManager* p){pMatBoxM=p;}
	void SetBBBManager(BillBoardBeamManager* p){ pBBBM = p; }
	void SetPASCmpTex(PASComputeTexture* p){ pPASCT = p; }
	void Set2DDrawer(My2DDrawManager* p){ p2DDrawer = p; }
	void SetInputDevice(Input *p){pInput = p;}

	//void SetViewProj(mat4* matView, mat4* matProj, vec3* pCamPos);
	//void SetViewProj(mat4* matView, mat4* matProj, vec3* pCamPos, vec3* pCamDir);

	//���ԕ`��
	//void DrawTime(D3DXMATRIX *world, float scale, DWORD mSec);

	void SetTex(int* Dest, const int Src);
	void SetMeshIdx(int* Dest, const int Src);

	void Draw();
	void DrawObject(); // �e�V�F�[�_�[����Ă΂��DataBox�ւ�Mesh�`�施��
	//void DrawTexture(ID3DXEffect* p); // �e�V�F�[�_�[����Ă΂��DataBox�ւ�TextureSet����

	void BeginDraw();
	void EndDraw();

#ifdef _DEBUG
	//int debugerMeshID;
	//int debugerTexID;
	//void DebugerInit();
	//void Debuger(D3DXVECTOR3 *pos,float scale=1.0f);
#endif

};

#endif