
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
Texture2D  tTmr  : register(t4);
Texture2D  xyzt  : register(t5);

//SamplerState Sampler  : register(s0);

//////////////////////////////////////////////////////////////
// function
//////////////////////////////////////////////////////////////
// if return>0 , ray hit ground;  return = length from camera to ground

//inscattered light along ray x+tv, when sun in direction s (=S[L]-T(x,x0)S[L]|x0)
float3 inscatter(float3 x, float t, float3 v, float3 s, float r, float mu){
	float3 result;

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

			//float3 attenuation = float3(1, 1, 1); // temp
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


//////////////////////////////////////////////////////////////
// ps
//////////////////////////////////////////////////////////////
struct PS_IN
{
	float4 Pos	: SV_POSITION;
	float  dir	: TEXCOORD0;
	float3 ray	: TEXCOORD1;
};

struct PS_OUT
{
	float4 MainCol	: SV_TARGET;
	float sv_depth : SV_DEPTH;
};

PS_OUT main( PS_IN In )
{ 
	PS_OUT Out = (PS_OUT)0;

	float3 x = xyzt[(uint2)In.Pos].xyz;
	float3 v = normalize(In.ray);
	float3 s = normalize(SunDir);

	float t = tTmr[(uint2)In.Pos].x;
	float T = tTmr[(uint2)In.Pos].y;
	float mu= tTmr[(uint2)In.Pos].z;
	float r = tTmr[(uint2)In.Pos].w;
	
	float t2 = xyzt[(uint2)In.Pos].w;
	//if (t2 != T)clip(-1);

	float3 inscatterCol = inscatter(x, t, v, s, r, mu); //S[L]-T(x,xs)S[l]|xs
	Out.MainCol = float4(inscatterCol, 1.0);

	Out.sv_depth = 0.0;

	return Out;
}
