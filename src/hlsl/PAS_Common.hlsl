/////////////////////////////////////////////////////////////
// context buffer

cbuffer cbSceneParam : register(b0)
{
	// ���C�ϊ��p�}�g���N�X
	float3	SunDir	: packoffset(c0);
	//float4	camP	: packoffset(c0);
};

cbuffer cbPlanetParam : register(b1)
{
	// ���C�ϊ��p�}�g���N�X
	float3	cam		: packoffset(c0);

	// �U���J���[
	float3 Color1 : packoffset(c1);
	float3 Color2 : packoffset(c2);
	float3 Color3 : packoffset(c3);

	//// ���C���[�U���W��
	float	HR		: packoffset(c4.x);
	float3	betaR	: packoffset(c4.y);

	//// �~�[�U���W��
	float	HM		: packoffset(c5.x);
	float3	betaMSca: packoffset(c5.y);
	float	mieG	: packoffset(c6.x);
	float3	betaMEx	: packoffset(c6.y);

	//// ���a�f�[�^�@
	float Rg : packoffset(c7.x); // �n�\�܂ł̔��a
	float Rt : packoffset(c7.y); // ��C�̑w�̔��a
};

//////////////////////////////////////////////////////////////
// static const
//�e�X�N�`���T�C�Y
static const int TRANSMITTANCE_W = 256; //��C�̓��ߓx transmittance
static const int TRANSMITTANCE_H = 64;
static const int SKY_W = 64; // ��C�̕��ˏƓx(���˗��H) irradiance
static const int SKY_H = 16;
static const int RES_R = 32; // ��C�̎U���x�����e�N�X�`��
static const int RES_MU = 128;
static const int RES_MU_S = 32;
static const int RES_NU = 8;

//////////////////////////////////////////////////////////////
// texture
Texture2D  Tex0  : register(t0);
Texture2D  Tex1  : register(t1);
Texture2D  Tex2  : register(t2);
Texture3D  Tex3  : register(t3);
Texture3D  Tex4  : register(t4);

RWTexture2D<float4>  rwOutputTex : register(u0);
RWTexture2D<float4>  rwOutputTex1 : register(u1);
RWTexture3D<float4>  rwOutputTex2 : register(u2);
RWTexture3D<float4>  rwOutputTex3 : register(u3);

SamplerState Sampler  : register(s0);
SamplerState Sampler_Wrap  : register(s1);

// ----------------------------------------------------------------------------
// PHYSICAL MODEL PARAMETERS
static const float AVERAGE_GROUND_REFLECTANCE = 0.1;


// ----------------------------------------------------------------------------
// NUMERICAL INTEGRATION PARAMETERS�@�ϕ��p�p�����[�^
static const int TRANSMITTANCE_INTEGRAL_SAMPLES = 500;
static const int INSCATTER_INTEGRAL_SAMPLES = 50;
static const int IRRADIANCE_INTEGRAL_SAMPLES = 32;
static const int INSCATTER_SPHERICAL_INTEGRAL_SAMPLES = 16;
 
static const float M_PI = 3.141592657;

// ----------------------------------------------------------------------------
// PARAMETERIZATION OPTIONS�@�p�����[�^���I�v�V����
#define TRANSMITTANCE_NON_LINEAR
#define INSCATTER_NON_LINEAR

// ----------------------------------------------------------------------------
// PARAMETERIZATION FUNCTIONS�@�p�����[�^���֐�
// ----------------------------------------------------------------------------

// ���ߗ�UV���擾 r:�f���̒��S�ƃJ�����̋����H(km)�@mu:��(���_�x�N�g���̊p�x�H)
float2 getTransmittanceUV(float r, float mu) {
	float uR, uMu;
	uR = sqrt((r - Rg) / (Rt - Rg));
	uMu = atan((mu + 0.15) / (1.0 + 0.15) * tan(1.5)) / 1.5;
	return float2(uMu, uR);
}

// ���ڒ��̃e�N�X�`�����W���g���ׂ����x��muS�����߂�H
void getTransmittanceRMu(int texelx, int texely, out float r, out float muS) {
	// gl_FragCoord���e�N�X�`�����W�ɕϊ��@r=v muS=u
	r = texely / float(TRANSMITTANCE_H);  //gl_FragCoord:�E�B���h�E��̃t���O�����g�ʒu �`��Ώۂ�Window���W�H
	muS = texelx / float(TRANSMITTANCE_W);

	r = Rg + (r * r) * (Rt - Rg);	// ���݌v�Z���̃e�N�Z�����炻�̍��x���v�Z
	muS = -0.15 + tan(1.5 * muS) / tan(1.5) * (1.0 + 0.15);
}

//���ˏƓx��UV���擾
void getIrradianceUV(float r, float muS, out float uMuS, out float uR) {
	uR = (r - Rg) / (Rt - Rg);
	uMuS = (muS + 0.2) / (1.0 + 0.2);
}

void getIrradianceRMuS(int texelx, int texely, out float r, out float muS) {
	r = Rg + (texely + 0.5) / (float(SKY_H) - 1.0) * (Rt - Rg);
	muS = -0.2 + (texelx + 0.5) / (float(SKY_W) - 1.0) * (1.0 + 0.2);
}

