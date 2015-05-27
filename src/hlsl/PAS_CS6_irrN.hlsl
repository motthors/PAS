#include"PAS_Common.hlsl"

cbuffer gcbSceneParam : register(b2)
{
	float	first : packoffset(c0);
};


static const float dphi = M_PI / float(IRRADIANCE_INTEGRAL_SAMPLES);
static const float dtheta = M_PI / float(IRRADIANCE_INTEGRAL_SAMPLES);


// Tex2 = irradianceTexture
// UAV0(dD) <- Tex3(SR), Tex4(SM)
// UAV1(irrTmp) <- Tex0(irr)
//inscatter1
[numthreads(SKY_W, SKY_H, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	int x = did.x;
	int y = did.y;
	float r, muS;

	getIrradianceRMuS(x, y, r, muS);
	float3 s = float3(max(sqrt(1.0 - muS * muS), 0.0), 0.0, muS);

	float3 result = float3(0,0,0);
	// integral over 2.PI around x with two nested loops over w directions (theta,phi) -- Eq (15)
	for (int iphi = 0; iphi < 2 * IRRADIANCE_INTEGRAL_SAMPLES; ++iphi) {
		float phi = (float(iphi) + 0.5) * dphi;
		for (int itheta = 0; itheta < IRRADIANCE_INTEGRAL_SAMPLES / 2; ++itheta) {
			float theta = (float(itheta) + 0.5) * dtheta;
			float dw = dtheta * dphi * sin(theta);
			float3 w = float3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));
			float nu = dot(s, w);
			if (first == 1.0) {
				// first iteration is special because Rayleigh and Mie were stored separately,
				// without the phase functions factors; they must be reintroduced here
				float pr1 = phaseFunctionR(nu);
				float pm1 = phaseFunctionM(nu);
				float3 ray1 = texture4D(Tex3, r, w.z, muS, nu).rgb;
				float3 mie1 = texture4D(Tex4, r, w.z, muS, nu).rgb;
				result += (ray1 * pr1 + mie1 * pm1) * w.z * dw;
			}
			else {
				result += texture4D(Tex3, r, w.z, muS, nu).rgb * w.z * dw;
			}
		}
	}

	rwOutputTex[did.xy] = float4(result, 0.0);

	// irradianceTemp Copy
	rwOutputTex1[did.xy] = Tex0[did.xy];
}
