#include"PAS_Common.hlsl"

cbuffer gcbSceneParam : register(b2)
{
	float	first : packoffset(c0);
};


static const float dphi = M_PI / float(INSCATTER_SPHERICAL_INTEGRAL_SAMPLES);
static const float dtheta = M_PI / float(INSCATTER_SPHERICAL_INTEGRAL_SAMPLES);


void inscatter(float r, float mu, float muS, float nu, out float3 raymie) {
	r = clamp(r, Rg, Rt);
	mu = clamp(mu, -1.0, 1.0);
	muS = clamp(muS, -1.0, 1.0);
	float var = sqrt(1.0 - mu * mu) * sqrt(1.0 - muS * muS);
	nu = clamp(nu, muS * mu - var, muS * mu + var);

	float cthetamin = -sqrt(1.0 - (Rg / r) * (Rg / r));

	float3 v = float3(sqrt(1.0 - mu * mu), 0.0, mu);
	float sx = v.x == 0.0 ? 0.0 : (nu - muS * mu) / v.x;
	float3 s = float3(sx, sqrt(max(0.0, 1.0 - sx * sx - muS * muS)), muS);

	raymie = float3(0,0,0);

	// integral over 4.PI around x with two nested loops over w directions (theta,phi) -- Eq (7)
	for (int itheta = 0; itheta < INSCATTER_SPHERICAL_INTEGRAL_SAMPLES; ++itheta) {
		float theta = (float(itheta) + 0.5) * dtheta;
		float ctheta = cos(theta);

		float greflectance = 0.0;
		float dground = 0.0;
		float3 gtransp = float3(0, 0, 0);
		if (ctheta < cthetamin) { // if ground visible in direction w
			// compute transparency gtransp between x and ground
			greflectance = AVERAGE_GROUND_REFLECTANCE / M_PI;
			dground = -r * ctheta - sqrt(r * r * (ctheta * ctheta - 1.0) + Rg * Rg);
			gtransp = transmittance(Rg, -(r * ctheta + dground) / Rg, dground);
		}

		for (int iphi = 0; iphi < 2 * INSCATTER_SPHERICAL_INTEGRAL_SAMPLES; ++iphi) {
			float phi = (float(iphi) + 0.5) * dphi;
			float dw = dtheta * dphi * sin(theta);
			float3 w = float3(cos(phi) * sin(theta), sin(phi) * sin(theta), ctheta);

			float nu1 = dot(s, w);
			float nu2 = dot(v, w);
			float pr2 = phaseFunctionR(nu2);
			float pm2 = phaseFunctionM(nu2);

			// compute irradiance received at ground in direction w (if ground visible) =deltaE
			float3 gnormal = (float3(0.0, 0.0, r) + dground * w) / Rg;
			float3 girradiance = irradiance(Tex2, Rg, dot(gnormal, s));

			float3 raymie1; // light arriving at x from direction w

			// first term = light reflected from the ground and attenuated before reaching x, =T.alpha/PI.deltaE
			raymie1 = greflectance * girradiance * gtransp;

			// second term = inscattered light, =deltaS
			if (first == 1.0) {
				// first iteration is special because Rayleigh and Mie were stored separately,
				// without the phase functions factors; they must be reintroduced here
				float pr1 = phaseFunctionR(nu1);
				float pm1 = phaseFunctionM(nu1);
				float3 ray1 = texture4D(Tex3, r, w.z, muS, nu1).rgb;
				float3 mie1 = texture4D(Tex4, r, w.z, muS, nu1).rgb;
				raymie1 += ray1 * pr1 + mie1 * pm1;
			}
			else {
				raymie1 += texture4D(Tex3, r, w.z, muS, nu1).rgb;
			}

			// light coming from direction w and scattered in direction v
			// = light arriving at x from direction w (raymie1) * SUM(scattering coefficient * phaseFunction)
			// see Eq (7)
			raymie += raymie1 * (betaR * exp(-(r - Rg) / HR) * pr2 + betaMSca * exp(-(r - Rg) / HM) * pm2) * dw;
		}
	}

	// output raymie = J[T.alpha/PI.deltaE + deltaS] (line 7 in algorithm 4.1)
}

// Tex2 = deltaE
// UAV2(dJ) <- Tex3(SR), Tex4(SM)
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
	inscatter(r, mu, muS, nu, raymie);
	rwOutputTex2[did] = float4(raymie, 1.0);
}
