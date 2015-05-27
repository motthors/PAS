
//////////////////////////////////////////////////////////////
// 外部から設定する変数

cbuffer cbMeshParam : register(b0)
{
	float4 Width_Shift	: packoffset(c0);
	float FakeDepth : packoffset(c1);
};

//////////////////////////////////////////////////////////////
// vs1
struct VS_IN
{
	float4 Pos		: POSITION;
	float2 uv		: TEXCOORD0;
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float2 Pos2		: TEXCOORD1; //テクセルのパース空間座標
};

VS_OUT main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos.xy = In.Pos.xy * Width_Shift.xy*1.05 + Width_Shift.zw;
	Out.Pos.z = 1 - FakeDepth;
	Out.Pos.w = 1.0;
	Out.uv = In.uv;
	Out.Pos2 = Out.Pos.xy;
	return Out;
}
