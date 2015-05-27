/////////////////////////////////////
//IDirect3DDevice9::SetTexture()����

//sampler etex0 : register(s0);
//sampler etex1 : register(s1);

//////////////////////////////////////
//ID3DXEffect::SetTexture()����

texture t0;
texture t1;

sampler etex0 : register(s0) = sampler_state
 {
	texture=<t0>;
	//�e�N�X�`���t�B���^�@SetSamplerState()
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	//�e�N�X�`���̃A�h���b�V���O SetSamplerState() D3DTADDRESS_�`
	AddressU = Wrap;
	AddressV = Wrap;
};
sampler etex1 : register(s1) = sampler_state
 {
	texture=<t1>;
	//�e�N�X�`���t�B���^�@SetSamplerState()
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	//�e�N�X�`���̃A�h���b�V���O SetSamplerState() D3DTADDRESS_�`
	AddressU = Wrap;
	AddressV = Wrap;
};

