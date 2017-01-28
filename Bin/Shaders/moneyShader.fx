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
float moneyLevel
	<
		float UIMin = 0.0;
		float UISoftMin = 0.0;
		float UIMax = 99.0;
		float UISoftMax = 99.0;
		float UIStep = 0.01;
		string UIName = "moneyLevel";
		int UIOrder = 2;
		string UIWidget = "Slider";
> = 0.5;

};

// ---------------------------------------- Textures -----------------------------------------
Texture2D moneyShader2dds
<
	string ResourceName = "moneyShader2.dds";
	string UIName = "moneyShader2dds";
	string ResourceType = "2D";
	string UIWidget = "FilePicker";
>;

SamplerState MMMLWWWSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};


// -------------------------------------- ShaderVertex --------------------------------------
struct APPDATA
{
	float3 Position : POSITION;
	float2 map1 : TEXCOORD0;
};

struct SHADERDATA
{
	float4 Position : SV_Position;
	float4 map1 : TEXCOORD0;
	half3 FogFactor : TEXCOORD1;
	float4 WorldPosition : TEXCOORD2;
};

SHADERDATA ShaderVertex(APPDATA IN)
{
	SHADERDATA OUT;

	OUT.Position = float4(IN.Position, 1);
	float4 OutUVs = float4(IN.map1.x, IN.map1.y, 0.0, 0.0);
	OUT.map1 = OutUVs;
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

	float AddOp = (IN.map1.xy.y + -0.25);
	float2 VectorConstruct = float2(IN.map1.xy.x, AddOp);
	float4 Sampler = moneyShader2dds.Sample(MMMLWWWSampler, float2(VectorConstruct.xy.x, 1-VectorConstruct.xy.y));
	float uvShiftTop = (IN.map1.xy.x + moneyLevel);
	float AddOp158 = (IN.map1.xy.y + -0.5);
	float SubOp = (AddOp158 - (moneyLevel * 0.25));
	float2 VectorConstruct91 = float2(uvShiftTop, SubOp);
	float4 Sampler68 = moneyShader2dds.Sample(MMMLWWWSampler, float2(VectorConstruct91.xy.x, 1-VectorConstruct91.xy.y));
	float3 MulOp = (Sampler.xyz * Sampler68.xyz.z);
	float4 VectorConstruct59 = float4(MulOp.x, MulOp.y, MulOp.z, Sampler68.xyz.z);
	if (MayaHwFogEnabled) { 
		float fogFactor = (1.0 - IN.FogFactor.x) * MayaHwFogColor.a; 
		VectorConstruct59.rgb	= lerp(VectorConstruct59.rgb, MayaHwFogColor.rgb, fogFactor); 
	} 

	OUT.Color = VectorConstruct59;

	return OUT;
}

// -------------------------------------- technique T0 ---------------------------------------
technique11 T0
<
	bool overridesDrawState = false;
	int isTransparent = 0;
>
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

