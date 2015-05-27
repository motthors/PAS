//////////////////////////////////////////////////////////////
// Const Buffer
cbuffer cbSceneParam : register(b0)
{
	float coord : packoffset(c4.w);
}
//////////////////////////////////////////////////////////////
// texture
Texture3D  Tex  : register(t0);
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
	Out.MainCol = Tex.Sample(Sampler, float3(In.uv, coord));
	return Out;
}

