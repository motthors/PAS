#include"PAS_Common.hlsl"


//////////////////////////////////////////////////////////////
// cs

//[numthreads(256, 1, 1)]
//void main(uint3 did : SV_DispatchThreadID)
//{
//	// �p�[�e�B�N���̒ʂ��ԍ������߂�
//	float3 seed = (float)did;
//	float3 rand;
//	rand.x = frac(sin(dot(float2(did.xy), float2(12.9898, 78.233)) + seed) * 43758.5453);
//	rand.y = frac(sin(dot(float2(did.xy), float2(12.9898, 78.233)) + rand.x) * 43758.5453);
//	rand.z = frac(sin(dot(float2(did.xy), float2(12.9898, 78.233)) + rand.y) * 43758.5453);
//
//	// �s�N�Z���F�i�[
//	rwOutputTex[uint2(did.x, did.y)] = float4(rand, 1.0);
//}

/////////////////////////////////////////////////////////////////

float opticalDepth(float H, float r, float mu)
{
	float result = 0.0;
	float dx = limit(r, mu) / float(TRANSMITTANCE_INTEGRAL_SAMPLES);
	float xi = 0.0;
	float yi = exp(-(r - Rg) / H);
	for (int i = 1; i <= TRANSMITTANCE_INTEGRAL_SAMPLES; ++i) {
		float xj = float(i) * dx;
		float yj = exp(-(sqrt(r * r + xj * xj + 2.0f * xj * r * mu) - Rg) / H);  //memo�Fsqrt()�̎����]���藝
		result += (yi + yj) / 2.0f * dx;
		xi = xj;
		yi = yj;
	}
	return mu - 0.0001f < -sqrt(1.0f - (Rg / r) * (Rg / r)) ? 1e9f : result;
	//mu<-sqrt(�`)�@�^�Ȃ��v�͒n�ʂ������Ă���@�U�Ȃ������Ă��遁�v�Z���ʂ��g����
}

//transmittance1
[numthreads(TRANSMITTANCE_W, TRANSMITTANCE_H/16, 1)]
void main(uint3 did : SV_DispatchThreadID)
{
	int x = did.x;
	int y = did.y;
	float r, muS;
	getTransmittanceRMu(x, y, r, muS);
	float3 depth = betaR * opticalDepth(HR, r, muS) + betaMEx * opticalDepth(HM, r, muS);
	rwOutputTex[int2(x, y)] = float4(exp(-depth), 1.0);
}
