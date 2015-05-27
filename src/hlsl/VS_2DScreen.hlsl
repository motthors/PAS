
/////////////////////////////////////////////////////
// スクリーンいっぱいに四角ポリゴンを設置するだけ


struct VS_IN
{
	float4 Pos		: POSITION;
	float2 uv		: TEXCOORD0;
};

struct VS_OUT
{
	float4 Pos		: SV_Position;
	float2 uv		: TEXCOORD0;
};

VS_OUT main(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	Out.Pos = In.Pos;
	Out.uv = In.uv;
	return Out;
}
