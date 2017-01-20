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
	float time : Time < string UIWidget = "None"; >;

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
float3 orbColor2
	<
		string UIName = "orbColor2";
		int UIOrder = 4;
		string UIWidget = "ColorPicker";
> = {0.0,0.0775376,0.672};

float3 orbColor1
	<
		string UIName = "orbColor1";
		int UIOrder = 3;
		string UIWidget = "ColorPicker";
> = {0.0,1.0,0.923067};

float stars
	<
		float UIMin = 0.0;
		float UISoftMin = 0.0;
		float UIMax = 1.0;
		float UISoftMax = 1.0;
		float UIStep = 0.01;
		string UIName = "stars";
		int UIOrder = 10;
		string UIWidget = "Slider";
> = 1.0;

float bubbleTimeFactor
	<
		float UIMin = 0.0;
		float UISoftMin = 0.0;
		float UIMax = 99.0;
		float UISoftMax = 99.0;
		float UIStep = 0.01;
		string UIName = "bubbleTimeFactor";
		int UIOrder = 2;
		string UIWidget = "Slider";
> = 0.15;

float waveTimeFactor
	<
		float UIMin = 0.0;
		float UISoftMin = 0.0;
		float UIMax = 99.0;
		float UISoftMax = 99.0;
		float UIStep = 0.01;
		string UIName = "waveTimeFactor";
		int UIOrder = 1;
		string UIWidget = "Slider";
> = 0.1;

float orbValue
	<
		float UIMin = 0.0;
		float UISoftMin = 0.0;
		float UIMax = 1.0;
		float UISoftMax = 1.0;
		float UIStep = 0.01;
		string UIName = "orbValue";
		string UIWidget = "Slider";
> = 0.780749;

};

// ---------------------------------------- Textures -----------------------------------------
Texture2D orbShaderdds
<
	string ResourceName = "orbShader.dds";
	string UIName = "orbShaderdds";
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

	float SubOp = (stars - 0.5);
	float AddOp = ((SubOp * 0.5) + IN.map1.xy.x);
	float MulOp = (time * bubbleTimeFactor);
	float SubOp98 = (IN.map1.xy.y - MulOp);
	float2 VectorConstruct = float2(AddOp, SubOp98);
	float4 Sampler = orbShaderdds.Sample(MMMLWWWSampler, float2(VectorConstruct.xy.x, 1-VectorConstruct.xy.y));
	float4 Sampler100 = orbShaderdds.Sample(MMMLWWWSampler, float2(IN.map1.xy.x, 1-IN.map1.xy.y));
	float AddOp137 = ((Sampler.xyz.z * 0.3) + Sampler100.xyz.y);
	float MulOp64 = (time * waveTimeFactor);
	float SubOp66 = (MulOp64 - IN.map1.xy.x);
	float SinOp = sin((time * 15.0));
	float AddOp152 = ((SinOp * 0.001) + IN.map1.xy.y);
	float MulOp126 = (orbValue * 0.5);
	float SubOp124 = ((AddOp152 + 0.25) - MulOp126);
	float2 VectorConstruct63 = float2(SubOp66, SubOp124);
	float4 Sampler36 = orbShaderdds.Sample(MMMLWWWSampler, float2(VectorConstruct63.xy.x, 1-VectorConstruct63.xy.y));
	float SubOp128 = (Sampler36.xyz.x - (1.0 - Sampler100.w));
	float SatOp = saturate(SubOp128);
	float3 MulOp60 = (lerp(orbColor2.xyz, orbColor1.xyz, AddOp137) * SatOp);
	float4 VectorConstruct61 = float4(MulOp60.x, MulOp60.y, MulOp60.z, SatOp);
	if (MayaHwFogEnabled) { 
		float fogFactor = (1.0 - IN.FogFactor.x) * MayaHwFogColor.a; 
		VectorConstruct61.rgb	= lerp(VectorConstruct61.rgb, MayaHwFogColor.rgb, fogFactor); 
	} 

	OUT.Color = VectorConstruct61;

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

