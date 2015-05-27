
#include"PAS_Common.hlsl"

//////////////////////////////////////////////////////////////
// 定数
static const float ISun = 100.0;
static const float exposure = 0.4;

//////////////////////////////////////////////////////////////
// texture
//////////////////////////////////////////////////////////////

Texture2D  refT  : register(t0);
Texture2D  traT  : register(t1);
Texture2D  irrT  : register(t2);
Texture3D  insT  : register(t3);

//SamplerState Sampler  : register(s0);

//////////////////////////////////////////////////////////////
// function
//////////////////////////////////////////////////////////////
// if return>0 , ray hit ground;  return = length from camera to ground

float ComputeD_hlslFix(float r, float mu, float R){
	// original fomula
	//float t = -r * mu - sqrt(r * r * (mu * mu - 1.0) + R * R);
	float temp = r * r * (mu * mu - 1.0) + R * R;
	temp = (temp >= 0) ? sqrt(temp) : 1e100;
	return -r * mu - temp;
}

//inscattered light along ray x+tv, when sun in direction s (=S[L]-T(x,x0)S[L]|x0)
float3 inscatter(inout float3 x, inout float t, float3 v, float3 s, inout float r, inout float mu, out float3 attenuation) {
	attenuation = float3(1, 1, 1); // temp
	float3 result;
	//r = length(x);
	//mu = dot(x, v) / r;
	//float d = -r * mu - sqrt(r * r * (mu * mu - 1.0) + Rt * Rt);
	float d = ComputeD_hlslFix(r, mu, Rt);

	if (d > 0.0) { // if x in space and ray intersects atmosphere
		// move x to nearest intersection of ray with top atmosphere boundary
		x += d * v;
		t -= d;
		mu = (r * mu + d) / Rt;	// mu -> mu = dot(ray, N of ground)(only when ray hit ground) [-1.0, 0.0]
		r = Rt;
	}
	if (r <= Rt) { // if ray intersects atmosphere
		float nu = dot(v, s);
		float muS = dot(x, s) / r;
		float phaseR = phaseFunctionR(nu);
		float phaseM = phaseFunctionM(nu);
		float4 inscatter = max(texture4D(insT, r, mu, muS, nu), 0.0);

		if (t > 0.0) {
			float3 x0 = x + t * v; // Coord of the ground in the direction of ray
			float r0 = length(x0);
			float rMu0 = dot(x0, v);
			float mu0 = rMu0 / r0;
			float muS0 = dot(x0, s) / r0;

			// avoids imprecision problems in transmittance computations based on textures
			//attenuation = analyticTransmittance(r, mu, t);

//			if (r0 > Rg + 0.01) { //山とか？   //// r0をzバッファに置き換えると使うようになるのかも
//				// computes S[L]-T(x,x0)S[L]|x0
//				inscatter = max(inscatter - attenuation.rgbr * texture4D(insT, r0, mu0, muS0, nu), 0.0);
//// FIX
//				// avoids imprecision problems near horizon by interpolating between two points above and below horizon
//				const float EPS = 0.004;
//				float muHoriz = -sqrt(1.0 - (Rg / r) * (Rg / r));
//				if (abs(mu - muHoriz) < EPS) {
//					float a = ((mu - muHoriz) + EPS) / (2.0 * EPS);
//
//					mu = muHoriz - EPS;
//					r0 = sqrt(r * r + t * t + 2.0 * r * t * mu);
//					mu0 = (r * mu + t) / r0;
//					float4 inScatter0 = texture4D(insT, r, mu, muS, nu);
//					float4 inScatter1 = texture4D(insT, r0, mu0, muS0, nu);
//					float4 inScatterA = max(inScatter0 - attenuation.rgbr * inScatter1, 0.0);
//
//					mu = muHoriz + EPS;
//					r0 = sqrt(r * r + t * t + 2.0 * r * t * mu);
//					mu0 = (r * mu + t) / r0;
//					inScatter0 = texture4D(insT, r, mu, muS, nu);
//					inScatter1 = texture4D(insT, r0, mu0, muS0, nu);
//					float4 inScatterB = max(inScatter0 - attenuation.rgbr * inScatter1, 0.0);
//
//					inscatter = lerp(inScatterA, inScatterB, a);
//				}
//// end FIX
//			}
		}
// FIX
		// avoids imprecision problems in Mie scattering when sun is below horizon
		inscatter.w *= smoothstep(0.00, 0.02, muS);
// end FIX
		result = max(inscatter.rgb * phaseR + getMie(inscatter) * phaseM, 0.0);
	}
	else { // x in space and ray looking in space
		result = float3(0,0,0);
	}
	return result * ISun;
}