//�e�p�����^����K�؂ȃe�N�X�`�����W�ɕϊ�
//r:�J�����܂ł̔��a�����@mu:���̓����Ă���x�N�g���@muS:���z�̕����x�N�g���@nu: ���z�̊p�x ���E��(p4_reft Parameterization line5)
float4 texture4D(Texture3D Tex, float r, float mu, float muS, float nu) {
	float H = sqrt(Rt * Rt - Rg * Rg);
	float rho = sqrt(r * r - Rg * Rg);

	float rmu = r * mu;
	float delta = rmu * rmu - r * r + Rg * Rg;
	float4 cst = rmu < 0.0 && delta > 0.0 ? float4(1.0, 0.0, 0.0, 0.5 - 0.5 / float(RES_MU)) : float4(-1.0, H * H, H, 0.5 + 0.5 / float(RES_MU));
		float uR = 0.5 / float(RES_R) + rho / H * (1.0 - 1.0 / float(RES_R));
	float uMu = cst.w + (rmu * cst.x + sqrt(delta + cst.y)) / (rho + cst.z) * (0.5 - 1.0 / float(RES_MU));
	float uMuS = 0.5 / float(RES_MU_S) + (atan(max(muS, -0.1975) * tan(1.26 * 1.1)) / 1.1 + (1.0 - 0.26)) * 0.5 * (1.0 - 1.0 / float(RES_MU_S));

	float lerp = (nu + 1.0) / 2.0 * (float(RES_NU) - 1.0);
	float uNu = floor(lerp);
	lerp = lerp - uNu;
	return Tex.SampleLevel(Sampler, float3((uNu + uMuS) / float(RES_NU), uMu, uR),0) * (1.0 - lerp) +
		Tex.SampleLevel(Sampler, float3((uNu + uMuS + 1.0) / float(RES_NU), uMu, uR),0) * lerp;
}


void getMuMuSNu(float r, float4 dhdH, float2 Texel, out float mu, out float muS, out float nu) {
	float x = Texel.x + 0.5;
	float y = Texel.y + 0.5;

	if (y < float(RES_MU) / 2.0) {
		float d = 1.0 - y / (float(RES_MU) / 2.0 - 1.0);
		d = min(max(dhdH.z, d * dhdH.w), dhdH.w * 0.999);
		mu = (Rg * Rg - r * r - d * d) / (2.0 * r * d);
		mu = min(mu, -sqrt(1.0 - (Rg / r) * (Rg / r)) - 0.001);
	}
	else {
		float d = (y - float(RES_MU) / 2.0) / (float(RES_MU) / 2.0 - 1.0);
		d = min(max(dhdH.x, d * dhdH.y), dhdH.y * 0.999);
		mu = (Rt * Rt - r * r - d * d) / (2.0 * r * d);
	}
	muS = fmod(x, float(RES_MU_S)) / (float(RES_MU_S) - 1.0);  // glsl��mod��hlsl��fmod�ɂ��Ă������킩���
	// paper formula
	//muS = -(0.6 + log(1.0 - muS * (1.0 -  exp(-3.6)))) / 3.0;
	// better formula
	muS = tan((2.0 * muS - 1.0 + 0.26) * 1.1) / tan(1.26 * 1.1);
	nu = -1.0 + floor(x / float(RES_MU_S)) / (float(RES_NU) - 1.0) * 2.0;
}


// ----------------------------------------------------------------------------
// UTILITY FUNCTIONS
// ----------------------------------------------------------------------------

// nearest intersection of ray r,mu with ground or top atmosphere boundary
// �����x�N�g���̍ŋߌ����_�v�Z�@��C�����E���n�ʂ��̔���
// mu=cos(ray zenith angle at ray origin)
float limit(float r, float mu) {
	float dout = -r * mu + sqrt(r * r * (mu * mu - 1.0) + Rt * Rt);
	float delta2 = r * r * (mu * mu - 1.0) + Rg * Rg;
	if (delta2 >= 0.0) {
		float din = -r * mu - sqrt(delta2);
		if (din >= 0.0) {
			dout = min(dout, din);
		}
	}
	return dout;
}

// transmittance(=transparency) of atmosphere for infinite ray (r,mu)
// �������C�iR�AMU�j���̑�C�̓��ߗ��i=�����x�j�v�Z
// (mu=cos(view zenith angle)), intersections with ground ignored �n�ʂƂ̌�_�͖���
float3 transmittance(float r, float mu) {
	float2 uv = getTransmittanceUV(r, mu);
	return Tex1.SampleLevel(Sampler, uv, 0).rgb;
}

// transmittance(=transparency) of atmosphere for infinite ray (r,mu)
// (mu=cos(view zenith angle)), or zero if ray intersects ground �܂��̓��C���n�ʂɌ�������Ƃ��[��
float3 transmittanceWithShadow(float r, float mu) {
	return mu < -sqrt(1.0 - (Rg / r) * (Rg / r)) ? float3(0, 0, 0) : transmittance(r, mu); // true:�n�ʌ����@false:�����
}

