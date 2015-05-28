
cbuffer cbBuffer0 : register(b0)
{
	float  Weight0 : packoffset(c0.x);
	float  Weight1 : packoffset(c0.y);
	float  Weight2 : packoffset(c0.z);
	float  Weight3 : packoffset(c0.w);
	float  Weight4 : packoffset(c1.x);
	float  Weight5 : packoffset(c1.y);
	float  Weight6 : packoffset(c1.z);
	float  Weight7 : packoffset(c1.w);
};

Texture2D  Tex0  : register(t0);
// Å´  Load Tex0,  Write rwTex
RWTexture2D<uint4> rwTex : register(u0);
RWTexture2D<uint4> rwTemp : register(u0);


float4 Blur_X(uint2 texel)
{
	float4 Out = 0;

	Out += (Tex0[texel + uint2(1, 0)] + Tex0[texel - uint2(1, 0)]) * Weight0;
	Out += (Tex0[texel + uint2(2, 0)] + Tex0[texel - uint2(2, 0)]) * Weight1;
	Out += (Tex0[texel + uint2(3, 0)] + Tex0[texel - uint2(3, 0)]) * Weight2;
	Out += (Tex0[texel + uint2(4, 0)] + Tex0[texel - uint2(4, 0)]) * Weight3;
	Out += (Tex0[texel + uint2(5, 0)] + Tex0[texel - uint2(5, 0)]) * Weight4;
	Out += (Tex0[texel + uint2(6, 0)] + Tex0[texel - uint2(6, 0)]) * Weight5;
	Out += (Tex0[texel + uint2(7, 0)] + Tex0[texel - uint2(7, 0)]) * Weight6;
	Out += (Tex0[texel + uint2(8, 0)] + Tex0[texel - uint2(8, 0)]) * Weight7;

	return Out;
}

float4 Blur_Y(uint2 texel)
{
	float4 Out = 0;

	Out += (rwTemp[texel + uint2(0, 1)] + rwTemp[texel - uint2(0, 1)]) * Weight0;
	Out += (rwTemp[texel + uint2(0, 2)] + rwTemp[texel - uint2(0, 2)]) * Weight1;
	Out += (rwTemp[texel + uint2(0, 3)] + rwTemp[texel - uint2(0, 3)]) * Weight2;
	Out += (rwTemp[texel + uint2(0, 4)] + rwTemp[texel - uint2(0, 4)]) * Weight3;
	Out += (rwTemp[texel + uint2(0, 5)] + rwTemp[texel - uint2(0, 5)]) * Weight4;
	Out += (rwTemp[texel + uint2(0, 6)] + rwTemp[texel - uint2(0, 6)]) * Weight5;
	Out += (rwTemp[texel + uint2(0, 7)] + rwTemp[texel - uint2(0, 7)]) * Weight6;
	Out += (rwTemp[texel + uint2(0, 8)] + rwTemp[texel - uint2(0, 8)]) * Weight7;

	return Out;
}
