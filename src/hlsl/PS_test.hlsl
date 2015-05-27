
//////////////////////////////////////////////////////////////
// ピクセルシェーダテスト

Texture2D  Tex  : register(t1);
SamplerState Sampler  : register(s0);

struct PS_IN
{
	float4 pos	: SV_POSITION;
	float  dirMu : TEXCOORD0;
	float3 ray	: TEXCOORD1;
};


struct PS_OUT
{
	float4 MainCol	: SV_Target0;
};


PS_OUT main(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.MainCol = Tex.Sample(Sampler, In.pos / float2(1920, 1080));
	//Out.MainCol = Tex.Sample(Sampler, float3(In.pos / float2(1920, 1080),0.5));

	return Out;
}

