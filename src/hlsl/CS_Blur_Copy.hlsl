
cbuffer cbBuffer0 : register(b0)
{
	// トーンマップ変数
	float  ccenter : packoffset(c0.x);
	float  cwidth : packoffset(c0.y);
};

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
	float4 hdr;
	//rwTexDest[did.xy] = TexSrc.SampleLevel(Sampler, float2(x / u, y / v), 0) - 1;
	//hdr = TexSrc.SampleLevel(Sampler, float2(x / u, y / v), 0)*0.2
	//	+ TexSrc.SampleLevel(Sampler, float2((x+1) / u, y / v), 0)*0.2
	//	+ TexSrc.SampleLevel(Sampler, float2((x-1) / u, y / v), 0)*0.2
	//	+ TexSrc.SampleLevel(Sampler, float2(x / u, (y+1) / v), 0)*0.2
	//	+ TexSrc.SampleLevel(Sampler, float2(x / u, (y-1) / v), 0)*0.2;

	hdr = TexSrc.SampleLevel(Sampler, float2(x / u, y / v), 0);

	float center = 40;
	float width = 30;
	hdr.r = 1/(1+exp(-4/width*(hdr.r-center)));
	hdr.g = 1/(1+exp(-4/width*(hdr.g-center)));
	hdr.b = 1/(1+exp(-4/width*(hdr.b-center)));
	hdr.a = 1;
	rwTexDest[did.xy] = hdr;
}
