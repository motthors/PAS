

float4 PS_Blur_X( VS_OUT In ) : COLOR0
{
	float4 Out = 0;
   
	//Out = tex2D(tex0,In.Tex);
	Out += ( tex2D( etex0, In.Tex + float2(g_Texel0, 0.0) ) + tex2D( etex0, In.Tex - float2( g_Texel0, 0.0 ) ) ) * Weight0;
	Out += ( tex2D( etex0, In.Tex + float2(g_Texel1, 0.0) ) + tex2D( etex0, In.Tex - float2( g_Texel1, 0.0 ) ) ) * Weight1;
	Out += ( tex2D( etex0, In.Tex + float2(g_Texel2, 0.0) ) + tex2D( etex0, In.Tex - float2( g_Texel2, 0.0 ) ) ) * Weight2;
	Out += ( tex2D( etex0, In.Tex + float2(g_Texel3, 0.0) ) + tex2D( etex0, In.Tex - float2( g_Texel3, 0.0 ) ) ) * Weight3;
	Out += ( tex2D( etex0, In.Tex + float2(g_Texel4, 0.0) ) + tex2D( etex0, In.Tex - float2( g_Texel4, 0.0 ) ) ) * Weight4;
	Out += ( tex2D( etex0, In.Tex + float2(g_Texel5, 0.0) ) + tex2D( etex0, In.Tex - float2( g_Texel5, 0.0 ) ) ) * Weight5;
	Out += ( tex2D( etex0, In.Tex + float2(g_Texel6, 0.0) ) + tex2D( etex0, In.Tex - float2( g_Texel6, 0.0 ) ) ) * Weight6;
	Out += ( tex2D( etex0, In.Tex + float2(g_Texel7, 0.0) ) + tex2D( etex0, In.Tex - float2( g_Texel7, 0.0 ) ) ) * Weight7;

	return Out;
}


float4 PS_Blur_Y( VS_OUT In ) : COLOR0
{
	float4 Out = 0;
	
	//Out = tex2D(tex0,In.Tex);
	Out += ( tex2D( etex0, In.Tex + float2(0.0, g_Texel0) ) + tex2D( etex0, In.Tex - float2( 0.0, g_Texel0 ) ) ) * Weight0;
	Out += ( tex2D( etex0, In.Tex + float2(0.0, g_Texel1) ) + tex2D( etex0, In.Tex - float2( 0.0, g_Texel1 ) ) ) * Weight1;
	Out += ( tex2D( etex0, In.Tex + float2(0.0, g_Texel2) ) + tex2D( etex0, In.Tex - float2( 0.0, g_Texel2 ) ) ) * Weight2;
	Out += ( tex2D( etex0, In.Tex + float2(0.0, g_Texel3) ) + tex2D( etex0, In.Tex - float2( 0.0, g_Texel3 ) ) ) * Weight3;
	Out += ( tex2D( etex0, In.Tex + float2(0.0, g_Texel4) ) + tex2D( etex0, In.Tex - float2( 0.0, g_Texel4 ) ) ) * Weight4;
	Out += ( tex2D( etex0, In.Tex + float2(0.0, g_Texel5) ) + tex2D( etex0, In.Tex - float2( 0.0, g_Texel5 ) ) ) * Weight5;
	Out += ( tex2D( etex0, In.Tex + float2(0.0, g_Texel6) ) + tex2D( etex0, In.Tex - float2( 0.0, g_Texel6 ) ) ) * Weight6;
	Out += ( tex2D( etex0, In.Tex + float2(0.0, g_Texel7) ) + tex2D( etex0, In.Tex - float2( 0.0, g_Texel7 ) ) ) * Weight7;
	return Out;
}

