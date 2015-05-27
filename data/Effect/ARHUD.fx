#include"DefineStruct.h"
#include"VS_Normal.h"
#include"PS_Normal.h"

technique Main
{
	pass p0
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_AR();

		// �����_�����O�X�e�[�g�ݒ�

		//RGB�J���[�v�f�ݒ� SetTextureStageState() D3DTSS
		ColorOp[0] = SELECTARG1;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp[0] = SELECTARG1;
		AlphaArg1[0] = TEXTURE;
		AlphaArg2[0] = DIFFUSE;
		ColorOp[1] = DISABLE;
		AlphaOp[1] = DISABLE;

		//SetRenderState()
		ZEnable = TRUE;			//D3DRS_ZENABLE				Z��r���s
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE		�[�x�o�b�t�@�֏�������
		CullMode = NONE;		//D3DRS_CULLMODE			�w�ʃJ�����O

		AlphaBlendEnable = TRUE;//D3DRS_ALPHABLENDENABLE	�A���t�@�u�����h���s
		AlphaTestEnable = TRUE; //D3DRS_ALPHATESTENABLE		�A���t�@�e�X�g���s
		AlphaFunc = GREATEREQUAL;//D3DRS_ALPHAFUNC			D3DCMP_�` �A���t�@�e�X�g���s���֐��@�V�����s�N�Z���l�����݂̃s�N�Z���l�ȏ�̂Ƃ�
		AlphaRef = 1;			//D3DRS_ALPHAREF			�A���t�@�e�X�g���̊�l

		SrcBlend = SRCALPHA;	//D3DRS_SRCBLEND			�������������s
		DestBlend = INVSRCALPHA;//D3DRS_DESTBLEND			���ߏ������s

	}
}
