struct VertexInput
{
	float4 position : POSITION;
	float4 normals : NORMALS;
	float4 tangent : TANGENT;
	float4 biTangent : BITANGENT;
	float2 uv : UV;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 normals : NORMALS;
	float4 tangent : TANGENT;
	float4 biTangent : BITANGENT;
	float4 worldPosition : WORLDPOSITION;
	float2 uv : TEXCOORD;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 cameraSpaceInversed;
	float4x4 projectionSpace;
}

cbuffer ToWorld : register(b1)
{
	float4x4 worldSpace;
}

cbuffer GUIPixelBuffer : register(b1)
{
	float isProgressBar;
	float progressBarValue;
	float skipEmissive;
	float flashButton;
	//new srtuff
	float isHealthBar;
	float healthPercent;
	float isManaBar;
	float manaPercent;

	float4 cameraPosition;
}

TextureCube cubeMap : register(t0);
Texture2D albedo : register(t1);
Texture2D roughness : register(t2);
Texture2D ambientOcclusion : register(t3);
Texture2D emissive : register(t4);
Texture2D normalmap : register(t6);
Texture2D metallic : register(t11);

SamplerState globalSamplerState;

float4 PS_PBL(PixelInput input);
float4 PS_HealthBar(PixelInput input);
float4 PS_FlashButton(PixelInput input);

static const uint CubeMap_MipCount = 11;

static struct Light
{
	float4 color;
	float4 direction;

} DirectionalLight;

PixelInput VS_PosNormBinormTanTex(VertexInput input)
{
	PixelInput output;
	output.position = input.position;

	output.position = mul(worldSpace, output.position);
	output.worldPosition = output.position;
	output.position = mul(cameraSpaceInversed, output.position);
	output.position = mul(projectionSpace, output.position);

	output.normals = input.normals;
	output.tangent = input.tangent;
	output.biTangent = input.biTangent;

	output.uv = input.uv;
	
	return output;
}

float4 PS_PosNormBinormTanTex(PixelInput input) : SV_TARGET
{
	float4 output = (1.f - isProgressBar) * PS_PBL(input); //albedo.Sample(globalSamplerState, input.uv);

	output += isProgressBar * PS_HealthBar(input);

	output += PS_FlashButton(input) * (1.f - isProgressBar);
	output += PS_FlashButton(input) * isProgressBar * 3.f;
	
	return output;
}

float4 PS_HealthBar(PixelInput input)
{
	float Damage = 6.f * progressBarValue;

	float4 bGColor = float4(0.f, 0.f, 0.f, 1.f);
	float4 Sampler = albedo.Sample(globalSamplerState, input.uv);
	float4 VectorConstruct = float4(Sampler.xyz.x, Sampler.xyz.y, Sampler.xyz.z, Sampler.w);
	float MulOp = (Damage * 0.1667);
	float MulOp1374 = (input.uv.x * 5.333);
	float FloorOp = floor((MulOp + MulOp1374));
	float SatOp = saturate((1.0 - FloorOp));
	float4 LerpOp = lerp(bGColor, VectorConstruct, SatOp);
	float4 VectorConstruct1384 = float4(LerpOp.xyz.x, LerpOp.xyz.y, LerpOp.xyz.z, 1.0);

	return VectorConstruct1384;
}

float4 PS_FlashButton(PixelInput input)
{
	float4 flashColor = float4(1.f, 1.f, 1.f, 1.f);
	flashColor *= 0.2f;
	flashColor *= flashButton;
	return flashColor;
}

float3 PS_Lambert(PixelInput input, float3 aNormal, float3 aDirection)
{
	float3 output = saturate(dot(aNormal, -aDirection));
	return output;
}

//LYS i think!
float RoughToSPow(float fRoughness)
{
	return (2.0f / (fRoughness * fRoughness)) - 2.0f;
}

static const float k0 = 0.00098f;
static const float k1 = 0.9921f;
static const float fakeLysMaxSpecularPower = (2.0f / (0.0014f * 0.0014f)) - 2.0f;
static const float fMaxT = (exp2(-10.f / sqrt(fakeLysMaxSpecularPower)) - k0) / k1;
float GetSpecPowToMip(float fSpecPow, int nMips)
{
	float fSmulMaxT = (exp2(-10.0f / sqrt(fSpecPow)) - k0) / k1;

	return float(nMips - 1 - 0) * (1.0f - clamp(fSmulMaxT / fMaxT, 0.0f, 1.0f));
}

float3 PS_Albedo(PixelInput input)
{
	float3 output = albedo.Sample(globalSamplerState, input.uv).xyz;
	return output;
}

float3 PS_Roughness(PixelInput input)
{
	float3 output = roughness.Sample(globalSamplerState, input.uv).xyz;
	return output;
}

float3 PS_AmbientOcclusion(PixelInput input)
{
	float3 output = ambientOcclusion.Sample(globalSamplerState, input.uv).xyz;
	return output;
}

float3 PS_Emissive(PixelInput input)
{
	float3 output = emissive.Sample(globalSamplerState, input.uv).xyz;
	return (1.f - skipEmissive) * output;
}

