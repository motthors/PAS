//�O������ݒ肷��ϐ�///////////////////////////////////

//���[���h�A�r���[�A�ˉe���W�ϊ��}�g���b�N�X
float4x4 matWorld : World;
float4x4 matWorldIT : WorldInverseTranspose = (float4x4)0;
float4x4 matWVP : WorldViewProjection;

float4 vecCameraPos;

//�Ɩ�/////////////////////////////////////////////////
//���s�Ɩ�
//�Ɩ��̕����x�N�g��
float4 LightDir = {0.0, -1.0, -1.0, 1.0};
float3 SpecColor : Specular = {1.0f,1.0f,1.0f};
float  shininess = 10.0f;  //�X�y�L�����[�̋��x
float  LambertPower = 1.0;       //�����o�[�g���x
float  SpecularPower = 0.9;       //�X�y�L�����[�̋��x
float3 AmbiColor : Ambient = {0.01f,0.01f,0.01f};	//����

//���R����
float Emission = 0.0f;

#include"DefineTexture.h"