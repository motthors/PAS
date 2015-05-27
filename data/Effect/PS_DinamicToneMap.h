
struct PS_OUT
{
	float4 HDR	: COLOR0;
	//float4 HDR	: COLOR1;
	//float4 ShadeZbuf	: COLOR2;
};

float MaxToneLevel = 1.0;
float MinToneLevel = 0.0;

PS_OUT PS_DinamicToneMap( VS_OUT In )
{ 
	PS_OUT Out = (PS_OUT)0;
    //Out.MainCol = max( 1.0, tex2D(tex0, In.Tex) );
	Out.HDR	= float4(tex2D(etex0, In.Tex).xyz*2 - 1.0, 1.0);
	//Out.HDR = float4(In.Tex.r, In.Tex.g, 0.0, 1.0);
    return Out;
}