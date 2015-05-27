#define _DEFRENDER_INSTANCE_

#include"DefineRendering.h"


DefineRendering::DefineRendering()
{
	WindowX			= 1280;//1280;	16
	WindowY			= 720;//720;	9
	RenderTargetX	= 1920;//1920;
	RenderTargetY	= 1080;//1080;
	RenderTargetNum	= 2;
}

void DefineRendering::ConvertToTexelBaseCoord(float* xOut, float* yOut, LONG x, LONG y)
{
	*xOut = (float)(2 * x) / (float)DefRender.WindowX - 1.0f;
	*yOut = (float)(-2 * y) / (float)DefRender.WindowY + 1.0f;
}

void DefineRendering::ConvertToTexelBaseCoord_Relative(float* xOut, float* yOut, LONG x, LONG y)
{
	*xOut = (float)(2 * x) / (float)DefRender.WindowX;
	*yOut = (float)(-2 * y) / (float)DefRender.WindowY;
}