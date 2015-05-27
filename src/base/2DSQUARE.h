#ifndef _2DSQ_H_
#define _2DSQ_H_
////////////////////////////////////////////////////////////////
//�|�X�g�G�t�F�N�g�p�̃E�B���h�E�Ɠ����傫���̃|���S���`��N���X
////////////////////////////////////////////////////////////////

#include"IncludeAll.h"
#include"../dx11/DirectX11Base.h"
#include"../Shader/ShaderBox.h"

class D3D2DSQUARE
{
private:
	//���_�t�H�[�}�b�g
	DirectX11Base*	m_pDx11;
	ShaderBox*		m_pShaderBox;

	//���_�錾
	typedef struct{
		float x, y, z;		// ���W
		float u, v;			// UV���W
	} VERTEX;
	// �o�b�t�@
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	// �V�F�[�_�[
	UINT	m_idVertexShader;
	UINT	m_idPixelShader;
	// ���C�A�E�g

	DXDEVICE*		m_pDevice;
	DXCONTEXT*		m_pContext;

	//�Q�c�|���S���̃T�C�Y
	UINT Width, Height;

public:
	D3D2DSQUARE();
	D3D2DSQUARE(DirectX11Base* pdx11, ShaderBox* psb, UINT Width, UINT Height);
	~D3D2DSQUARE();
	//������
	void Init();
	void Invalidate();
	//2D�|���S���̃T�C�Y���擾
	UINT GetWidth();
	UINT GetHeight();
   //�V�F�[�_��o�^
	void SetVertexShader(UINT idpv);
	void SetPixelShader(UINT idps);
	//2D�|���S���������_�����O
	void Render();
};


#endif