
#include"PAS_Common.hlsl"

//////////////////////////////////////////////////////////////
// 定数
static const float ISun = 100.0;
static const float exposure = 0.4;

//////////////////////////////////////////////////////////////
// texture
Texture2D  refT  : register(t0);
Texture2D  traT  : register(t1);
Texture2D  irrT  : register(t2);
Texture3D  insT  : register(t3);
Texture2D  tTmr  : register(t4);
Texture2D  xyzt  : register(t5);

//////////////////////////////////////////////////////////////
// function


//ground radiance at end of ray x+tv, when sun in direction s
//attenuated bewteen ground and viewer (=R[L0]+R[L*])
float3 groundColor(float3 x, float t, float3 v, float3 s, float r, float mu)
{
	float3 result;
	if (t > 0.0) { // if ray hits ground surface
		// ground reflectance at end of ray, x0
		float3 x0 = x + t * v;
		float r0 = length(x0);
		float3 n = x0 / r0;

		float2 coords = float2(atan(-n.x / n.z), acos(n.y)) * float2(0.5, 1.0) / M_PI;
		float4 reflectance = refT.Sample(Sampler, coords);// *float4(0.2, 0.2, 0.2, 1.0);
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


float4 HDR(float4 L) {
	L = L * exposure;
	L.r = L.r < 1.413 ? pow(L.r * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.r);
	L.g = L.g < 1.413 ? pow(L.g * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.g);
	L.b = L.b < 1.413 ? pow(L.b * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.b);
	L.a = L.a < 1.413 ? pow(L.a * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.a);
	return L;
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
	float4  Depth	: SV_TARGET1;
	float sv_depth	: SV_DEPTH;
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
	if (t2 != T)clip(-1);

	float3 groundCol = groundColor(x, t, v, s.xyz, r, mu); //R[L0]+R[L*]

	Out.MainCol = float4(groundCol, 1.0);
	//Out.MainCol = float4(tTmr.Sample(Sampler, In.Pos / float2(1280, 720)).rgb,1);
	//Out.MainCol = float4(In.ray,1.0);
	
	// 深度テスト用Depth計算
	//if (t2 >= 0)
	//	Out.sv_depth = t2*-In.dir / 10000000.0; //temp
	//else
	//	Out.sv_depth = 1.0;
	Out.sv_depth = 1.0;

	return Out;
}