float3 PS_ObjectNormal(PixelInput input)
{
	float3 normal = normalmap.Sample(globalSamplerState, input.uv).xyz;
	normal = (normal * 2.0f) - float3(1.0f, 1.0f, 1.0f);
	normal = normalize(normal);
	float3x3 tangentSpaceMatrix = float3x3(normalize(input.biTangent.xyz), normalize(input.tangent.xyz), normalize(input.normals.xyz));
	normal = mul(normal, tangentSpaceMatrix);

	return normal;
}

float3 PS_Metalness(PixelInput input)
{
	float3 output = metallic.Sample(globalSamplerState, input.uv).xyz;
	return output;
}

float3 PS_Substance(PixelInput input)
{
	float3 albedo = PS_Albedo(input);
	float3 metalness = PS_Metalness(input);
	float3 substance = (float3(0.04f, 0.04f, 0.04f) - (float3(0.04f, 0.04f, 0.04f) * metalness)) + albedo * metalness;
	
	return substance;
}

float3 PS_ReflectionFresnel(PixelInput input)
{
	float roughness = PS_Roughness(input).x;
	float3 substance = PS_Substance(input);
	float3 normal = PS_ObjectNormal(input);
	float3 toEye = normalize(cameraPosition.xyz - input.worldPosition.xyz);
	float VdotN = dot(toEye.xyz, normal);

	VdotN = saturate(VdotN);
	VdotN = 1.0f - VdotN;
	VdotN = pow(VdotN, 5); //whaah

	float3 fresnel = VdotN * (1.f - substance);
	fresnel = fresnel / (6 - 5 * roughness);
	fresnel = substance + fresnel;

	return fresnel;
}

float3 PS_Fresnel(PixelInput input, float3 aDirection)
{
	float3 toLight = -aDirection;
	float3 toEye = normalize(cameraPosition.xyz - input.worldPosition.xyz);
	float3 substance = PS_Substance(input);
	float3 halfvec = normalize(toLight + toEye);

	float LdotH = dot(toLight, halfvec);
	LdotH = saturate(LdotH);
	LdotH = 1.0f - LdotH;
	LdotH = pow(LdotH, 5);

	float3 fresnel = LdotH * (1.f - substance);
	fresnel = substance + fresnel;

	return fresnel;
}

float3 PS_Distribution(PixelInput input, float3 aDirection)
{
	float3 toLight = -aDirection.xyz;

	float roughness = PS_Roughness(input).x;
	float3 normal = PS_ObjectNormal(input);
	float3 toEye = normalize(cameraPosition.xyz - input.worldPosition.xyz);
	float3 halfvec = normalize(toLight + toEye.xyz);
	float HdotN = saturate(dot(halfvec, normal));

	float m = roughness * roughness; // roughness ^2 or what?
	float m2 = m * m; //roughness ^4 or what?

	float denominator = HdotN * HdotN * (m2 - 1.0f) + 1.0f;
	float distribution = m2 / (3.1415926538f * denominator * denominator);

	return distribution.xxx;
}

float3 PS_Visibility(PixelInput input, float3 aDirection)
{
	float3 toLight = -aDirection.xyz;

	float roughness = PS_Roughness(input).x;
	float roughnessRemapped = (roughness + 1.0f) / 2.0f;
	float3 normal = PS_ObjectNormal(input);
	float3 toEye = normalize(cameraPosition.xyz - input.worldPosition.xyz);

	float NdotL = saturate(dot(normal, toLight));
	float NdotV = saturate(dot(normal, toEye));

	float k = roughnessRemapped * roughnessRemapped * 1.7724f; //sqrt(PI) wth is K?
	float G1V = NdotV * (1.0f - k) + k;
	float G1L = NdotL * (1.0f - k) + k;
	float visibility = (NdotV * NdotL) / (G1V * G1L);

	return visibility.xxx;
}

float3 PS_MetalnessAlbedo(PixelInput input)
{
	float3 albedo = PS_Albedo(input);
	float3 metalness = PS_Metalness(input);
	float3 metalnessAlbedo = albedo - (albedo * metalness);

	return metalnessAlbedo.xyz;
}

float3 PS_AmbientDiffuse(PixelInput input)
{
	float3 normal = PS_ObjectNormal(input);
	float3 metalnessAlbedo = PS_MetalnessAlbedo(input);
	float3 ambientOcclusion = PS_AmbientOcclusion(input);

	float3 ambientLight = cubeMap.SampleLevel(globalSamplerState, normal.xyz, (uint) CubeMap_MipCount.x - 2).xyz; // FIX!
	float3 fresnel = PS_ReflectionFresnel(input);

	float3 output = metalnessAlbedo * ambientLight * ambientOcclusion * (float3(1.0f, 1.0f, 1.0f) - fresnel);
	return output;
}

