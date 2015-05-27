
#include"PAS_Common.hlsl"

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
	float4 tTmur	: SV_TARGET0;
	float4  Depth	: SV_TARGET1;
	float4 xyzt		: SV_TARGET2;
	float sv_depth	: SV_DEPTH;
};

PS_OUT main(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float3 x = cam.xyz;
	float3 v = normalize(In.ray);
	//float3 s = normalize(SunDir.xyz);

	float r = length(x);
	float mu = dot(x, v) / r;
	float t = ComputeD_hlslFix(r, mu, Rg);
	float T = ComputeD_hlslFix(r, mu, Rt);
	float t2 = t; //T>0で更新しないt

	// 深度バッファ用Depth計算
	if (t >= 0)
		Out.Depth.r = t*-In.dir;
	else
		Out.Depth.r = 1e40;


	if (T > 0.0) { // if x in space and ray intersects atmosphere
		// move x to nearest intersection of ray with top atmosphere boundary
		x += T * v;
		t -= T;
		mu = (r * mu + T) / Rt;	// mu -> mu = dot(ray, N of ground)(only when ray hit ground) [-1.0, 0.0]
		r = Rt;
	}

	// PAS計算に必要な値の保存
	Out.tTmur = float4(t, t2, mu, r);
	Out.xyzt = float4(x, t2);


	// 深度テスト用Depth計算
	if (t >= 0)
	{
		Out.sv_depth = Out.Depth.r / 10000000.0; //temp
		//Out.sv_depth =_Blank; //temp
		//Out.tTmur = float4(0.5, 0, 0, 1);
	}
	else
	{
		Out.sv_depth = 0.999;
		//Out.tTmur = float4(0, 0.5, 0, 1);
	}
	return Out;
}
