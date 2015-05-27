#ifndef _CREATE_TEXTURE_H_
#define _CREATE_TEXTURE_H_

#include"IncludeAll.h"

//���̂Ƃ���A���S�����񂩂�e�N�X�`��������p�N���X
//DirectX�f�o�C�X�ƃQ�[���̊Ԃ̃N�b�V����

class CreateTexture
{

	DXDEVICE*	m_pDevice;
	DXCONTEXT*	m_pContext;
	HFONT hFont;			//�e�N�X�`���p�t�H���g�f�o�C�X
	UINT m_FontSize;

public:

	CreateTexture();
	~CreateTexture();

	void SetD3Ddev(DXDEVICE* dev, DXCONTEXT* pcont){ m_pDevice = dev; m_pContext = pcont; }
	void Init(int fontsize=200);

	void FromString(TCHAR* text, DXTEXTURE2D** pTex, ID3D11ShaderResourceView** pView, float *pRatio, int fontsize = 200);

};

#endif