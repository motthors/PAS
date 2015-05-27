
//////////////////////////////////////////////////////////////
//! コンスタントバッファ

cbuffer m_2DconstBuffer : register(b0)
{
	float4 Width_Shift	: packoffset(c0);
	float Alpha			: packoffset(c1);
	float XperY			: packoffset(c1.y);
};

//////////////////////////////////////////////////////////////
// ps
struct PS_IN
{
	float4 Pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float2 TexelPos	: TEXCOORD1; //テクセルのパース空間座標
};

struct PS_OUT
{
	float4 MainCol	: SV_Target;
};


PS_OUT main(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	//			ボケ度　 線細さ　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　半径
	float len1 = 600 * (0.003 - abs(length((In.TexelPos - Width_Shift.zw)*float2(1, XperY)) - Width_Shift.x));
	float len2 = 600 * (0.003 - abs(length((In.TexelPos - Width_Shift.zw)*float2(1, XperY)) - (Width_Shift.x - 0.075)));
	float col = clamp(len1, 0.1, 1.0) + clamp(len2, 0.1, 1.0);
	float alf = 800 * (0.04 - abs(length((In.TexelPos - Width_Shift.zw)*float2(1, XperY)) - (Width_Shift.x - 0.037)));
	alf = clamp(alf, 0.0, 0.7);
	Out.MainCol = float4(col, col, col, alf*Alpha);
	//Out.MainCol = float4(1 * Width_Shift.z, 1 * Width_Shift.w, 1, 0.6);
	return Out;
}