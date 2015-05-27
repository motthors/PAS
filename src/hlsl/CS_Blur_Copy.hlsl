

Texture2D  TexSrc  : register(t0);
SamplerState Sampler  : register(s0);

RWTexture2D<float4> rwTexDest : register(u0);


[numthreads(16, 9, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	uint x = did.x;
	uint y = did.y;
	rwTexDest[int2(x, y)] = TexSrc.SampleLevel(Sampler,uint2(x, y),1);
}
