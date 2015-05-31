///////////////////////////////////////////////////////////
// テクスチャ

Texture2D  SrcTex1  : register(t0);
Texture2D  SrcTex2  : register(t1);
Texture2D  SrcTex3  : register(t2);
SamplerState Sampler  : register(s0);



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
		SrcTex1.Sample(Sampler, In.uv)*0.4
		+SrcTex2.Sample(Sampler, In.uv)*0.2
		+SrcTex2.Sample(Sampler, In.uv)*0.05;
	return Out;
}

