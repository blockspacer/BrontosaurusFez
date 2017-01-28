// ----------------------------------------- Header ------------------------------------------
#ifndef SFX_HLSL_5
	#define SFX_HLSL_5
#endif 
#ifndef _MAYA_
	#define _MAYA_
#endif 



#ifndef half 
	#define half float 
	#define half2 float2 
	#define half3 float3 
	#define half4 float4 
#endif 



// ----------------------------------- Per Frame --------------------------------------
cbuffer UpdatePerFrame : register(b0)
{
	float4x4 viewI : ViewInverse < string UIWidget = "None"; >;

	bool MayaHwFogEnabled : HardwareFogEnabled < string UIWidget = "None"; > = false; 
	int MayaHwFogMode : HardwareFogMode < string UIWidget = "None"; > = 0; 
	float MayaHwFogStart : HardwareFogStart < string UIWidget = "None"; > = 0.0; 
	float MayaHwFogEnd : HardwareFogEnd < string UIWidget = "None"; > = 100.0; 
	float MayaHwFogDensity : HardwareFogDensity < string UIWidget = "None"; > = 0.1; 
	float4 MayaHwFogColor : HardwareFogColor < string UIWidget = "None"; > = { 0.5, 0.5, 0.5, 1.0 }; 


	float4x4 viewPrj : ViewProjection < string UIWidget = "None"; >;

};

// --------------------------------------- Per Object -----------------------------------------
cbuffer UpdatePerObject : register(b1)
{
	float4x4 world : World < string UIWidget = "None"; >;

	float4x4 wvp : WorldViewProjection < string UIWidget = "None"; >;

};

// --------------------------------------- Attributes -----------------------------------------
cbuffer UpdateAttributes : register(b2)
{
float4 RimlightColor
	<
		string UIName = "RimlightColor";
		string UIWidget = "ColorPicker";
> = {0.0,1.0,0.5,1.0};

};

// -------------------------------------- ShaderVertex --------------------------------------
struct APPDATA
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};

struct SHADERDATA
{
	float4 Position : SV_Position;
	float4 WorldPosition : TEXCOORD0;
	float4 Normal : NORMAL;
	half3 FogFactor : TEXCOORD1;
};

SHADERDATA ShaderVertex(APPDATA IN)
{
	SHADERDATA OUT;

	OUT.Position = float4(IN.Position, 1);
	float4 WorldPos = mul(OUT.Position, world);
	OUT.WorldPosition = WorldPos;
	float3 MulOp = mul(IN.Normal, ((float3x3)world));
	float3 NormalN = normalize(MulOp);
	float4 WorldNormal = float4(NormalN.x, NormalN.y, NormalN.z, 1.0);
	OUT.Normal = WorldNormal;
	OUT.WorldPosition = (mul(float4(IN.Position,1), world));
	float4 _HPosition = mul( float4(OUT.WorldPosition.xyz, 1), viewPrj ); 
	float fogFactor = 0.0; 
	if (MayaHwFogMode == 0) { 
				fogFactor = saturate((MayaHwFogEnd - _HPosition.z) / (MayaHwFogEnd - MayaHwFogStart)); 
	} 
	else if (MayaHwFogMode == 1) { 
				fogFactor = 1.0 / (exp(_HPosition.z * MayaHwFogDensity)); 
	} 
	else if (MayaHwFogMode == 2) { 
				fogFactor = 1.0 / (exp(pow(_HPosition.z * MayaHwFogDensity, 2))); 
	} 
	OUT.FogFactor = float3(fogFactor, fogFactor, fogFactor); 

	float4 WVSpace = mul(OUT.Position, wvp);
	OUT.Position = WVSpace;

	return OUT;
}

// -------------------------------------- ShaderPixel --------------------------------------
struct PIXELDATA
{
	float4 Color : SV_Target;
};

PIXELDATA ShaderPixel(SHADERDATA IN)
{
	PIXELDATA OUT;

	float4 TextureIn = float4(0.153,0.0,0.0706146,1.0);
	float3 CameraPosition = viewI[3].xyz;
	float3 CamVec = (CameraPosition - IN.WorldPosition.xyz);
	float3 CamVecNorm = normalize(CamVec);
	float AddOp = (-0.4 + CamVecNorm.y);
	float3 VectorConstruct = float3(CamVecNorm.x, AddOp, CamVecNorm.z);
	float3 NormOp = normalize(IN.Normal.xyz);
	float AddOp75 = (dot(VectorConstruct.xyz, NormOp) + -0.4);
	float PowOp = pow(AddOp75, 0.182219);
	float Contrastmulstiplier = ((PowOp - 0.5) * 5.0);
	float ContrastAdd = (Contrastmulstiplier + 0.5);
	float SatOp = saturate(ContrastAdd);
	float AddOp92 = (dot(CamVecNorm, NormOp) + -0.4);
	float PowOp90 = pow(AddOp92, 0.144385);
	float Contrastmulstiplier98 = ((PowOp90 - 0.5) * 5.0);
	float ContrastAdd97 = (Contrastmulstiplier98 + 0.5);
	float SatOp104 = saturate(ContrastAdd97);
	float OneMinusOp = (1.0 - (SatOp * SatOp104));
	float4 AddOp84 = (TextureIn + (RimlightColor * OneMinusOp));
	float4 SatOp85 = saturate(AddOp84);
	float4 VectorConstruct102 = float4(SatOp85.xyz.x, SatOp85.xyz.y, SatOp85.xyz.z, 1.0);
	if (MayaHwFogEnabled) { 
		float fogFactor = (1.0 - IN.FogFactor.x) * MayaHwFogColor.a; 
		VectorConstruct102.rgb	= lerp(VectorConstruct102.rgb, MayaHwFogColor.rgb, fogFactor); 
	} 

	OUT.Color = VectorConstruct102;

	return OUT;
}

// -------------------------------------- technique T0 ---------------------------------------
technique11 T0
{
	pass P0
	<
		string drawContext = "colorPass";
	>
	{
		SetVertexShader(CompileShader(vs_5_0, ShaderVertex()));
		SetPixelShader(CompileShader(ps_5_0, ShaderPixel()));
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetGeometryShader(NULL);
	}

}

