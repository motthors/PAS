
#include"DefineTexture.h"

#include"VS_Non.h"
#include"PS_2DPostEffect.h"

technique Main
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS_Non();
		PixelShader = compile ps_2_0 PS_2DPostEffect();
	}
}
