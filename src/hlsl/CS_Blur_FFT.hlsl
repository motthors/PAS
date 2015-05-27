#include"PAS_Common.hlsl"

cbuffer cbBuffer0 : register(b0)
{
	float  g_Weight0 : packoffset(c0.x);  
};

Texture2D  SrcTex  : register(t0);
SamplerState Sampler  : register(s0);

RWTexture2D<float4> rwTex : register(u0);
RWTexture2D<float4> rwTemp : register(u1);
/*
 SrcTex
downÅ´Å@        Åø Effect
 rwBuff   ÅÃ   rwFFT 
 Å´UpAdd  FFT
 Render
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


//irradiance1
[numthreads(1, 1, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	uint x = did.x;
	uint y = did.y;
	rwTex[int2(x, y)] = Blur_Y(uint2(x, y));
}
