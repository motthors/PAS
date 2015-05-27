#ifndef _DEFUNE_RENDERING_H_
#define _DEFUNE_RENDERING_H_

#include<Windows.h>

#ifdef _DEFRENDER_INSTANCE_
#define _DEFRENDER_GLOBAL_  
#else
#define _DEFRENDER_GLOBAL_ extern
#endif

class DefineRendering{
public:
	int WindowX;
	int WindowY;
	int RenderTargetX;
	int RenderTargetY;
	int RenderTargetNum;

	DefineRendering();
	void ConvertToTexelBaseCoord(float* xOut, float* yOut, LONG x, LONG y);
	void ConvertToTexelBaseCoord_Relative(float* xOut, float* yOut, LONG x, LONG y);
};

_DEFRENDER_GLOBAL_ DefineRendering DefRender;

#endif