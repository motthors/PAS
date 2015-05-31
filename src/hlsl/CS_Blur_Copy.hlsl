

Texture2D  TexSrc  : register(t0);
SamplerState Sampler  : register(s0);

RWTexture2D<float4> rwTexDest : register(u0);


[numthreads(16, 9, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	float x = did.x + 0.5;
	float y = did.y + 0.5;
	uint u;
	uint v;
	rwTexDest.GetDimensions(u, v);
	rwTexDest[did.xy] = TexSrc.SampleLevel(Sampler, float2(x / u, y / v), 0);
}
