Texture2D sprite : register (t1);
SamplerState Sampler;


cbuffer ConstantBuffer : register( b0 ) //to vertex & geometry
{
    float4x4 cameraSpaceInversed;
    float4x4 projectionSpace;
}

cbuffer ToWorld : register(b1)
{
    float4x4 worldSpace;
}


struct InputVertex
{
    float4 position : POSITION;
    float size : SIZE;
    float alpha : ALPHA;
};
//missalligned?
struct InputPixel
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float alpha : ALPHA;
};

struct InputGeometry
{
    float4 position : SV_POSITION;
    float size : SIZE;
    float alpha : ALPHA;
    float rotation : ROTATION;
};


InputGeometry VS_PosSizeAlpha(InputVertex input)
{
    InputGeometry output = (InputGeometry)0;
    output.rotation = input.position.w;
    output.position = float4(input.position.xyz, 1.0f);
    //output.position = mul(worldSpace, output.position);
    output.position = mul(cameraSpaceInversed, output.position);

    output.size = input.size;
    output.alpha = input.alpha;

    return output;
}

float4 PS_PosSizeAlpha(InputPixel input) : SV_TARGET
{
    float4 color = sprite.Sample(Sampler, input.uv);

    return float4(color.rgb, color.a * input.alpha);
}


[maxvertexcount(4)] //whaah //one input vertex gives max 4 new vertices in this case I think?
void GS_PosSizeAlpha(point InputGeometry input[1], inout TriangleStream<InputPixel> triStream)
{
    const float4 offset[4] =
    {
        {-input[0].size, input[0].size, 0.0f, 1.0f},
        {input[0].size, input[0].size, 0.0f, 1.0f},
        {-input[0].size, -input[0].size, 0.0f, 1.0f},
        {input[0].size, -input[0].size, 0.0f, 1.0f}
    };

    const float2 uv_coord[4] =
    {
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 0.0f},
        {1.0f, 0.0f}
    };

    float4x4 rotationMatrix = (float4x4)0;
    rotationMatrix._11 = cos(input[0].rotation);
    rotationMatrix._12 = sin(input[0].rotation);
    rotationMatrix._21 = -sin(input[0].rotation);
    rotationMatrix._22 = cos(input[0].rotation);
    rotationMatrix._33 = 1.0f;
    rotationMatrix._44 = 1.0f;


    for(int i = 0 ; i < 4 ; ++i)
    {
        InputPixel vertex = (InputPixel)0;
        float4 offsetPos = mul(rotationMatrix, offset[i]);
        offsetPos.w = 0.0f;
        vertex.position = input[0].position + offsetPos;

        //vertex.position = mul(rotationMatrix, vertex.position);
        vertex.position = mul(projectionSpace, vertex.position);

        vertex.uv = uv_coord[i];
        vertex.alpha = input[0].alpha;

        triStream.Append(vertex);
    }
    triStream.RestartStrip();
}
