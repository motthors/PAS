#include"PAS_Common.hlsl"

cbuffer gcbSceneParam : register(b2)
{
	float	first : packoffset(c0);
};


static const float dphi = M_PI / float(INSCATTER_SPHERICAL_INTEGRAL_SAMPLES);
static const float dtheta = M_PI / float(INSCATTER_SPHERICAL_INTEGRAL_SAMPLES);


float3 integrand(float r, float mu, float muS, float nu, float t) {
	float ri = sqrt(r * r + t * t + 2.0 * r * mu * t);
	float mui = (r * mu + t) / ri;
	float muSi = (nu * t + muS * r) / ri;
	return texture4D(Tex3, ri, mui, muSi, nu).rgb * transmittance(r, mu, t);
}


float3 inscatter(float r, float mu, float muS, float nu) {
	float3 raymie = float3(0,0,0);
	float dx = limit(r, mu) / float(INSCATTER_INTEGRAL_SAMPLES);
	float xi = 0.0;
	float3 raymiei = integrand(r, mu, muS, nu, 0.0);
	for (int i = 1; i <= INSCATTER_INTEGRAL_SAMPLES; ++i) {
		float xj = float(i) * dx;
		float3 raymiej = integrand(r, mu, muS, nu, xj);
		raymie += (raymiei + raymiej) / 2.0 * dx;
		xi = xj;
		raymiei = raymiej;
	}
	return raymie;
}


// Tex = transmittance
// UAV2(dE) <- Tex3(dJ)
// UAV3(insTmp) <- Tex4(ins)
//inscatter1
[numthreads(RES_R, 1, RES_MU_S)]
void main(uint3 did : SV_DispatchThreadID)
{
	int x = did.x;
	int y = did.y;
	int z = did.z;
	float3 raymie;
	float mu, muS, nu;
	float r;
	float4 dhdH;

	SetValue(r, dhdH, z);
	getMuMuSNu(r, dhdH, float2(x,y), mu, muS, nu);
	rwOutputTex2[did] = float4(inscatter(r, mu, muS, nu), 1.0);

	// inscatterTemp Copy
	rwOutputTex3[did] = Tex4[did];
}
