//外部から設定する変数///////////////////////////////////

//ワールド、ビュー、射影座標変換マトリックス
float4x4 matWorld : World;
float4x4 matWorldIT : WorldInverseTranspose = (float4x4)0;
float4x4 matWVP : WorldViewProjection;

float4 vecCameraPos;

//照明/////////////////////////////////////////////////
//平行照明
//照明の方向ベクトル
float4 LightDir = {0.0, -1.0, -1.0, 1.0};
float3 SpecColor : Specular = {1.0f,1.0f,1.0f};
float  shininess = 10.0f;  //スペキュラーの強度
float  LambertPower = 1.0;       //ランバート強度
float  SpecularPower = 0.9;       //スペキュラーの強度
float3 AmbiColor : Ambient = {0.01f,0.01f,0.01f};	//環境光

//自然発光
float Emission = 0.0f;

#include"DefineTexture.h"