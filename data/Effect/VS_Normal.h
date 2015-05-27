
//通常の頂点変換

struct VS_IN
{
	float4 Pos		: POSITION;   	//頂点の座標
   	float4 Normal	: NORMAL;	//法線ベクトル
    float2 Tex		: TEXCOORD0; 	//テクスチャ座標
	//float4 Tangent	: TANGENT0;
    //float4 Binormal	: BINORMAL0;
};

struct VS_OUT
{
   	float4 Pos		: POSITION;     //頂点の座標
	float3 Col		: COLOR0;       //頂点カラー
   	float2 Tex		: TEXCOORD0;    //テクセル座標
   	float3 Normal	: TEXCOORD1;   //オブジェクトの法線ベクトル
   	float3 Look		: TEXCOORD2;   //頂点 -> 視点 ベクトル
};


//バーテックスシェーダー
VS_OUT VS_main( VS_IN IN )
{
	VS_OUT Out = (VS_OUT)0;
	
	//頂点の座標を行列変換する
	Out.Pos = mul( IN.Pos, matWVP );
	Out.Tex = IN.Tex; //テクセルはそのまま

	Out.Normal = normalize( mul(IN.Normal,matWorldIT).xyz );
	//Out.Normal = normalize( IN.Normal.xyz );

	float4 WorldPos = mul( IN.Pos, matWorld );
	Out.Look	= normalize(vecCameraPos.xyz - WorldPos.xyz); //頂点 -> 視点 へのベクトルを計算
    
	//頂点カラーをランバートで
	float3 Ln = normalize(-LightDir.xyz);
    float3 Nn = normalize(Out.Normal);
   	float ldn = dot(Ln,Nn);
	ldn = ldn*0.5 + 0.4;
	ldn += Emission;
	Out.Col = ldn;
	return Out;
}