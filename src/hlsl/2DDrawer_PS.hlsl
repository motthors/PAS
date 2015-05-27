
//////////////////////////////////////////////////////////////
//! �R���X�^���g�o�b�t�@
/*!
�R���X�^���g�o�b�t�@���g�p���邱�Ƃł������̃R���X�^���g���W�X�^��1�܂Ƃ߂ɂ��邱�Ƃ��ł���
���W�X�^�̕ω��̃^�C�~���O�ɂ���ĕ����Ă��������悢
*/
cbuffer cbSceneParam : register(b0)
{
	matrix	mtxView			: packoffset(c0);
	matrix	mtxProj			: packoffset(c4);
};

cbuffer cbMeshParam : register(b1)
{
	matrix	mtxWorld		: packoffset(c0);
	float4 Width_Shift = 0;
	float2 CenterPos = 0;
	float Alpha;
};

//////////////////////////////////////////////////////////////
// texture
texture Tex;

sampler2D Sampler : register(s0) = sampler_state{
	texture = <Tex>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//////////////////////////////////////////////////////////////
// ps
struct PS_IN
{
	float2 uv	: TEXCOORD0;
	float2 Pos	: TEXCOORD1; //�e�N�Z���̃p�[�X��ԍ��W
};

struct PS_OUT
{
	float4 MainCol	: COLOR0;
};



PS_OUT PS_Circle(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	float len = 600 * (0.006 - abs(length((In.Pos - CenterPos)*float2(1, Width_Shift.x / Width_Shift.y)) - Width_Shift.x*0.9));
	float col = clamp(len, 0.2, 1.0);
	float alf = 600 * (-length((In.Pos - CenterPos)*float2(1, Width_Shift.x / Width_Shift.y)) + Width_Shift.x*0.9);
	alf = clamp(alf, 0.0, 0.7);
	Out.MainCol = float4(col, col, col, alf*Alpha);
	//Out.MainCol = float4(1 * Width_Shift.z, 1 * Width_Shift.w, 1, 0.6);
	return Out;
}

PS_OUT PS_Donut(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	//			�{�P�x�@ ���ׂ��@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���a
	float len1 = 600 * (0.003 - abs(length((In.Pos - CenterPos)*float2(1, Width_Shift.x / Width_Shift.y)) - Width_Shift.x));
	float len2 = 600 * (0.003 - abs(length((In.Pos - CenterPos)*float2(1, Width_Shift.x / Width_Shift.y)) - (Width_Shift.x - 0.08)));
	float col = clamp(len1, 0.1, 1.0) + clamp(len2, 0.1, 1.0);
	float alf = 800 * (0.046 - abs(length((In.Pos - CenterPos)*float2(1, Width_Shift.x / Width_Shift.y)) - (Width_Shift.x*0.98 - 0.035)));
	alf = clamp(alf, 0.0, 0.7);
	Out.MainCol = float4(col, col, col, alf*Alpha);
	//Out.MainCol = float4(1 * Width_Shift.z, 1 * Width_Shift.w, 1, 0.6);
	return Out;
}