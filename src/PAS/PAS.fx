
//////////////////////////////////////////////////////////////
// 外部から設定する変数
//////////////////////////////////////////////////////////////

//float4x4 matWorld	: World;
//float4x4 matWVP		: WorldViewProjection;

float4x4 matViewI;
float4x4 matProjI;
float4 cam;	//vector to camera from center of earth
float4 s = float4(1.0, 1.0, 1.0, 1.0);

// 半径データ　
float Rg = 6360.0; // 地表までの半径
float Rt = 6420.0; // 大気の層の半径
float RL = 6421.0;

// レイリー散乱係数
float HR = 8.0;
float3 betaR = float3(5.8e-3, 1.35e-2, 3.31e-2);

// ミー散乱係数
float HM = 1.2;
float3 betaMSca = float3(4e-3, 4e-3, 4e-3);
float3 betaMEx = float3(4e-3, 4e-3, 4e-3) / 0.9;
float mieG = 0.8;

float4 vecCameraPos;

int tex4d_w;
float coord = 0.0;
//////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////
//テスクチャサイズ
static const int TRANSMITTANCE_W = 256; //大気の透過度 transmittance
static const int TRANSMITTANCE_H = 64;
static const int SKY_W = 64; // 大気の放射照度(反射率？) irradiance
static const int SKY_H = 16;
static const int RES_R = 32; // 大気の散乱度合いテクスチャ
static const int RES_MU = 128;
static const int RES_MU_S = 32;
static const int RES_NU = 8;

static const float M_PI = 3.1415926535;
 
static const float ISun = 100.0;
float exposure = 0.4;

//////////////////////////////////////////////////////////////
// texture
//////////////////////////////////////////////////////////////
texture refT;
texture traT;
texture irrT;
texture insT;

sampler2D reflectanceSampler : register(s0) = sampler_state{
	texture=<refT>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};
sampler2D transmittanceSampler : register(s1) = sampler_state{
	texture = <traT>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};
sampler2D irradianceSampler : register(s2) = sampler_state{
	texture=<irrT>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};
