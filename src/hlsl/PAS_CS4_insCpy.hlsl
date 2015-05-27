#include"PAS_Common.hlsl"


//irradiance1
[numthreads(RES_R, 1, RES_MU_S)]
void main(uint3 did : SV_DispatchThreadID)
{
	// UAV2 <- Tex3,Tex4
	float4 ray = Tex3[did]; //deltaSR
	float4 mie = Tex4[did]; //deltaSM
	rwOutputTex2[did] = float4(ray.rgb, mie.r); // store only red component of single Mie scattering (cf. "Angular precision")
}
