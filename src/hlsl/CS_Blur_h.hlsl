#include"PAS_Common.hlsl"

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
RWTexture2D<float4> rwBlur1 : register(u1);
RWTexture2D<float4> rwBlur2 : register(u2);
RWTexture2D<float4> rwBlur3 : register(u3);


float4 Blur_X(uint2 texel)
{
	float4 Out = 0;

	Out += (rwTex[texel + int2(1, 0)] + rwTex[texel - int2(1, 0)]) * Weight0;
	Out += (rwTex[texel + int2(2, 0)] + rwTex[texel - int2(2, 0)]) * Weight1;
	Out += (rwTex[texel + int2(3, 0)] + rwTex[texel - int2(3, 0)]) * Weight2;
	Out += (rwTex[texel + int2(4, 0)] + rwTex[texel - int2(4, 0)]) * Weight3;
	Out += (rwTex[texel + int2(5, 0)] + rwTex[texel - int2(5, 0)]) * Weight4;
	Out += (rwTex[texel + int2(6, 0)] + rwTex[texel - int2(6, 0)]) * Weight5;
	Out += (rwTex[texel + int2(7, 0)] + rwTex[texel - int2(7, 0)]) * Weight6;
	Out += (rwTex[texel + int2(8, 0)] + rwTex[texel - int2(8, 0)]) * Weight7;

	return Out;
}

float4 Blur_Y(uint2 texel)
{
	float4 Out = 0;

	Out += (rwTemp[texel + int2(0, 1)] + rwTemp[texel - int2(0, 1)]) * Weight0;
	Out += (rwTemp[texel + int2(0, 2)] + rwTemp[texel - int2(0, 2)]) * Weight1;
	Out += (rwTemp[texel + int2(0, 3)] + rwTemp[texel - int2(0, 3)]) * Weight2;
	Out += (rwTemp[texel + int2(0, 4)] + rwTemp[texel - int2(0, 4)]) * Weight3;
	Out += (rwTemp[texel + int2(0, 5)] + rwTemp[texel - int2(0, 5)]) * Weight4;
	Out += (rwTemp[texel + int2(0, 6)] + rwTemp[texel - int2(0, 6)]) * Weight5;
	Out += (rwTemp[texel + int2(0, 7)] + rwTemp[texel - int2(0, 7)]) * Weight6;
	Out += (rwTemp[texel + int2(0, 8)] + rwTemp[texel - int2(0, 8)]) * Weight7;

	return Out;
}
