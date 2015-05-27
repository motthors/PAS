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

float4 Blur_Y(uint2 texel)
{
	float4 Out = 0;

	Out += (rwTemp[texel + int2(0, 1)] + rwTemp[texel - int2(0, 1)]) * Weight0;
	Out += (rwTemp[texel + int2(0, 3)] + rwTemp[texel - int2(0, 3)]) * Weight1;
	Out += (rwTemp[texel + int2(0, 5)] + rwTemp[texel - int2(0, 5)]) * Weight2;
	Out += (rwTemp[texel + int2(0, 7)] + rwTemp[texel - int2(0, 7)]) * Weight3;
	Out += (rwTemp[texel + int2(0, 9)] + rwTemp[texel - int2(0, 9)]) * Weight4;
	Out += (rwTemp[texel + int2(0, 11)]+ rwTemp[texel - int2(0, 11)]) * Weight5;
	Out += (rwTemp[texel + int2(0, 13)]+ rwTemp[texel - int2(0, 13)]) * Weight6;
	Out += (rwTemp[texel + int2(0, 15)]+ rwTemp[texel - int2(0, 15)]) * Weight7;

	return Out;
}


[numthreads(1, 1, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	uint x = did.x;
	uint y = did.y;
	rwTex[int2(x, y)] = Blur_Y(uint2(x, y));
}
