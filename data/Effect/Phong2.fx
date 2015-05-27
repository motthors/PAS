// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------

float4x4 matWVP;

float4 LightDir; // ライトの方向
float4 vColor = {1.0f,1.0f,1.0f,1.0f};  // ライト＊メッシュの色
float4 vecCameraPos;  // カメラの位置（ローカル座標系）

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos   : POSITION;
    float4 Color  : COLOR0;
    float3 N   : TEXCOORD0;
    float3 Eye   : TEXCOORD1;
float2 Tex : TEXCOORD2;
};
//テクスチャ
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
// シーンの描画
// -------------------------------------------------------------
VS_OUTPUT VS(
      float4 Pos    : POSITION,          // ローカル位置座標
      float4 Normal : NORMAL,            // 法線ベクトル
float2 Tex : TEXCOORD
){
 VS_OUTPUT Out = (VS_OUTPUT)0;        // 出力データ
 
 // 座標変換
 Out.Pos = mul(Pos, matWVP);
 
 // 拡散光＋環境光
 float amb = -LightDir.w; // 環境光の強さ
 float4 L = LightDir; // ローカル座標系でのライトベクトル
 Out.Color = vColor * max(amb, dot(Normal, L));
 
 // 鏡面反射用のベクトル
 Out.N   = Normal.xyz;
 Out.Eye = vecCameraPos - Pos.xyz;
Out.Tex = Tex;//テクスチャ座標
 
 return Out;
}
// -------------------------------------------------------------
float4 PS(VS_OUTPUT In) : COLOR
{  
 float3 L = LightDir.xyz;
 float3 H = normalize(L + normalize(In.Eye)); // ハーフベクトル
 float3 N = normalize(In.N);
 
   // return (In.Color+ pow(max(0,dot(N, H)), 10))*tex2D( tex0, In. Tex);// 拡散光＋環境光
   return In.Color*1.5*tex2D( tex0, In. Tex)+ pow(max(0,dot(N, H)), 40); 
    //+ pow(max(0,dot(N, H)), 10); // 鏡面反射光
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique Main
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}