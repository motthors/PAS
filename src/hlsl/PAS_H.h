///////////////////////////////////////////////////////////////
//// context buffer
//
//cbuffer cbSceneParam : register(b0)
//{
//	// ���C�ϊ��p�}�g���N�X
//	//matrix	matViewI	: packoffset(c0);
//	//matrix	matProjI	: packoffset(c4);
//	matrix	matVIxPI	: packoffset(c0);
//	float4	cam : packoffset(c4);
//
//	//// ���a�f�[�^�@
//	float Rg : packoffset(c5.x); // �n�\�܂ł̔��a
//	float Rt : packoffset(c5.y); // ��C�̑w�̔��a
//	float RL : packoffset(c5.z);
//
//	//// ���C���[�U���W��
//	float HR : packoffset(c5.w) = 8.0;
//	float3 betaR : packoffset(c6) = float3(5.8e-3, 1.35e-2, 3.31e-2);
//
//	//// �~�[�U���W��
//	float HM		: packoffset(c6.w);
//	float3 betaMSca	: packoffset(c7);
//	float mieG		: packoffset(c7.w);
//	float3 betaMEx	: packoffset(c8);
//
//	float coord : packoffset(c8.w);
//};
//
//
//float4 s = float4(1.0, 1.0, 1.0, 1.0);
//
//
//// transmittance T ��C�̓��ߓx
//static const int TRANSMITTANCE_W = 256;
//static const int TRANSMITTANCE_H = 64;
//
//// irradiance E 
//static const int SKY_W = 64;
//static const int SKY_H = 16;
//
//// scatter S
//static const int RES_R = 32;
//static const int RES_MU = 128;
//static const int RES_MU_S = 32;
//static const int RES_NU = 8;