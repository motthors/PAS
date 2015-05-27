
//////////////////////////////////////////////////////////////
// 外部から設定する変数
//////////////////////////////////////////////////////////////

float4 Width_Shift = 0;
float2 CenterPos = 0;
float Alpha;

//////////////////////////////////////////////////////////////
// texture
//////////////////////////////////////////////////////////////
texture Tex;

sampler2D Sampler : register(s0) = sampler_state{
	texture = <Tex>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//////////////////////////////////////////////////////////////
// vs1
//////////////////////////////////////////////////////////////
struct VS_IN
{
	float4 Pos		: POSITION;
	float2 uv		: TEXCOORD0;
};

struct VS_OUT
{
	float4 Pos		: POSITION;
	float2 uv		: TEXCOORD0;
	float2 Pos2		: TEXCOORD1; //テクセルのパース空間座標
};

VS_OUT VS_main( VS_IN In )
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos.xy = In.Pos.xy * Width_Shift.xy*1.05 + Width_Shift.zw;
	Out.Pos.z = In.Pos.z;
	Out.Pos.w = 1.0;
	Out.uv = In.uv; 
	Out.Pos2 = Out.Pos.xy;
	return Out;
}


//////////////////////////////////////////////////////////////
// ps
//////////////////////////////////////////////////////////////
struct PS_IN
{
	float2 uv	: TEXCOORD0;
	float2 Pos	: TEXCOORD1; //テクセルのパース空間座標
};

struct PS_OUT
{
	float4 MainCol	: COLOR0;
};


PS_OUT PS_RefTex(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.MainCol = tex2D(Sampler, In.uv);
	Out.MainCol.a = Out.MainCol.a * Alpha;
	return Out;
}

PS_OUT PS_Circle(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	float len =600*(0.006 - abs(length((In.Pos - CenterPos)*float2(1,Width_Shift.x / Width_Shift.y)) - Width_Shift.x*0.9));
	float col = clamp(len, 0.2, 1.0);
	float alf = 600 * (-length((In.Pos - CenterPos)*float2(1, Width_Shift.x / Width_Shift.y)) + Width_Shift.x*0.9);
	alf = clamp(alf, 0.0, 0.7);
	Out.MainCol = float4(col, col, col, alf*Alpha);
	//Out.MainCol = float4(1 * Width_Shift.z, 1 * Width_Shift.w, 1, 0.6);
	return Out;
}

PS_OUT PS_Donut(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	//			ボケ度　 線細さ　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　半径
	float len1 = 600 * (0.003 - abs(length((In.Pos - CenterPos)*float2(1, Width_Shift.x / Width_Shift.y)) - Width_Shift.x));
	float len2 = 600 * (0.003 - abs(length((In.Pos - CenterPos)*float2(1, Width_Shift.x / Width_Shift.y)) - (Width_Shift.x - 0.08)));
	float col = clamp(len1, 0.1, 1.0) + clamp(len2, 0.1, 1.0);
	float alf =  800 * (0.046 - abs(length((In.Pos - CenterPos)*float2(1, Width_Shift.x / Width_Shift.y)) - (Width_Shift.x*0.98 - 0.035)));
	alf = clamp(alf, 0.0, 0.7);
	Out.MainCol = float4(col, col, col, alf*Alpha);
	//Out.MainCol = float4(1 * Width_Shift.z, 1 * Width_Shift.w, 1, 0.6);
	return Out;
}
//////////////////////////////////////////////////////////////
//  technique
//////////////////////////////////////////////////////////////


technique main
{
	pass p0
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_RefTex();

		// レンダリングステート設定

		//RGBカラー要素設定 SetTextureStageState() D3DTSS
		ColorOp[0] = SELECTARG1;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp[0] = SELECTARG1;
		AlphaArg1[0] = TEXTURE;
		AlphaArg2[0] = DIFFUSE;
		ColorOp[1] = DISABLE;
		AlphaOp[1] = DISABLE;

		//SetRenderState()
		ZEnable = FALSE;		//D3DRS_ZENABLE		Z比較実行
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE	深度バッファへ書き込み
		CullMode = NONE;	//D3DRS_CULLMODE	背面カリング

		AlphaBlendEnable = TRUE;//D3DRS_ALPHABLENDENABLE	アルファブレンド実行
		AlphaTestEnable = TRUE; //D3DRS_ALPHATESTENABLE		アルファテスト実行
		AlphaFunc = GREATEREQUAL;//D3DRS_ALPHAFUNC	D3DCMP_～ アルファテスト実行時関数　新しいピクセル値が現在のピクセル値以上のとき
		AlphaRef = 1;		//D3DRS_ALPHAREF	アルファテスト可時の基準値

		SrcBlend = SRCALPHA;	//D3DRS_SRCBLEND			半透明処理実行
		DestBlend = INVSRCALPHA;//D3DRS_DESTBLEND			透過処理実行

	}

	pass p1
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_Circle();

		//RGBカラー要素設定 SetTextureStageState() D3DTSS
		ColorOp[0] = SELECTARG1;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp[0] = SELECTARG1;
		AlphaArg1[0] = TEXTURE;
		AlphaArg2[0] = DIFFUSE;
		ColorOp[1] = DISABLE;
		AlphaOp[1] = DISABLE;
		//SetRenderState()
		ZEnable = FALSE;		//D3DRS_ZENABLE		Z比較実行
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE	深度バッファへ書き込み
		CullMode = NONE;	//D3DRS_CULLMODE	背面カリング

		AlphaBlendEnable = TRUE;//D3DRS_ALPHABLENDENABLE	アルファブレンド実行
		AlphaTestEnable = TRUE; //D3DRS_ALPHATESTENABLE		アルファテスト実行
		AlphaFunc = GREATEREQUAL;//D3DRS_ALPHAFUNC	D3DCMP_～ アルファテスト実行時関数　新しいピクセル値が現在のピクセル値以上のとき
		AlphaRef = 1;		//D3DRS_ALPHAREF	アルファテスト可時の基準値

		SrcBlend = SRCALPHA;	//D3DRS_SRCBLEND			半透明処理実行
		DestBlend = INVSRCALPHA;//D3DRS_DESTBLEND			透過処理実行
	}
	

	pass p2
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_Donut();

		//RGBカラー要素設定 SetTextureStageState() D3DTSS
		ColorOp[0] = SELECTARG1;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp[0] = SELECTARG1;
		AlphaArg1[0] = TEXTURE;
		AlphaArg2[0] = DIFFUSE;
		ColorOp[1] = DISABLE;
		AlphaOp[1] = DISABLE;
		//SetRenderState()
		ZEnable = FALSE;		//D3DRS_ZENABLE		Z比較実行
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE	深度バッファへ書き込み
		CullMode = NONE;	//D3DRS_CULLMODE	背面カリング

		AlphaBlendEnable = TRUE;//D3DRS_ALPHABLENDENABLE	アルファブレンド実行
		AlphaTestEnable = TRUE; //D3DRS_ALPHATESTENABLE		アルファテスト実行
		AlphaFunc = GREATEREQUAL;//D3DRS_ALPHAFUNC	D3DCMP_～ アルファテスト実行時関数　新しいピクセル値が現在のピクセル値以上のとき
		AlphaRef = 1;		//D3DRS_ALPHAREF	アルファテスト可時の基準値

		SrcBlend = SRCALPHA;	//D3DRS_SRCBLEND			半透明処理実行
		DestBlend = INVSRCALPHA;//D3DRS_DESTBLEND			透過処理実行
	}
}
