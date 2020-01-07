Texture2D g_texDecal: register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0
{
    float4 g_vLightDir;
    float4 g_vEye;
};

cbuffer global_1
{
    matrix g_mW;
    matrix g_mWVP;
    float4 g_Ambient = float4(0, 0, 0, 0);
    float4 g_Diffuse = float4(1, 0, 0, 0); 
    float4 g_Specular = float4(1, 1, 1, 1);
};

//ç\ë¢ëÃ
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};


VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(Pos,g_mWVP);
    output.Tex = Tex;

    return output;
}

float4 PS( VS_OUTPUT input  ) : SV_Target
{
	return g_texDecal.Sample( g_samLinear, input.Tex );
}
