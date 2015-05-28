///////////////////////////////////////////////////////////
// テクスチャ

Texture2D  SrcTex  : register(t0);
SamplerState Sampler  : register(s0);


float4 PS_Blur_Y(float2 texel)
{
	float4 Out = 0;

	//Out = tex2D(tex0,In.Tex);
	//Out += (SrcTex.Sample(Sampler, texel + float2(0.0, g_Texel0)) + SrcTex.Sample(Sampler, texel - float2(0.0, g_Texel0))) * Weight0;
	//Out += (SrcTex.Sample(Sampler, texel + float2(0.0, g_Texel1)) + SrcTex.Sample(Sampler, texel - float2(0.0, g_Texel1))) * Weight1;
	//Out += (SrcTex.Sample(Sampler, texel + float2(0.0, g_Texel2)) + SrcTex.Sample(Sampler, texel - float2(0.0, g_Texel2))) * Weight2;
	//Out += (SrcTex.Sample(Sampler, texel + float2(0.0, g_Texel3)) + SrcTex.Sample(Sampler, texel - float2(0.0, g_Texel3))) * Weight3;
	//Out += (SrcTex.Sample(Sampler, texel + float2(0.0, g_Texel4)) + SrcTex.Sample(Sampler, texel - float2(0.0, g_Texel4))) * Weight4;
	//Out += (SrcTex.Sample(Sampler, texel + float2(0.0, g_Texel5)) + SrcTex.Sample(Sampler, texel - float2(0.0, g_Texel5))) * Weight5;
	//Out += (SrcTex.Sample(Sampler, texel + float2(0.0, g_Texel6)) + SrcTex.Sample(Sampler, texel - float2(0.0, g_Texel6))) * Weight6;
	//Out += (SrcTex.Sample(Sampler, texel + float2(0.0, g_Texel7)) + SrcTex.Sample(Sampler, texel - float2(0.0, g_Texel7))) * Weight7;
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
	Out.MainCol = PS_Blur_Y(In.uv);
	return Out;
}

