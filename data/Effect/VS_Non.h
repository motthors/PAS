////////////////////////////////////////////////////
//頂点変換をまったくしない
//2Dポストエフェクト用ポリゴン描画用
///////////////////////////////////////////////////

struct VS_IN
{
	float4 Pos	: POSITION;
	float2 Tex	: TEXCOORD0;
};

struct VS_OUT
{
	float4 Pos	: POSITION;
	float2 Tex	: TEXCOORD0;
};

VS_OUT VS_Non( VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	Out.Pos = In.Pos;
	Out.Tex = In.Tex;
	return Out;
}