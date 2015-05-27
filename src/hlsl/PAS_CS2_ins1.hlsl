#include"PAS_Common.hlsl"



void integrand(float r, float mu, float muS, float nu, float t, out float3 ray, out float3 mie) {
	ray = float3(0.0, 0.0, 0.0);
	mie = float3(0.0, 0.0, 0.0);
	float ri = sqrt(r * r + t * t + 2.0 * r * mu * t);
	float muSi = (nu * t + muS * r) / ri;
	ri = max(Rg, ri);
	if (muSi >= -sqrt(1.0 - Rg * Rg / (ri * ri))) {
		float3 ti = transmittance(r, mu, t) * transmittance(ri, muSi);
		ray = exp(-(ri - Rg) / HR) * ti;
		mie = exp(-(ri - Rg) / HM) * ti;
	}
}

void inscatter(float r, float mu, float muS, float nu, out float3 ray, out float3 mie) {
	ray = float3(0.0, 0.0, 0.0);
	mie = float3(0.0, 0.0, 0.0);
	float dx = limit(r, mu) / float(INSCATTER_INTEGRAL_SAMPLES);
	float xi = 0.0;
	float3 rayi;
	float3 miei;
	integrand(r, mu, muS, nu, 0.0, rayi, miei);
	for (int i = 1; i <= INSCATTER_INTEGRAL_SAMPLES; ++i) {
		float xj = float(i) * dx;
		float3 rayj;
		float3 miej;
		integrand(r, mu, muS, nu, xj, rayj, miej);
		ray += (rayi + rayj) / 2.0 * dx;
		mie += (miei + miej) / 2.0 * dx;
		xi = xj;
		rayi = rayj;
		miei = miej;
	}
	ray *= betaR;
	mie *= betaMSca;
}


//inscatter1
[numthreads(RES_R, 1, RES_MU_S)]
void main(uint3 did : SV_DispatchThreadID)
{
	int x = did.x;
	int y = did.y;
	int z = did.z;
	float3 ray;
	float3 mie;
	float mu, muS, nu;
	float r;
	float4 dhdH;

	SetValue(r, dhdH, z);
	getMuMuSNu(r, dhdH, float2(x,y), mu, muS, nu);
	inscatter(r, mu, muS, nu, ray, mie);
	rwOutputTex2[int3(x, y, z)] = float4(ray, 1.0);
	rwOutputTex3[int3(x, y, z)] = float4(mie, 1.0);
}
