///////////////////////////////////////////////////////////
// テクスチャ

Texture2D  SrcTex0  : register(t0);
Texture2D  SrcTex1  : register(t1);
Texture2D  SrcTex2  : register(t2);
Texture2D  SrcTex3  : register(t3);
SamplerState Sampler  : register(s0);


float4 ToneMap(float4 L)
{
	float weight = 0.5;
	L.r = 1.0 - exp(-L.r * weight);
	L.g = 1.0 - exp(-L.g * weight);
	L.b = 1.0 - exp(-L.b * weight);
	L.a = 1.0;
	return L;
}

struct PS_IN
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD0;
};


struct PS_OUT
{
	float4 MainCol	: SV_Target0;
};



PS_OUT main(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.MainCol =
		ToneMap(SrcTex0.Sample(Sampler, In.uv))
		+ SrcTex1.Sample(Sampler, In.uv)
		+ SrcTex2.Sample(Sampler, In.uv)
		+SrcTex2.Sample(Sampler, In.uv); //*/
	Out.MainCol.a = 1.0;
	return Out;
}

