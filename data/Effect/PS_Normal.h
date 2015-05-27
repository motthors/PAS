
struct PS_OUT
{
	float4 MainCol	: COLOR0;
	//float4 HDR		: COLOR1;
	//float4 ShadeZbuf	: COLOR2;
};

//ƒtƒHƒ“
PS_OUT PS_main( VS_OUT In )
{ 
	PS_OUT Out = (PS_OUT)0;

	//Lanbert power
  	float3 Ln = normalize(LightDir).xyz;
    //float3 Nn = normalize(In.Normal);
   	//float ldn = dot(Ln,Nn);
	//ldn = ldn*0.6 + 0.4;
	//ldn += Emission;

	//phong
	float3 Ref = normalize( reflect( Ln, In.Normal ) );
	float3 Lambert	= In.Col + AmbiColor;
	float3 Phong	= pow( max( dot( Ref, In.Look),0.0) , shininess )*SpecularPower * SpecColor;
					
    
	float4 Color = tex2D(etex0, In.Tex);
	float3 result = Color.rgb * Lambert + Phong;

	//Out.HDR		= float4(result, Color.a) - float4(1.0, 1.0, 1.0, 1.0);
	Out.MainCol	=  float4(result, Color.a);
	//Out.MainCol	= clamp(float4(result, Color.a), 
	//					float4(0.0, 0.0, 0.0, 0.0),
	//					float4(1.0, 1.0, 1.0, 1.0) );//*/
    return Out;
}

//Œõ‚Ì‰e‹¿‚ðŽó‚¯‚È‚¢AR•\Ž¦
PS_OUT PS_AR( VS_OUT In )
{ 
	PS_OUT Out = (PS_OUT)0;
    Out.MainCol = tex2D(etex0, In.Tex);
	//Out.HDR		= Out.MainCol - 1.0;
    return Out;
}