//ground radiance at end of ray x+tv, when sun in direction s
//attenuated bewteen ground and viewer (=R[L0]+R[L*])
float3 groundColor(float3 x, float t, float3 v, float3 s, float r, float mu, float3 attenuation)
{
	float3 result;
	if (t > 0.0) { // if ray hits ground surface
		// ground reflectance at end of ray, x0
		float3 x0 = x + t * v;
		float r0 = length(x0);
		float3 n = x0 / r0;

		float2 coords = float2(atan2(n.z, n.x), acos(n.y)) * float2(0.5, 1.0) / M_PI;
		float4 reflectance = refT.Sample(Sampler_Wrap, coords);// *float4(0.2, 0.2, 0.2, 1.0);
		if (r0 > Rg + 0.01) {
			reflectance = float4(0.4, 0.4, 0.4, 0.0);
		}

		// direct sun light (radiance) reaching x0
		float muS = dot(n, s);
		float3 sunLight = transmittanceWithShadow(r0, muS);

		// precomputed sky light (irradiance) (=E[L*]) at x0
		float3 groundSkyLight = irradiance(irrT, r0, muS);

		// light reflected at x0 (=(R[L0]+R[L*])/T(x,x0))
		float3 groundColor = reflectance.rgb * (max(muS, 0.0) * sunLight + groundSkyLight) * ISun / M_PI;

		// water specular color due to sunLight
		if (reflectance.w > 0.0) {
			float3 h = normalize(s - v);
			float fresnel = 0.02 + 0.98 * pow(1.0 - dot(-v, h), 5.0);
			float waterBrdf = fresnel * pow(max(dot(h, n), 0.0), 150.0);
			groundColor += reflectance.w * max(waterBrdf, 0.0) * sunLight * ISun;
		}

		//result = attenuation * groundColor; //=R[L0]+R[L*]
		result = groundColor;
	}
	else { // ray looking at the sky
		result = float3(0,0,0);
	}
	return result;
}

// direct sun light for ray x+tv, when sun in direction s (=L0)
float3 sunColor(float3 x, float t, float3 v, float3 s, float r, float mu) {
	if (t > 0.0) {
		return float3(0, 0, 0);
	}
	else {
		float3 transmittance = (r <= Rt ? transmittanceWithShadow(r, mu) : float3(1.0, 1.0, 1.0)); // T(x,xo)
		//float3 transmittance =  transmittanceWithShadow(r, mu);
		float isun = step(cos(M_PI / 180.0), dot(v, s)) * ISun; // Lsun
		return transmittance * isun; // Eq (9)
	}
}

float3 TransColor(float3 srcColor){
	float3 c1 = srcColor.x * Color1;
	float3 c2 = srcColor.y * Color2;
	float3 c3 = srcColor.z * Color3;
	return (c1 + c2 + c3);
}

float3 HDR(float3 L) {
	L = L * exposure;
	L.r = L.r < 1.413 ? pow(L.r * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.r);
	L.g = L.g < 1.413 ? pow(L.g * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.g);
	L.b = L.b < 1.413 ? pow(L.b * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.b);
	return L;
}

//////////////////////////////////////////////////////////////
// ps
//////////////////////////////////////////////////////////////
struct PS_IN
{
	float4 Pos	: SV_Position;
	float  dir	: TEXCOORD0;
	float3 ray	: TEXCOORD1;
};

struct PS_OUT
{
	float4 MainCol	: SV_TARGET0;
};

PS_OUT main( PS_IN In )
{ 
	PS_OUT Out = (PS_OUT)0;

	float3 x = cam.xyz;
	float3 v = normalize(In.ray);
	float3 s = normalize(SunDir.xyz);

	float r = length(x);
	float mu = dot(x, v) / r;
	//float t = -r * mu - sqrt(r * r * (mu * mu - 1.0) + Rg * Rg); 
	float t = ComputeD_hlslFix(r, mu, Rg);

	//float3 g = x - float3(0.0, 0.0, Rg + 10.0);
	//float a = v.x * v.x + v.y * v.y - v.z * v.z;
	//float b = 2.0 * (g.x * v.x + g.y * v.y - g.z * v.z);
	//float c = g.x * g.x + g.y * g.y - g.z * g.z;
	//float d = -(b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
	//bool cone = d > 0.0 && abs(x.z + d * v.z - Rg) <= 10.0;

	//if (t > 0.0) {
	//	if (cone && d < t) {
	//		t = d;
	//	}
	//}
	//else if (cone) {
	//	t = d;
	//}

	float3 attenuation;
	float3 inscatterCol = inscatter(x, t, v, s.xyz, r, mu, attenuation); //S[L]-T(x,xs)S[l]|xs
	float3 groundCol = groundColor(x, t, v, s.xyz, r, mu, attenuation); //R[L0]+R[L*]
	float3 sunCol = sunColor(x, t, v, s.xyz, r, mu); //L0
	//Out.MainCol = float4(TransColor(sunCol + groundCol + inscatterCol), 1); // Eq (16)
	Out.MainCol = float4(TransColor(sunCol + groundCol + inscatterCol), 1); // Eq (16)
	//Out.MainCol = float4(HDR(groundCol), 1);
	//float test = (r > Rg*2) ? 1.0 : 0;
	//float test2 = sqrt(mu*mu*2 - 1.0);
	//Out.MainCol = float4(-d, t, cone ? 1:0, 1);
	//Out.MainCol = float4(analyticTransmittance(r, mu, t), 1);
	//Out.MainCol = float4(opticalDepth(HR, r, mu, t)*sign(t), 0, 0, 1);
	//Out.MainCol = float4(exp((Rg - r) / HR)*sign(t)*10000, 0, 0, 1);
	//Out.MainCol = float4(In.ray*1000, 1);
	return Out;
}
