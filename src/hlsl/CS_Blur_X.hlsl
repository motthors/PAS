#include"CS_Blur.hlsl"

cbuffer cbBuffer0 : register(b0)
{
	float  g_Weight0 : packoffset(c0.x);  
	float  g_Weight1 : packoffset(c0.y);  
	float  g_Weight2 : packoffset(c0.z);  
	float  g_Weight3 : packoffset(c0.w);  
	float  g_Weight4 : packoffset(c1.x);  
	float  g_Weight5 : packoffset(c1.y);  
	float  g_Weight6 : packoffset(c1.z);  
	float  g_Weight7 : packoffset(c1.w); 
};

RWTexture2D<float4> rwTex : register(u0);
RWTexture2D<float4> rwTemp : register(u1);
/*
     rwTex
BlurXÅ´Å@Å™BlurY
     rwTemp 
*/

float4 Blur_X(uint2 texel)
{
	float4 Out = 0;

	Out += (rwTex[texel + int2(1, 0)] + rwTex[texel - int2(1, 0)]) * Weight0;
	Out += (rwTex[texel + int2(3, 0)] + rwTex[texel - int2(3, 0)]) * Weight1;
	Out += (rwTex[texel + int2(5, 0)] + rwTex[texel - int2(5, 0)]) * Weight2;
	Out += (rwTex[texel + int2(7, 0)] + rwTex[texel - int2(7, 0)]) * Weight3;
	Out += (rwTex[texel + int2(9, 0)] + rwTex[texel - int2(9, 0)]) * Weight4;
	Out += (rwTex[texel + int2(11, 0)] + rwTex[texel - int2(11, 0)]) * Weight5;
	Out += (rwTex[texel + int2(13, 0)] + rwTex[texel - int2(13, 0)]) * Weight6;
	Out += (rwTex[texel + int2(15, 0)] + rwTex[texel - int2(15, 0)]) * Weight7;

	return Out;
}



[numthreads(1, 1, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	uint x = did.x;
	uint y = did.y;
	rwTemp[int2(x, y)] = Blur_X(uint2(x,y));
}
