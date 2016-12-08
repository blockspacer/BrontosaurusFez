// ----------------------------------------- Header ------------------------------------------
#ifndef SFX_HLSL_3
	#define SFX_HLSL_3
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
	bool MayaHwFogEnabled : HardwareFogEnabled < string UIWidget = "None"; > = false; 
	int MayaHwFogMode : HardwareFogMode < string UIWidget = "None"; > = 0; 
	float MayaHwFogStart : HardwareFogStart < string UIWidget = "None"; > = 0.0; 
	float MayaHwFogEnd : HardwareFogEnd < string UIWidget = "None"; > = 100.0; 
	float MayaHwFogDensity : HardwareFogDensity < string UIWidget = "None"; > = 0.1; 
	float4 MayaHwFogColor : HardwareFogColor < string UIWidget = "None"; > = { 0.5, 0.5, 0.5, 1.0 }; 


float4x4 viewPrj : ViewProjection < string UIWidget = "None"; >;


// --------------------------------------- Per Object -----------------------------------------
float4x4 world : World < string UIWidget = "None"; >;

float4x4 wvp : WorldViewProjection < string UIWidget = "None"; >;


// --------------------------------------- Attributes -----------------------------------------
float Damage
<
	float UIMin = 0.0;
	float UISoftMin = 0.0;
	float UIMax = 99.0;
	float UISoftMax = 99.0;
	float UIStep = 0.01;
	string UIName = "Damage";
	string UIWidget = "Slider";
> = 0.0;


// ---------------------------------------- Textures -----------------------------------------
Texture2D guipsd
<
	string ResourceName = "sourceimages/gui.psd";
	string UIName = "guipsd";
	string ResourceType = "2D";
	string UIWidget = "FilePicker";
>;

sampler2D guipsdSampler = sampler_state 
{
	Texture = <guipsd>;
	MinFilter = MIN_MAG_MIP_LINEAR;
	MagFilter = MIN_MAG_MIP_LINEAR;
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
	float4 Position : POSITION;
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
	float4 Color : COLOR0;
};

PIXELDATA ShaderPixel(SHADERDATA IN)
{
	PIXELDATA OUT;

	float4 bGColor = float4(0.0,0.0,0.0,1.0);
	float4 Sampler = tex2D(guipsdSampler, float2(IN.map1.xy.x, 1-IN.map1.xy.y));
	float4 VectorConstruct = float4(Sampler.xyz.x, Sampler.xyz.y, Sampler.xyz.z, Sampler.w);
	float MulOp = (Damage * 0.1667);
	float MulOp1374 = (IN.map1.xy.x * 5.333);
	float FloorOp = floor((MulOp + MulOp1374));
	float SatOp = saturate((1.0 - FloorOp));
	float4 LerpOp = lerp(bGColor, VectorConstruct, SatOp);
	float4 VectorConstruct1384 = float4(LerpOp.xyz.x, LerpOp.xyz.y, LerpOp.xyz.z, 1.0);
	if (MayaHwFogEnabled) { 
		float fogFactor = (1.0 - IN.FogFactor.x) * MayaHwFogColor.a; 
		VectorConstruct1384.rgb	= lerp(VectorConstruct1384.rgb, MayaHwFogColor.rgb, fogFactor); 
	} 

	OUT.Color = VectorConstruct1384;

	return OUT;
}

// -------------------------------------- technique T0 ---------------------------------------
technique T0
{
	pass P0
	{
		VertexShader = compile vs_3_0 ShaderVertex();
		PixelShader = compile ps_3_0 ShaderPixel();
	}

}

