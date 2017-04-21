cbuffer cbPerFrame
{
    float gTime;
};

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 PosL     : POSITION;
    float3 TargetL  : TARGET;
    float3 NormalL  : NORMAL;
    float2 Tex0     : TEXCOORD;
    float2 Tex1     : TEXCOORD;
	float4 Color    : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

//    vin.PosL.xy += 0.5f * sin(vin.PosL.x) * sin(3.0f * gTime);
//    vin.PosL.z *= 0.6f + 0.4f * sin(2.0f * gTime);
	// 转换到齐次剪裁空间
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// 将顶点颜色直接传递到像素着色器
	vout.Color = vin.Color;

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
}

RasterizerState wireframe
{
    FillMode = Wireframe;
    CullMode = Back;
};

technique11 ColorTech
{
	pass P0
    {
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
        //SetRasterizerState(wireframe);
	}
}