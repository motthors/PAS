/////////////////////////////////////////////////////////////
// context buffer

cbuffer cbSceneParam : register(b0)
{
	// レイ変換用マトリクス
	float3	SunDir	: packoffset(c0);
	//float4	camP	: packoffset(c0);
};

cbuffer cbPlanetParam : register(b1)
{
	// レイ変換用マトリクス
	float3	cam		: packoffset(c0);

	// 散乱カラー
	float3 Color1 : packoffset(c1);
	float3 Color2 : packoffset(c2);
	float3 Color3 : packoffset(c3);

	//// レイリー散乱係数
	float	HR		: packoffset(c4.x);
	float3	betaR	: packoffset(c4.y);

	//// ミー散乱係数
	float	HM		: packoffset(c5.x);
	float3	betaMSca: packoffset(c5.y);
	float	mieG	: packoffset(c6.x);
	float3	betaMEx	: packoffset(c6.y);

	//// 半径データ　
	float Rg : packoffset(c7.x); // 地表までの半径
	float Rt : packoffset(c7.y); // 大気の層の半径
};

//////////////////////////////////////////////////////////////
// static const
//テスクチャサイズ
static const int TRANSMITTANCE_W = 256; //大気の透過度 transmittance
static const int TRANSMITTANCE_H = 64;
static const int SKY_W = 64; // 大気の放射照度(反射率？) irradiance
static const int SKY_H = 16;
static const int RES_R = 32; // 大気の散乱度合いテクスチャ
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
// NUMERICAL INTEGRATION PARAMETERS　積分用パラメータ
static const int TRANSMITTANCE_INTEGRAL_SAMPLES = 500;
static const int INSCATTER_INTEGRAL_SAMPLES = 50;
static const int IRRADIANCE_INTEGRAL_SAMPLES = 32;
static const int INSCATTER_SPHERICAL_INTEGRAL_SAMPLES = 16;
 
static const float M_PI = 3.141592657;

// ----------------------------------------------------------------------------
// PARAMETERIZATION OPTIONS　パラメータ化オプション
#define TRANSMITTANCE_NON_LINEAR
#define INSCATTER_NON_LINEAR

// ----------------------------------------------------------------------------
// PARAMETERIZATION FUNCTIONS　パラメータ化関数
// ----------------------------------------------------------------------------

// 透過率UVを取得 r:惑星の中心とカメラの距離？(km)　mu:μ(視点ベクトルの角度？)
float2 getTransmittanceUV(float r, float mu) {
	float uR, uMu;
	uR = sqrt((r - Rg) / (Rt - Rg));
	uMu = atan((mu + 0.15) / (1.0 + 0.15) * tan(1.5)) / 1.5;
	return float2(uMu, uR);
}

// 注目中のテクスチャ座標を使うべき高度とmuSを求める？
void getTransmittanceRMu(int texelx, int texely, out float r, out float muS) {
	// gl_FragCoordをテクスチャ座標に変換　r=v muS=u
	r = texely / float(TRANSMITTANCE_H);  //gl_FragCoord:ウィンドウ上のフラグメント位置 描画対象のWindow座標？
	muS = texelx / float(TRANSMITTANCE_W);

	r = Rg + (r * r) * (Rt - Rg);	// 現在計算中のテクセルからその高度を計算
	muS = -0.15 + tan(1.5 * muS) / tan(1.5) * (1.0 + 0.15);
}

//放射照度のUVを取得
void getIrradianceUV(float r, float muS, out float uMuS, out float uR) {
	uR = (r - Rg) / (Rt - Rg);
	uMuS = (muS + 0.2) / (1.0 + 0.2);
}

void getIrradianceRMuS(int texelx, int texely, out float r, out float muS) {
	r = Rg + (texely + 0.5) / (float(SKY_H) - 1.0) * (Rt - Rg);
	muS = -0.2 + (texelx + 0.5) / (float(SKY_W) - 1.0) * (1.0 + 0.2);
}

//各パラメタから適切なテクスチャ座標に変換
//r:カメラまでの半径距離　mu:光の入ってくるベクトル　muS:太陽の方向ベクトル　nu: 太陽の角度 ｖ・ｓ(p4_reft Parameterization line5)
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
	muS = fmod(x, float(RES_MU_S)) / (float(RES_MU_S) - 1.0);  // glslのmodはhlslのfmodにしていいかわからん
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
// 光入ベクトルの最近交差点計算　大気圏境界か地面かの判定
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
// 無限レイ（R、MU）時の大気の透過率（=透明度）計算
// (mu=cos(view zenith angle)), intersections with ground ignored 地面との交点は無視
float3 transmittance(float r, float mu) {
	float2 uv = getTransmittanceUV(r, mu);
	return Tex1.SampleLevel(Sampler, uv, 0).rgb;
}

// transmittance(=transparency) of atmosphere for infinite ray (r,mu)
// (mu=cos(view zenith angle)), or zero if ray intersects ground またはレイが地面に交差するときゼロ
float3 transmittanceWithShadow(float r, float mu) {
	return mu < -sqrt(1.0 - (Rg / r) * (Rg / r)) ? float3(0, 0, 0) : transmittance(r, mu); // true:地面向き　false:空向き
}

// transmittance(=transparency) of atmosphere between x and x0
// x～x0間の大気の透過率（=透明度）
// assume segment x,x0 not intersecting ground　セグメントx,x0を想定し、地面と交差しない
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
// 解析式を用いた長さdのレイ(r,mu)のための光学的深度
// (mu=cos(view zenith angle)), intersections with ground ignored 地面との交点は無視
// H=height scale of exponential density function 指数関数的な密度関数の高さスケール
float opticalDepth(float H, float r, float mu, float d) {
	return 1.0;
	float a = sqrt((0.5 / H)*r);
	float2 a01 = a * float2(mu, mu + d / r);
	float2 a01s = sign(a01);
	float2 a01sq = a01 * a01;
	float x = a01s.y > a01s.x ? exp(a01sq.x) : 0.0;
	float2 y = a01s / (2.3193*abs(a01) + sqrt(1.52*a01sq + 4.0)) * float2(1.0, exp(-d / H*(d / (2.0*r) + mu)));
	return sqrt((6.2831*H)*r) * exp(/**/(Rg - r)/*/-60*/ / H) * (x + dot(y, float2(1.0, -1.0)));
	//			 ↑2*PI?
}

// transmittance(=transparency) of atmosphere for ray (r,mu) of length d
// 長さdのレイ(r,mu)による大気の透明度
// (mu=cos(view zenith angle)), intersections with ground ignored 地面との交点は無視
// uses analytic formula instead of transmittance texture　透過率のテクスチャの代わりに解析式を使用
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
// 近似シングルミー散乱（参照："Angular precision"段落内のおおよそのCm）
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
