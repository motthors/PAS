
//�ʏ�̒��_�ϊ�

struct VS_IN
{
	float4 Pos		: POSITION;   	//���_�̍��W
   	float4 Normal	: NORMAL;	//�@���x�N�g��
    float2 Tex		: TEXCOORD0; 	//�e�N�X�`�����W
	//float4 Tangent	: TANGENT0;
    //float4 Binormal	: BINORMAL0;
};

struct VS_OUT
{
   	float4 Pos		: POSITION;     //���_�̍��W
	float3 Col		: COLOR0;       //���_�J���[
   	float2 Tex		: TEXCOORD0;    //�e�N�Z�����W
   	float3 Normal	: TEXCOORD1;   //�I�u�W�F�N�g�̖@���x�N�g��
   	float3 Look		: TEXCOORD2;   //���_ -> ���_ �x�N�g��
};


//�o�[�e�b�N�X�V�F�[�_�[
VS_OUT VS_main( VS_IN IN )
{
	VS_OUT Out = (VS_OUT)0;
	
	//���_�̍��W���s��ϊ�����
	Out.Pos = mul( IN.Pos, matWVP );
	Out.Tex = IN.Tex; //�e�N�Z���͂��̂܂�

	Out.Normal = normalize( mul(IN.Normal,matWorldIT).xyz );
	//Out.Normal = normalize( IN.Normal.xyz );

	float4 WorldPos = mul( IN.Pos, matWorld );
	Out.Look	= normalize(vecCameraPos.xyz - WorldPos.xyz); //���_ -> ���_ �ւ̃x�N�g�����v�Z
    
	//���_�J���[�������o�[�g��
	float3 Ln = normalize(-LightDir.xyz);
    float3 Nn = normalize(Out.Normal);
   	float ldn = dot(Ln,Nn);
	ldn = ldn*0.5 + 0.4;
	ldn += Emission;
	Out.Col = ldn;
	return Out;
}