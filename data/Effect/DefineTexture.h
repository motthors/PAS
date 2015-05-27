/////////////////////////////////////
//IDirect3DDevice9::SetTexture()から

//sampler etex0 : register(s0);
//sampler etex1 : register(s1);

//////////////////////////////////////
//ID3DXEffect::SetTexture()から

texture t0;
texture t1;

sampler etex0 : register(s0) = sampler_state
 {
	texture=<t0>;
	//テクスチャフィルタ　SetSamplerState()
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	//テクスチャのアドレッシング SetSamplerState() D3DTADDRESS_～
	AddressU = Wrap;
	AddressV = Wrap;
};
sampler etex1 : register(s1) = sampler_state
 {
	texture=<t1>;
	//テクスチャフィルタ　SetSamplerState()
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	//テクスチャのアドレッシング SetSamplerState() D3DTADDRESS_～
	AddressU = Wrap;
	AddressV = Wrap;
};

