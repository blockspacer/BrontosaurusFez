struct VertexInput
{
	float4 position : POSITION;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 uv : TEX_COORD0;
	float4 color : COLOR;
};

cbuffer VertexConstantBuffer : register(b0)
{
	float2 position;
	float2 size;
	float4 rect;
	float4 color;
}

Texture2D albedo : register(t1);
SamplerState globalSamplerState;

PixelInput VS_Sprite(VertexInput input)
{	
	float2 outPosition;
	outPosition = input.position.xy * size;
	outPosition.x = outPosition.x + position.x;
	outPosition.y = outPosition.y + 1.f - position.y - size.y;

	//0 - 1 to -1 - 1
	outPosition = outPosition * 2.f;
	outPosition = outPosition - float2(1.f, 1.f);

	PixelInput output;
	output.position = float4(outPosition, 0.5f, 1.f);
	output.color = color;

	float2 uv;
	uv.x = rect.x + input.position.x * (rect.z - rect.x);
	uv.y = rect.y + input.position.y * (rect.w - rect.y);

	//flip uv i y-led
	output.uv = float2(uv.x, 1.f - uv.y);
	
	return output; 
}

float4 PS_Sprite(PixelInput input) : SV_TARGET
{
    float4 output = albedo.Sample(globalSamplerState, input.uv);
    //output.w = 1.0f;
	
    return output * input.color;
	
	//return float4(1.f, 1.f, 0.f, 1.f);
}