sampler3D inscatterSampler : register(s3) = sampler_state{
	texture=<insT>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

//////////////////////////////////////////////////////////////
// function
//////////////////////////////////////////////////////////////
// if return>0 , ray hit ground;  return = length from camera to ground
float ComputeD_hlslFix(float r, float mu, float R){
	// original fomula
	//float t = -r * mu - sqrt(r * r * (mu * mu - 1.0) + R * R);
	float temp = r * r * (mu * mu - 1.0) + R * R;
	temp = (temp >= 0) ? sqrt(temp) : 1e100;
	return -r * mu - temp;
}

float2 getTransmittanceUV(float r, float mu) {
	float uR, uMu;
	uR = sqrt((r - Rg) / (Rt - Rg));
	uMu = atan((mu + 0.15) / (1.0 + 0.15) * tan(1.5)) / 1.5;
	return float2(uMu, uR);
}

float2 getIrradianceUV(float r, float muS) {
	float uR = (r - Rg) / (Rt - Rg);
	float uMuS = (muS + 0.2) / (1.0 + 0.2);
	return float2(uMuS, uR);
}

float3 transmittance(float r, float mu) {
	float2 uv = getTransmittanceUV(r, mu);
	return tex2D(transmittanceSampler, uv).rgb;
}

float3 transmittanceWithShadow(float r, float mu) {
	return mu < -sqrt(1.0 - (Rg / r) * (Rg / r)) ? float3(0,0,0) : transmittance(r, mu); // true:地面向き　false:空向き
}

float4 texture4D(sampler3D table, float r, float mu, float muS, float nu) {
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
	return tex3D(table, float3((uNu + uMuS) / float(RES_NU), uMu, uR)) * (1.0 - lerp) +
		tex3D(table, float3((uNu + uMuS + 1.0) / float(RES_NU), uMu, uR)) * lerp;
}

float opticalDepth(float H, float r, float mu, float d) {
	float a = sqrt((0.5 / H)*r);
	float2 a01 = a * float2(mu, mu + d / r);
	float2 a01s = sign(a01);
	float2 a01sq = a01 * a01;
	float x = a01s.y > a01s.x ? exp(a01sq.x) : 0.0;
	float2 y = a01s / (2.3193*abs(a01) + sqrt(1.52*a01sq + 4.0)) * float2(1.0, exp(-d / H*(d / (2.0*r) + mu)));
	return sqrt((6.2831*H)*r) * exp((Rg - r) / H) * (x + dot(y, float2(1.0, -1.0)));
}

float3 analyticTransmittance(float r, float mu, float d) {
	return exp(-betaR * opticalDepth(HR, r, mu, d) - betaMEx * opticalDepth(HM, r, mu, d));
}

float3 irradiance(sampler2D samp, float r, float muS) {
	float2 uv = getIrradianceUV(r, muS);
	return tex2D(samp, uv).rgb;
}

// Rayleigh phase function p2(2)formula
float phaseFunctionR(float mu) {
	return (3.0 / (16.0 * M_PI)) * (1.0 + mu * mu);
}

// Mie phase function  p2(4)formula
float phaseFunctionM(float mu) {
	return 1.5 * 1.0 / (4.0 * M_PI) * (1.0 - mieG*mieG) * pow(1.0 + (mieG*mieG) - 2.0*mieG*mu, -3.0 / 2.0) * (1.0 + mu * mu) / (2.0 + mieG*mieG);
}

float3 getMie(float4 rayMie) { // rayMie.rgb=C*, rayMie.w=Cm,r
	return rayMie.rgb * rayMie.w / max(rayMie.r, 1e-4) * (betaR.r / betaR);
}

//inscattered light along ray x+tv, when sun in direction s (=S[L]-T(x,x0)S[L]|x0)
float3 inscatter(inout float3 x, inout float t, float3 v, float3 s, inout float r, inout float mu, out float3 attenuation) {
	attenuation = float3(0, 0, 0); // temp
	float3 result;
	//r = length(x);
	mu = dot(x, v) / r;
	//float d = -r * mu - sqrt(r * r * (mu * mu - 1.0) + Rt * Rt);
	float d = ComputeD_hlslFix(r, mu, Rt);

	if (d > 0.0) { // if x in space and ray intersects atmosphere
		// move x to nearest intersection of ray with top atmosphere boundary
		x += d * v;
		t -= d;
		mu = (r * mu + d) / Rt;
		r = Rt;
	}
	if (r <= Rt) { // if ray intersects atmosphere
		float nu = dot(v, s);
		float muS = dot(x, s) / r;
		float phaseR = phaseFunctionR(nu);
		float phaseM = phaseFunctionM(nu);
		float4 inscatter = max(texture4D(inscatterSampler, r, mu, muS, nu), 0.0);

		if (t > 0.0) {
			float3 x0 = x + t * v; //Coord of the ground in the direction of ray
			float r0 = length(x0);
			float rMu0 = dot(x0, v);
			float mu0 = rMu0 / r0;
			float muS0 = dot(x0, s) / r0;

			// avoids imprecision problems in transmittance computations based on textures
			attenuation = analyticTransmittance(r, mu, t);

			if (r0 > Rg + 0.01) { //山とか？
				// computes S[L]-T(x,x0)S[L]|x0
				inscatter = max(inscatter - attenuation.rgbr * texture4D(inscatterSampler, r0, mu0, muS0, nu), 0.0);
// FIX
				// avoids imprecision problems near horizon by interpolating between two points above and below horizon
				const float EPS = 0.004;
				float muHoriz = -sqrt(1.0 - (Rg / r) * (Rg / r));
				if (abs(mu - muHoriz) < EPS) {
					float a = ((mu - muHoriz) + EPS) / (2.0 * EPS);

					mu = muHoriz - EPS;
					r0 = sqrt(r * r + t * t + 2.0 * r * t * mu);
					mu0 = (r * mu + t) / r0;
					float4 inScatter0 = texture4D(inscatterSampler, r, mu, muS, nu);
					float4 inScatter1 = texture4D(inscatterSampler, r0, mu0, muS0, nu);
					float4 inScatterA = max(inScatter0 - attenuation.rgbr * inScatter1, 0.0);

					mu = muHoriz + EPS;
					r0 = sqrt(r * r + t * t + 2.0 * r * t * mu);
					mu0 = (r * mu + t) / r0;
					inScatter0 = texture4D(inscatterSampler, r, mu, muS, nu);
					inScatter1 = texture4D(inscatterSampler, r0, mu0, muS0, nu);
					float4 inScatterB = max(inScatter0 - attenuation.rgbr * inScatter1, 0.0);

					inscatter = lerp(inScatterA, inScatterB, a);
				}
// end FIX
			}
		}
// FIX
		// avoids imprecision problems in Mie scattering when sun is below horizon
		inscatter.w *= smoothstep(0.00, 0.02, muS);
// end FIX
		result = max(inscatter.rgb * phaseR + getMie(inscatter) * phaseM, 0.0);
	}
	else { // x in space and ray looking in space
		result = float3(0,0,0);
	}
	return result * ISun;
}


//ground radiance at end of ray x+tv, when sun in direction s
//attenuated bewteen ground and viewer (=R[L0]+R[L*])
float3 groundColor(float3 x, float t, float3 v, float3 s, float r, float mu, float3 attenuation)
{
	float3 result;
	if (t > 0.0) { // if ray hits ground surface
		// ground reflectance at end of ray, x0
		float3 x0 = x + t * v;
		float r0 = length(x0);
		float3 n = x0 / r0;

		float2 coords = float2(atan(-n.x/n.z), acos(n.y)) * float2(0.5, 1.0) / M_PI + float2(coord, 0.0);
		float4 reflectance = tex2D(reflectanceSampler, coords) * float4(0.2, 0.2, 0.2, 1.0);
		if (r0 > Rg + 0.01) {
			reflectance = float4(0.4, 0.4, 0.4, 0.0);
		}

		// direct sun light (radiance) reaching x0
		float muS = dot(n, s);
		float3 sunLight = transmittanceWithShadow(r0, muS);

		// precomputed sky light (irradiance) (=E[L*]) at x0
		float3 groundSkyLight = irradiance(irradianceSampler, r0, muS);

		// light reflected at x0 (=(R[L0]+R[L*])/T(x,x0))
		float3 groundColor = reflectance.rgb * (max(muS, 0.0) * sunLight + groundSkyLight) * ISun / M_PI;

		// water specular color due to sunLight
		if (reflectance.w > 0.0) {
			float3 h = normalize(s - v);
			float fresnel = 0.02 + 0.98 * pow(1.0 - dot(-v, h), 5.0);
			float waterBrdf = fresnel * pow(max(dot(h, n), 0.0), 150.0);
			groundColor += reflectance.w * max(waterBrdf, 0.0) * sunLight * ISun;
		}

		result = attenuation * groundColor; //=R[L0]+R[L*]
	}
	else { // ray looking at the sky
		result = float3(0,0,0);
	}
	return result;
}

// direct sun light for ray x+tv, when sun in direction s (=L0)
float3 sunColor(float3 x, float t, float3 v, float3 s, float r, float mu) {
	if (t > 0.0) {
		return float3(0, 0, 0);
	}
	else {
		float3 transmittance = (r <= Rt ? transmittanceWithShadow(r, mu) : float3(1.0, 1.0, 1.0)); // T(x,xo)
		float isun = step(cos(M_PI / 180.0), dot(v, s)) * ISun; // Lsun
		return transmittance * isun; // Eq (9)
	}
}

float3 HDR(float3 L) {
	L = L * exposure;
	L.r = L.r < 1.413 ? pow(L.r * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.r);
	L.g = L.g < 1.413 ? pow(L.g * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.g);
	L.b = L.b < 1.413 ? pow(L.b * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.b);
	return L;
}

//////////////////////////////////////////////////////////////
// vs1
//////////////////////////////////////////////////////////////
struct VS_IN
{
	float4 Pos		: POSITION;
	float2 uv		: TEXCOORD0;
};

struct VS_OUT
{
	float4 Pos		: POSITION;
	float3 ray		: TEXCOORD0;
	float2 uv		: TEXCOORD1;
};

VS_OUT VS_main( VS_IN In )
{
	VS_OUT Out = (VS_OUT)0;

	//Out.Pos = mul(In.Pos, matWVP);
	//Out.uv = In.uv; 
	//Out.ray = mul(In.Pos, matWorld).xyz - cam.xyz;
	//

	Out.Pos.xy = In.Pos.xy; 
	Out.Pos.z = 0.1;
	Out.Pos.w = 1.0;
	Out.uv = In.uv; 

	float4 vec4Ray = float4(mul(In.Pos, matProjI).xyz, 1.0);
	Out.ray = mul(vec4Ray, matViewI).xyz - cam.xyz;
	//Out.ray = mul(float4(In.uv * 2 - 1.0, 0.9, 1.0), matProjI).xyz;
	//Out.ray = float3(In.uv * 2 - 1.0, 0.9);
	return Out;
}


//////////////////////////////////////////////////////////////
// ps
//////////////////////////////////////////////////////////////
struct PS_IN
{
	float3 ray	: TEXCOORD0;
	float2 uv	: TEXCOORD1;
};

struct PS_OUT
{
	float4 MainCol	: COLOR0;
};


PS_OUT PS_main( PS_IN In )
{ 
	PS_OUT Out = (PS_OUT)0;

	float3 x = cam.xyz;
	float3 v = normalize(In.ray);


	float r = length(x);
	float mu = dot(x, v) / r;
	//float t = -r * mu - sqrt(r * r * (mu * mu - 1.0) + Rg * Rg); 
	float t = ComputeD_hlslFix(r, mu, Rg);

	float3 g = x - float3(0.0, 0.0, Rg + 10.0);
	float a = v.x * v.x + v.y * v.y - v.z * v.z;
	float b = 2.0 * (g.x * v.x + g.y * v.y - g.z * v.z);
	float c = g.x * g.x + g.y * g.y - g.z * g.z;
	float d = -(b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
	bool cone = d > 0.0 && abs(x.z + d * v.z - Rg) <= 10.0;

	if (t > 0.0) {
		if (cone && d < t) {
			t = d;
		}
	}
	else if (cone) {
		t = d;
	}

	float3 attenuation;
	float3 inscatterCol = inscatter(x, t, v, s.xyz, r, mu, attenuation); //S[L]-T(x,xs)S[l]|xs
	float3 groundCol = groundColor(x, t, v, s.xyz, r, mu, attenuation); //R[L0]+R[L*]
	float3 sunCol = sunColor(x, t, v, s.xyz, r, mu); //L0
	Out.MainCol = float4(HDR(sunCol + groundCol + inscatterCol), 0.5); // Eq (16)
	//Out.MainCol = float4(inscatterCol, 0.5);
	//float test = (r > Rg*2) ? 1.0 : 0;
	//float test2 = sqrt(mu*mu*2 - 1.0);
	//Out.MainCol = float4(t, 0, ComputeD_hlslFix(r, mu, Rt), 0.5);
	return Out;
}


PS_OUT PS_transmittance(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.MainCol.xyz = HDR(tex2D(transmittanceSampler, In.uv).xyz);
	Out.MainCol.a = 1.0;
	return Out;
}

PS_OUT PS_irradiance(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.MainCol = float4(tex2D(irradianceSampler, In.uv).rgb*5.0, 1.0);
	Out.MainCol.a = 1.0;
	return Out;
}

PS_OUT PS_inscatter(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.MainCol = tex3D(inscatterSampler, float3(In.uv, (tex4d_w+0.5)/RES_MU_S/*(s.x + 1.0) / 2.0)*/));
	Out.MainCol.a = 1.0;
	return Out;
}

PS_OUT PS_test(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	//Out.MainCol = float4(In.uv, 0, 1.0);
	Out.MainCol = float4(normalize(In.ray)*1000, 0.4);
	return Out;
}

PS_OUT PS_no(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	Out.MainCol.a = 0;
	return Out;
}
//////////////////////////////////////////////////////////////
//  technique
//////////////////////////////////////////////////////////////


technique main
{
	pass p0
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_main();

		// レンダリングステート設定

		//RGBカラー要素設定 SetTextureStageState() D3DTSS
		ColorOp[0] = SELECTARG1;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp[0] = SELECTARG1;
		AlphaArg1[0] = TEXTURE;
		AlphaArg2[0] = DIFFUSE;
		ColorOp[1] = DISABLE;
		AlphaOp[1] = DISABLE;

		//SetRenderState()
		ZEnable = FALSE;		//D3DRS_ZENABLE		Z比較実行
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE	深度バッファへ書き込み
		CullMode = NONE;	//D3DRS_CULLMODE	背面カリング

		AlphaBlendEnable = TRUE;//D3DRS_ALPHABLENDENABLE	アルファブレンド実行
		AlphaTestEnable = TRUE; //D3DRS_ALPHATESTENABLE		アルファテスト実行
		AlphaFunc = GREATEREQUAL;//D3DRS_ALPHAFUNC	D3DCMP_～ アルファテスト実行時関数　新しいピクセル値が現在のピクセル値以上のとき
		AlphaRef = 1;		//D3DRS_ALPHAREF	アルファテスト可時の基準値

		SrcBlend = SRCALPHA;	//D3DRS_SRCBLEND			半透明処理実行
		DestBlend = INVSRCALPHA;//D3DRS_DESTBLEND			透過処理実行

	}

	pass p2
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_test();
		//SetRenderState()
		ZEnable = FALSE;		//D3DRS_ZENABLE		Z比較実行
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE	深度バッファへ書き込み
		CullMode = NONE;	//D3DRS_CULLMODE	背面カリング

		AlphaBlendEnable = TRUE;//D3DRS_ALPHABLENDENABLE	アルファブレンド実行
		AlphaTestEnable = TRUE; //D3DRS_ALPHATESTENABLE		アルファテスト実行
		AlphaFunc = GREATEREQUAL;//D3DRS_ALPHAFUNC	D3DCMP_～ アルファテスト実行時関数　新しいピクセル値が現在のピクセル値以上のとき
		AlphaRef = 1;		//D3DRS_ALPHAREF	アルファテスト可時の基準値

		SrcBlend = SRCALPHA;	//D3DRS_SRCBLEND			半透明処理実行
		DestBlend = INVSRCALPHA;//D3DRS_DESTBLEND			透過処理実行
	}
	pass p3
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_transmittance();
		//SetRenderState()
		ZEnable = FALSE;		//D3DRS_ZENABLE		Z比較実行
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE	深度バッファへ書き込み
		CullMode = NONE;	//D3DRS_CULLMODE	背面カリング
	}
	pass p4
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_irradiance();
		//SetRenderState()
		ZEnable = FALSE;		//D3DRS_ZENABLE		Z比較実行
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE	深度バッファへ書き込み
		CullMode = NONE;	//D3DRS_CULLMODE	背面カリング
	}
	pass p5
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_inscatter();
		//SetRenderState()
		ZEnable = FALSE;		//D3DRS_ZENABLE		Z比較実行
		ZWriteEnable = FALSE;	//D3DRS_ZWRITEENABLE	深度バッファへ書き込み
		CullMode = NONE;	//D3DRS_CULLMODE	背面カリング
	}
	pass p6
	{
		//VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 PS_no();
	}
}
