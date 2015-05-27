#include"DefineStruct.h"
#include"VS_Normal.h"
#include"PS_Normal.h"

technique Main
{
	pass p0
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_AR();

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
		ZEnable = TRUE;			//D3DRS_ZENABLE				Z比較実行
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE		深度バッファへ書き込み
		CullMode = NONE;		//D3DRS_CULLMODE			背面カリング

		AlphaBlendEnable = TRUE;//D3DRS_ALPHABLENDENABLE	アルファブレンド実行
		AlphaTestEnable = TRUE; //D3DRS_ALPHATESTENABLE		アルファテスト実行
		AlphaFunc = GREATEREQUAL;//D3DRS_ALPHAFUNC			D3DCMP_～ アルファテスト実行時関数　新しいピクセル値が現在のピクセル値以上のとき
		AlphaRef = 1;			//D3DRS_ALPHAREF			アルファテスト可時の基準値

		SrcBlend = SRCALPHA;	//D3DRS_SRCBLEND			半透明処理実行
		DestBlend = INVSRCALPHA;//D3DRS_DESTBLEND			透過処理実行

	}
}