// transmittance(=transparency) of atmosphere between x and x0
// x�`x0�Ԃ̑�C�̓��ߗ��i=�����x�j
// assume segment x,x0 not intersecting ground�@�Z�O�����gx,x0��z�肵�A�n�ʂƌ������Ȃ�
// r=||x||, mu=cos(zenith angle of [x,x0) ray at x), v=unit direction vector of [x,x0) ray
float3 transmittance(float r, float mu, float3 v, float3 x0) {
	float3 result;
	float r1 = length(x0);
	float mu1 = dot(x0, v) / r;
	if (mu > 0.0) {
		result = min(transmittance(r, mu) / transmittance(r1, mu1), 1.0);
	}
	else {
		result = min(transmittance(r1, -mu1) / transmittance(r, -mu), 1.0);
	}
	return result;
}

// optical depth for ray (r,mu) of length d, using analytic formula
// ��͎���p��������d�̃��C(r,mu)�̂��߂̌��w�I�[�x
// (mu=cos(view zenith angle)), intersections with ground ignored �n�ʂƂ̌�_�͖���
// H=height scale of exponential density function �w���֐��I�Ȗ��x�֐��̍����X�P�[��
float opticalDepth(float H, float r, float mu, float d) {
	return 1.0;
	float a = sqrt((0.5 / H)*r);
	float2 a01 = a * float2(mu, mu + d / r);
	float2 a01s = sign(a01);
	float2 a01sq = a01 * a01;
	float x = a01s.y > a01s.x ? exp(a01sq.x) : 0.0;
	float2 y = a01s / (2.3193*abs(a01) + sqrt(1.52*a01sq + 4.0)) * float2(1.0, exp(-d / H*(d / (2.0*r) + mu)));
	return sqrt((6.2831*H)*r) * exp(/**/(Rg - r)/*/-60*/ / H) * (x + dot(y, float2(1.0, -1.0)));
	//			 ��2*PI?
}

// transmittance(=transparency) of atmosphere for ray (r,mu) of length d
// ����d�̃��C(r,mu)�ɂ���C�̓����x
// (mu=cos(view zenith angle)), intersections with ground ignored �n�ʂƂ̌�_�͖���
// uses analytic formula instead of transmittance texture�@���ߗ��̃e�N�X�`���̑���ɉ�͎����g�p
float3 analyticTransmittance(float r, float mu, float d) {
	return exp(-betaR * opticalDepth(HR, r, mu, d) - betaMEx * opticalDepth(HM, r, mu, d));
}

// transmittance(=transparency) of atmosphere between x and x0
// assume segment x,x0 not intersecting ground
// d = distance between x and x0, mu=cos(zenith angle of [x,x0) ray at x)
float3 transmittance(float r, float mu, float d) {
	float3 result;
	float r1 = sqrt(r * r + d * d + 2.0 * r * mu * d);
	float mu1 = (r * mu + d) / r1;
	if (mu > 0.0) {
		result = min(transmittance(r, mu) / transmittance(r1, mu1), 1.0);
	}
	else {
		result = min(transmittance(r1, -mu1) / transmittance(r, -mu), 1.0);
	}
	return result;
}


float3 irradiance(Texture2D Tex, float r, float muS) {
	float2 uv;
	getIrradianceUV(r, muS, uv.x, uv.y);
	return Tex.SampleLevel(Sampler, uv,0).rgb;
}

// Rayleigh phase function p2(2)formula
float phaseFunctionR(float mu) {
	return (3.0 / (16.0 * M_PI)) * (1.0 + mu * mu);
}

// Mie phase function  p2(4)formula
float phaseFunctionM(float mu) {
	return 1.5 * 1.0 / (4.0 * M_PI) * (1.0 - mieG*mieG) * pow(1.0 + (mieG*mieG) - 2.0*mieG*mu, -3.0 / 2.0) * (1.0 + mu * mu) / (2.0 + mieG*mieG);
}

// approximated single Mie scattering (cf. approximate Cm in paragraph "Angular precision")
// �ߎ��V���O���~�[�U���i�Q�ƁF"Angular precision"�i�����̂����悻��Cm�j
float3 getMie(float4 rayMie) { // rayMie.rgb=C*, rayMie.w=Cm,r
	return rayMie.rgb * rayMie.w / max(rayMie.r, 1e-4) * (betaR.r / betaR);
}


void SetValue(out float r, out float4 dhdH, int z)
{
	r = z / (RES_R - 1.0);
	r = r * r;
	r = sqrt(Rg * Rg + r * (Rt *  Rt - Rg *  Rg)) + (z == 0 ? 0.01 : (z == RES_R - 1 ? -0.001 : 0.0));
	float dmin = Rt - r;
	float dmax = sqrt(r * r - Rg *  Rg) + sqrt(Rt *  Rt - Rg *  Rg);
	float dminp = r - Rg;
	float dmaxp = sqrt(r * r - Rg * Rg);
	dhdH.x = dmin;
	dhdH.y = dmax;
	dhdH.z = dminp;
	dhdH.w = dmaxp;
}
