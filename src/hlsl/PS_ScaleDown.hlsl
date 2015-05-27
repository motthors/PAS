
//////////////////////////////////////////////////////////////
// 定数
static const float exposure = 0.4;

//////////////////////////////////////////////////////////////
// テクスチャ

Texture2D  SrcTex  : register(t0);
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

float4 HDR(float4 L) {
	L.rgb = L.rgb * exposure;
	L.r = L.r < 1.413 ? pow(L.r * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.r);
	L.g = L.g < 1.413 ? pow(L.g * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.g);
	L.b = L.b < 1.413 ? pow(L.b * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.b);
	L.a = 1.0;
	return L;
}

PS_OUT main(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.MainCol = SrcTex.Sample(Sampler, In.uv);
	//Out.MainCol = SrcTex[(uint2)In.pos];
	Out.MainCol = HDR(Out.MainCol);

	return Out;
}

