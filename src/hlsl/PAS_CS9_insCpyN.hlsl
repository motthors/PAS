#include"PAS_Common.hlsl"


// UAV2(ins) <- Tex4(insTmp) + Tex3(dSR)
[numthreads(RES_R, 1, RES_MU_S)]
void main(uint3 did : SV_DispatchThreadID)
{
	int x = did.x;
	int y = did.y;
	int z = did.z;
	float mu, muS, nu;
	float r;
	float4 dhdH;
	SetValue(r, dhdH, z);
	getMuMuSNu(r, dhdH, float2(x, y), mu, muS, nu);
	rwOutputTex2[did] = Tex4[did] + float4(Tex3[did].rgb / phaseFunctionR(nu), 0.0);
}
