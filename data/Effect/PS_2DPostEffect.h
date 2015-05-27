
struct PS_OUT
{
	float4 Col : COLOR0;
	float4 HDR : COLOR1;
};

PS_OUT PS_2DPostEffect( VS_OUT In )
{
	PS_OUT Out = (PS_OUT)0;
	Out.HDR = tex2D( etex0, In.Tex) - 1.0f;
	Out.Col = clamp(tex2D( etex0, In.Tex),
					float4(0.0, 0.0, 0.0, 0.0),
					float4(1.0, 1.0, 1.0, 1.0) );
	//Out.Col = Out.Col + float4(In.Tex.x, In.Tex.y, 0.0, 0.5);
	return Out;
}