float3 PS_AmbientSpecularity(PixelInput input)
{
	float3 normal = PS_ObjectNormal(input);
	float roughness = PS_Roughness(input).x;
	float3 ambientOcclusion = PS_AmbientOcclusion(input).xxx;

	float3 toEye = normalize(cameraPosition.xyz - input.worldPosition.xyz);
	float3 reflectionVector = -reflect(toEye, normal);

	float fakeLysSpecularPower = RoughToSPow(roughness);
	float lysMipMap = GetSpecPowToMip(fakeLysSpecularPower, (uint) CubeMap_MipCount.x);

	float3 ambientLight = cubeMap.SampleLevel(globalSamplerState, reflectionVector.xyz, lysMipMap).xyz;
	float3 fresnel = PS_ReflectionFresnel(input).xyz;

	float3 output = ambientLight * ambientOcclusion * fresnel;
	return output;
}

float3 PS_DirectDiffuse(PixelInput input, float3 normal, float3 aLightColor, float3 aLightDirection)
{
	float3 lightColor = aLightColor.xyz;

	float3 metalnessAlbedo = PS_MetalnessAlbedo(input);

	float3 lambert = PS_Lambert(input, normal, aLightDirection);
	float3 fresnel = PS_Fresnel(input, aLightDirection);

	float3 one = float3(1.0f, 1.0f, 1.0f);
	float3 output = metalnessAlbedo * lightColor * lambert * (one - fresnel);
	return output;
}

float3 PS_DirectSpecularity(PixelInput input, float3 normal, float3 aLightColor, float3 aLightDirection)
{
	float3 lightColor = aLightColor.xyz;

	float3 lambert = PS_Lambert(input, normal, aLightDirection);
	float3 fresnel = PS_Fresnel(input, aLightDirection);
	float3 distribution = PS_Distribution(input, aLightDirection).xxx;
	float3 visibility = PS_Visibility(input, aLightDirection).xxx;

	float3 output = lightColor * lambert * fresnel * distribution * visibility;
	return output;
}

float4 PS_PBL(PixelInput input)
{
	float3 normal = PS_ObjectNormal(input);
	float3 emissive = PS_Emissive(input);
	float3 albedo = PS_Albedo(input);
	float3 metalness = PS_Metalness(input);
	float3 metalnessAlbedo = albedo - (albedo * metalness);
	float3 ambientOcclusion = PS_AmbientOcclusion(input);
	float3 ambientLight = cubeMap.SampleLevel(globalSamplerState, normal.xyz, (uint) CubeMap_MipCount.x - 2).xyz; // FIX!
	float roughness = PS_Roughness(input).x;
	float3 substance = (float3(0.04f, 0.04f, 0.04f) - (float3(0.04f, 0.04f, 0.04f) * metalness)) + albedo * metalness;
	float3 toEye = normalize(cameraPosition.xyz - input.worldPosition.xyz);
	float VdotN = dot(toEye.xyz, normal);
	VdotN = saturate(VdotN);
	VdotN = 1.0f - VdotN;
	VdotN = pow(VdotN, 5); //whaah
	float3 fresnel = VdotN * (1.f - substance);
	fresnel = fresnel / (6 - 5 * roughness);
	fresnel = substance + fresnel;
	float3 ambientDiffuse = metalnessAlbedo * ambientLight * ambientOcclusion * (float3(1.0f, 1.0f, 1.0f) - fresnel);

	
	//AMBIENT SPEC

	float3 reflectionVector = -reflect(toEye, normal);
	float fakeLysSpecularPower = RoughToSPow(roughness);
	float lysMipMap = GetSpecPowToMip(fakeLysSpecularPower, (uint) CubeMap_MipCount.x);
	float3 ambientLightSpec = cubeMap.SampleLevel(globalSamplerState, reflectionVector.xyz, lysMipMap).xyz;
	float3 ambientSpecularity = ambientLightSpec * ambientOcclusion * fresnel;


	//DIRR DIFF

	DirectionalLight.direction = normalize(float4(-0.5f, -1.f, 0.f, 0.f));
	DirectionalLight.color = float4(1.f, 1.f, 1.f, 1.f);

	float3 lightColor = DirectionalLight.color.rgb;
	float3 lambert = PS_Lambert(input, normal, DirectionalLight.direction.xyz);
	float3 toLight = -DirectionalLight.direction.xyz;
	float3 halfvec = normalize(toLight + toEye);
	float LdotH = dot(toLight, halfvec);
	LdotH = saturate(LdotH);
	LdotH = 1.0f - LdotH;
	LdotH = pow(LdotH, 5);
	float3 Dirrfresnel = LdotH * (1.f - substance);
	Dirrfresnel = substance + Dirrfresnel;
	float3 one = float3(1.0f, 1.0f, 1.0f);
	float3 directionDiffuse = metalnessAlbedo * lightColor * lambert * (one - Dirrfresnel);


	//DIRR SPEC

	lightColor = DirectionalLight.color.rgb;
	float3 distribution = PS_Distribution(input, DirectionalLight.direction.xyz).xxx;
	float3 visibility = PS_Visibility(input, DirectionalLight.direction.xyz).xxx;
	float3 directionSpecularity = lightColor * lambert * Dirrfresnel * distribution * visibility;

	float4 output = float4(ambientDiffuse + ambientSpecularity + directionDiffuse + directionSpecularity + emissive, 1.f);
	return output;
}
