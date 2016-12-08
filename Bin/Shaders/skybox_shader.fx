
TextureCube skybox : register(t0);
SamplerState Sampler;

cbuffer ConstantBuffer : register( b0 ) //to vertex & geometry
{
    float4x4 cameraSpaceInversed;
    float4x4 projectionSpace;
}

cbuffer ToWorld : register(b1) // to vertex
{
    float4x4 worldSpace;
}

cbuffer PixelBuffer : register( b0 ) // to Pixel 
{
    float4 cameraPos;
}


struct InputVertex
{
    float4 position : POSITION; //camera position
};

struct InputPixel
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POS;
    
};

struct InputGeometry
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POS;
    float size : SIZE;
    
};


InputPixel VS_Pos(InputVertex input)
{
    InputPixel output = (InputPixel)0;

    output.position = input.position;
    output.position = mul(worldSpace, output.position);
    output.worldPosition = output.position;
    output.position = mul(cameraSpaceInversed, output.position);
    output.position = mul(projectionSpace, output.position);
    return output;
}

[maxvertexcount(24)]
void GS_Pos(point InputGeometry input[1], inout TriangleStream<InputPixel> triStream)
{
    const float4 offset[8] = 
    {
        {-input[0].size, input[0].size, input[0].size, 0.0f},
        {-input[0].size, input[0].size, -input[0].size, 0.0f},
        
        {input[0].size, input[0].size, input[0].size, 0.0f},
        {input[0].size, input[0].size, -input[0].size, 0.0f},
        
        {-input[0].size, -input[0].size, input[0].size, 0.0f},
        {-input[0].size, -input[0].size, -input[0].size, 0.0f},
        
        {input[0].size, -input[0].size, input[0].size, 0.0f},
        {input[0].size, -input[0].size, -input[0].size, 0.0f}        
                
    };


    //Top
    InputPixel vertex = (InputPixel)0;
    vertex.position = input[0].position + offset[0];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[1];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[2];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[3];
    triStream.Append(vertex);
    triStream.RestartStrip();

    //Left
    vertex.position = input[0].position + offset[0];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[1];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[4];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[5];
    triStream.Append(vertex);
    triStream.RestartStrip();

    //bot
    vertex.position = input[0].position + offset[4];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[5];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[6];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[7];
    triStream.Append(vertex);
    triStream.RestartStrip();

    //right
    vertex.position = input[0].position + offset[6];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[7];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[2];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[3];
    triStream.Append(vertex);
    triStream.RestartStrip();

    //front
    vertex.position = input[0].position + offset[0];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[2];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[4];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[6];
    triStream.Append(vertex);
    triStream.RestartStrip();

    //back
    vertex.position = input[0].position + offset[1];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[3];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[5];
    triStream.Append(vertex);
    vertex.position = input[0].position + offset[7];
    triStream.Append(vertex);
    triStream.RestartStrip();

}


float4 PS_Pos(InputPixel input) : SV_TARGET
{
    float3 toPixel = normalize(input.worldPosition.xyz - cameraPos.xyz);
    float3 ambientLight = skybox.SampleLevel(Sampler, toPixel.xyz, 0).xyz;

    return float4(ambientLight, 1.0f);
}
