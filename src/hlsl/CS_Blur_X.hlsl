//#include"CS_Blur_h.hlsl"

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
RWTexture2D<float4> rwTex : register(u0);


float4 Blur(uint2 texel)
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

[numthreads(1, 1, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	uint x = did.x;
	uint y = did.y;
	rwTex[uint2(x, y)] = Blur(uint2(x, y));
}
