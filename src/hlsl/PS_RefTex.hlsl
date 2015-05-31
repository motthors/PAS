//////////////////////////////////////////////////////////////
// Const Buffer
cbuffer m_2DconstBuffer : register(b0)
{
	float Alpha : packoffset(c0);
};
//////////////////////////////////////////////////////////////
// texture

//texture Tex;
//
//sampler2D Sampler : register(s0) = sampler_state{
//	texture = <Tex>;
//	MipFilter = LINEAR;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	AddressU = CLAMP;
//	AddressV = CLAMP;
//};

Texture2D  Tex  : register(t0);
SamplerState Sampler  : register(s0);

//////////////////////////////////////////////////////////////
// ps

struct PS_IN
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD0;
};

struct PS_OUT
{
	float4 MainCol	: SV_TARGET;
};


PS_OUT main(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.MainCol = Tex.Sample(Sampler, In.uv);
	Out.MainCol.a = Out.MainCol.a * Alpha;
	//Out.MainCol = float4(In.uv, 0, 1);
	return Out;
}

