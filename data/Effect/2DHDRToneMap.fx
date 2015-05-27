
float g_Texel0;
float g_Texel1;
float g_Texel2;
float g_Texel3;
float g_Texel4;
float g_Texel5;
float g_Texel6;
float g_Texel7;

float Weight0;    // �d��
float Weight1;    // �d��
float Weight2;    // �d��
float Weight3;    // �d��
float Weight4;    // �d��
float Weight5;    // �d��
float Weight6;    // �d��
float Weight7;    // �d��

#include"DefineTexture.h"
#include"VS_Non.h"
#include"PS_DinamicToneMap.h"
#include"PS_2DBlur.h"
#include"PS_2DAdd.h"

technique Main
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS_Non();
		PixelShader = compile ps_2_0 PS_DinamicToneMap();
	}
	pass p1
	{
		VertexShader = compile vs_2_0 VS_Non();
		PixelShader = compile ps_3_0 PS_Blur_X();
	}
	pass p2
	{
		VertexShader = compile vs_2_0 VS_Non();
		PixelShader = compile ps_3_0 PS_Blur_Y();
	}
	pass p3
	{
		VertexShader = compile vs_2_0 VS_Non();
		PixelShader = compile ps_3_0 PS_2DAdd();
	}
}