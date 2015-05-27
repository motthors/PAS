///////////////////////////////////////////////////////////
// テクスチャ

cbuffer cbBuffer0 : register(b0)
{
	float  g_Weight0 : packoffset(c0.x);
	float  g_Weight1 : packoffset(c0.y);
	float  g_Weight2 : packoffset(c0.z);
	float  g_Weight3 : packoffset(c0.w);
	float  g_Weight4 : packoffset(c1.x);
	float  g_Weight5 : packoffset(c1.y);
	float  g_Weight6 : packoffset(c1.z);
	float  g_Weight7 : packoffset(c1.w);
};

Texture2D  SrcTex  : register(t0);
SamplerState Sampler  : register(s0);


float4 PS_Blur_X(float2 texel)
{
	float4 Out = 0;

	//Out = tex2D(tex0,In.Tex);
	Out += (SrcTex.Sample(Sampler, texel + float2(g_Texel1, 0.0)) + SrcTex.Sample(Sampler, texel - float2(g_Texel1, 0.0))) * Weight1;
	Out += (SrcTex.Sample(Sampler, texel + float2(g_Texel2, 0.0)) + SrcTex.Sample(Sampler, texel - float2(g_Texel2, 0.0))) * Weight2;
	Out += (SrcTex.Sample(Sampler, texel + float2(g_Texel3, 0.0)) + SrcTex.Sample(Sampler, texel - float2(g_Texel3, 0.0))) * Weight3;
	Out += (SrcTex.Sample(Sampler, texel + float2(g_Texel4, 0.0)) + SrcTex.Sample(Sampler, texel - float2(g_Texel4, 0.0))) * Weight4;
	Out += (SrcTex.Sample(Sampler, texel + float2(g_Texel5, 0.0)) + SrcTex.Sample(Sampler, texel - float2(g_Texel5, 0.0))) * Weight5;
	Out += (SrcTex.Sample(Sampler, texel + float2(g_Texel6, 0.0)) + SrcTex.Sample(Sampler, texel - float2(g_Texel6, 0.0))) * Weight6;
	Out += (SrcTex.Sample(Sampler, texel + float2(g_Texel7, 0.0)) + SrcTex.Sample(Sampler, texel - float2(g_Texel7, 0.0))) * Weight7;
	Out += (SrcTex.Sample(Sampler, texel + float2(g_Texel0, 0.0)) + SrcTex.Sample(Sampler, texel - float2(g_Texel0, 0.0))) * Weight0;

	return Out;
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
	Out.MainCol = PS_Blur_X(In.uv);
	Out.MainCol = HDR(Out.MainCol);

	return Out;
}

