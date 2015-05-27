
#include"PAS_Common.hlsl"

//////////////////////////////////////////////////////////////
// ’è”
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

//////////////////////////////////////////////////////////////
// function
//////////////////////////////////////////////////////////////

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
	float mu = tTmr[(uint2)In.Pos].z;
	float r = tTmr[(uint2)In.Pos].w;

	//clip(-T);

	float3 sunCol = sunColor(x, t, v, s, r, mu); //L0
	Out.MainCol = float4(sunCol, 1.0);

	Out.sv_depth = 0.0;

	return Out;
}
