#include"CS_Blur_h.hlsl"


[numthreads(1, 1, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	uint x = did.x;
	uint y = did.y;
	rwTex[int2(x, y)] = 0;// Blur_Y(uint2(x, y));
}
