//#include"PAS_H.h"

/////////////////////////////////////////////////////////////
// context buffer

cbuffer cbSceneParam : register(b0)
{
	// レイ変換用マトリクス
	//matrix	matVIxPI	: packoffset(c0);
	matrix	matVI	: packoffset(c0);
	matrix	matPI	: packoffset(c4);
	//float4	cam		: packoffset(c8);
};	

cbuffer cbPlanetParam : register(b1)
{
	float3	cam		: packoffset(c0);
	float __Blank : packoffset(c0.w);
};

//////////////////////////////////////////////////////////////
// vs
struct VS_IN
{
	float3 Pos		: POSITION;
	float2 uv		: TEXCOORD0;
};

struct VS_OUT
{
	float4 Pos		: SV_Position;
	float  dirMu	: TEXCOORD0;
	float3 ray		: TEXCOORD1;
};

VS_OUT main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos.xy = In.Pos.xy;
	Out.Pos.z = __Blank;
	Out.Pos.w = 1.0;
	//Out.uv = In.uv;

	//Out.ray = mul(Out.Pos, matVIxPI).xyz;
	Out.ray = mul(float4(mul(Out.Pos, matPI).xyz, 0.0), matVI);

	float3 raydir = mul(float4(mul(float4(0, 0, 0.1, 1), matPI).xyz, 0.0), matVI);
	raydir = normalize(raydir);
	Out.dirMu = dot(cam.xyz, raydir) / length(cam.xyz);
	return Out;
}
