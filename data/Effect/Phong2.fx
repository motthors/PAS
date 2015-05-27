// -------------------------------------------------------------
// �O���[�o���ϐ�
// -------------------------------------------------------------

float4x4 matWVP;

float4 LightDir; // ���C�g�̕���
float4 vColor = {1.0f,1.0f,1.0f,1.0f};  // ���C�g�����b�V���̐F
float4 vecCameraPos;  // �J�����̈ʒu�i���[�J�����W�n�j

// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos   : POSITION;
    float4 Color  : COLOR0;
    float3 N   : TEXCOORD0;
    float3 Eye   : TEXCOORD1;
float2 Tex : TEXCOORD2;
};
//�e�N�X�`��
texture Tex;
sampler tex0 = sampler_state
{
 Texture =<Tex>;
 MinFilter =LINEAR;
 MagFilter =LINEAR;
 MipFilter =NONE;

 AddressU =Clamp;
 AddressV =Clamp;
};
// -------------------------------------------------------------
// �V�[���̕`��
// -------------------------------------------------------------
VS_OUTPUT VS(
      float4 Pos    : POSITION,          // ���[�J���ʒu���W
      float4 Normal : NORMAL,            // �@���x�N�g��
float2 Tex : TEXCOORD
){
 VS_OUTPUT Out = (VS_OUTPUT)0;        // �o�̓f�[�^
 
 // ���W�ϊ�
 Out.Pos = mul(Pos, matWVP);
 
 // �g�U���{����
 float amb = -LightDir.w; // �����̋���
 float4 L = LightDir; // ���[�J�����W�n�ł̃��C�g�x�N�g��
 Out.Color = vColor * max(amb, dot(Normal, L));
 
 // ���ʔ��˗p�̃x�N�g��
 Out.N   = Normal.xyz;
 Out.Eye = vecCameraPos - Pos.xyz;
Out.Tex = Tex;//�e�N�X�`�����W
 
 return Out;
}
// -------------------------------------------------------------
float4 PS(VS_OUTPUT In) : COLOR
{  
 float3 L = LightDir.xyz;
 float3 H = normalize(L + normalize(In.Eye)); // �n�[�t�x�N�g��
 float3 N = normalize(In.N);
 
   // return (In.Color+ pow(max(0,dot(N, H)), 10))*tex2D( tex0, In. Tex);// �g�U���{����
   return In.Color*1.5*tex2D( tex0, In. Tex)+ pow(max(0,dot(N, H)), 40); 
    //+ pow(max(0,dot(N, H)), 10); // ���ʔ��ˌ�
}

// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique Main
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}