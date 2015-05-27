#include"PAS_Common.hlsl"


cbuffer gcbSceneParam : register(b2)
{
	float	k : packoffset(c0);
};

//irradiance1
[numthreads(SKY_W, SKY_H, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	//float2 uv = did.xy / vec2(SKY_W, SKY_H);
	//rwOutputTex[did.xy] = k * Tex0.SampleLevel(Sampler, uv, 0); // k=0 for line 4, k=1 for line 10
	rwOutputTex[did.xy] = k * Tex0[did.xy]; // k=0 for line 4, k=1 for line 10
}
