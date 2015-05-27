

float4 PS_2DAdd( VS_OUT In ) : COLOR0
{
	float4 Out = 0.0f;
	Out = tex2D(etex0,In.Tex) + tex2D(etex1,In.Tex);
	return Out;
}