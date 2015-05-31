#ifndef _SHADER_VARIABLE_H_
#define _SHADER_VARIABLE_H_

#include"../base/IncludeAll.h"
#include"../dx11/DirectX11Base.h"
#include<d3dcompiler.h>
#include<vector>

 
typedef struct{
	TCHAR FileName[100];
	ID3D11VertexShader* ShaderObject;
	ID3D11InputLayout* Layout;
} VertexShaderStruct;

typedef struct{
	TCHAR FileName[100];
	ID3D11PixelShader* ShaderObject;
} PixelShaderStruct;

class ShaderBox{
	//DIrectX
	DXDEVICE*		m_pDevice;
	DXCONTEXT*		m_pContext;
	// �V�F�[�_�z��
	std::vector<VertexShaderStruct*>	VertexShaderBox;
	std::vector<PixelShaderStruct*>		PixelShaderBox;
	// ���ܐݒ肳��Ă���V�F�[�_��ID
	UINT								NowSetVertexShaderID;
	UINT								NowSetPixelShaderID;

	// ���̓��C�A�E�g���Œ�ɂ��Ă݂�e�X�g
	D3D11_INPUT_ELEMENT_DESC* m_pLayout;
	UINT m_LayoutNum = 2;

	// �}���`�p�X�����_�����O�p�z��
	static const int MultiRTNum = 8;
	ID3D11RenderTargetView *m_ArrpRTV[MultiRTNum];

	// �[�x�X�e���V���r���[�@���܂̂Ƃ��e������炤����
	ID3D11DepthStencilView*	m_pDepthStencilView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilOff0On1[2];
	int m_OnOffFlag = 0;
	// �[�x�X�e���V���X�e�[�gs
	static const int m_DepthStateNum = 3;
	ID3D11DepthStencilState* m_DepthStates[m_DepthStateNum];

	//int _CreateVertexShader(ID3D11VertexShader** OutVS, TCHAR* csoName, D3D11_INPUT_ELEMENT_DESC* inLayout, ID3D11InputLayout** outLayout, UINT LayoutNum);
	//int _CreatePixelShader(ID3D11PixelShader** OutPS, TCHAR* csoName);
	void _CreateVertexShader(ID3D11VertexShader** OutVS, TCHAR* csoName, ID3D11InputLayout** outLayout);
	void _CreatePixelShader(ID3D11PixelShader** OutPS, TCHAR* csoName);
public:
	//�`��pmatrix,vector
	mat4 *pmatWorld;
	mat4 matWIT;
	mat4 matView;
	mat4 matProj;
	mat4 matWVP;
	vec3 CameraPos; //vec3
	vec3 CameraDir; //vec3
	vec3 LightDir;  //vec4

	ShaderBox();
	~ShaderBox();
	void Init(DirectX11Base* pdx11);

	// �V�F�[�_�[�쐬
	UINT CreateVertexShader(TCHAR* csoName);
	UINT CreatePixelShader(TCHAR* csoName);
	void CreateComputeShader(ID3D11ComputeShader** outCS, TCHAR* csoName);
	// �V�F�[�_�[�o�^
	void SetShader(UINT idVS, UINT idPS);
	// �}�g���N�X�x�N�^�ݒ�
	void SetMatrixVector(mat4* V, mat4* P, vec3* pCamPos);

	// �e�N�X�`���A�r���[�쐬
	void CreateTexture2D(ID3D11Texture2D** ppTex, void* pInitData, size_t size, UINT x, UINT y, DXGI_FORMAT Format, UINT D3D11_BIND_Flags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
	void CreateTexture3D(ID3D11Texture3D** ppTex, void* pInitData, size_t size, UINT x, UINT y, UINT z, DXGI_FORMAT Format, UINT D3D11_BIND_Flags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
	void CreateSRV(ID3D11Resource* pTex, ID3D11ShaderResourceView** ppSRV, DXGI_FORMAT Format, D3D11_SRV_DIMENSION ViewDimension);		// �V�F�[�_����ǂݍ��ސ�pView
	void CreateRTV(ID3D11Resource* pTex, ID3D11RenderTargetView** ppRTV, DXGI_FORMAT Format, D3D11_RTV_DIMENSION ViewDimension);		// �������݂̂��߂�View
	void CreateUAV(ID3D11Resource* pTex, ID3D11UnorderedAccessView** ppUAV, DXGI_FORMAT Format, D3D11_UAV_DIMENSION ViewDimension);		// �ǂݏ����ėpView

	// �����_�[�^�[�Q�b�g�ݒ�
	void ChangeRenderTarget(UINT index, ID3D11RenderTargetView* pRTV);
	void SetRTsToShader();
	// Depth�ݒ�
	void CreateDepthStencilStates();
	void SetDepthstencil(ID3D11DepthStencilView* p){ m_pDepthStencilOff0On1[1] = p; }
	void SetDepthFlag(int on1_or_off0){ m_OnOffFlag = on1_or_off0; }
	enum KindOfDepthState{
		No,
		On_Less_WriteOK,
		On_AllOK_WriteNO
	};
	void SetDepthState(KindOfDepthState num);
};

#endif