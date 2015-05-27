#include"PAS_Common.hlsl"


//irradiance1
[numthreads(SKY_W, SKY_H, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	int x = did.x;
	int y = did.y;
	float r, muS;
	getIrradianceRMuS(x, y, r, muS);
	rwOutputTex[int2(x, y)] = float4(transmittance(r, muS) * max(muS, 0.0), 0.0);
}
