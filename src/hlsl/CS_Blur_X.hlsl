#include"CS_Blur.hlsl"



[numthreads(1, 1, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	uint x = did.x;
	uint y = did.y;
	rwTemp[int2(x, y)] = Blur_X(uint2(x,y));
}
