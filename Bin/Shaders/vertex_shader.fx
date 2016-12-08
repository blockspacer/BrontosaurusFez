#include <shader_structs.fx>

cbuffer ConstantBuffer : register( b0 ) //to vertex
{
	float4x4 cameraSpaceInversed;
	float4x4 projectionSpace;
}

cbuffer ToWorld : register(b1)
{
	float4x4 worldSpace;
	float4x4 worldSpaceLastFrame;
}


//VERTEX SHADER

Pos_InputPixel VS_Pos(Pos_InputVertex input)
{
	Pos_InputPixel output;
	output.position = input.position;
	output.position = mul(worldSpace, output.position);
	output.position = mul(cameraSpaceInversed, output.position);
	output.position = mul(projectionSpace, output.position);

	return output;
}

PosNormBinormTanTex_InputPixel VS_PosNormBinormTanTex(PosNormBinormTanTex_InputVertex input)
{
	PosNormBinormTanTex_InputPixel output;
	float3x3 rotation = (float3x3)worldSpace;

	output.normals = float4(mul(rotation, normalize(input.normals.xyz)), 1.0f);
	output.tangent = float4(mul(rotation, normalize(input.tangent.xyz)), 1.0f);//input.tangent;
	output.biTangent = float4(mul(rotation, normalize(input.biTangent.xyz)), 1.0f);//input.biTangent;
	output.uv = input.uv;

	output.position = input.position;
	output.worldPosLastFrame = mul(worldSpaceLastFrame, output.position );
   
	output.position = mul(worldSpace, output.position );
	output.worldPosition = float4(output.position.xyz, 1.0f);

	output.position = mul(cameraSpaceInversed, output.position);
	output.viewPosition = float4(output.position.xyz, 1.0f);
	output.position = mul(projectionSpace ,output.position);

	output.worldPosLastFrame = mul(cameraSpaceInversed, output.worldPosLastFrame );
   // output.worldPosLastFrame = mul(projectionSpace, output.worldPosLastFrame );

	return output;
}
