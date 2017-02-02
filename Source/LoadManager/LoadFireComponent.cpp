#include "stdafx.h"
#include "LoadFireComponent.h"

#include "..\Components\FireComponent.h"
#include "..\BrontosaurusEngine\FireEmitterInstance.h"
#include "..\BrontosaurusEngine\FireEmitterData.h"

int LoadFireComponent(KLoader::SLoadedComponentData someData)
{
	CScene* scene = LoadManager::GetInstance().GetCurrentScene();
	if (!scene) return NULL_COMPONENT;

	CFireEmitterInstance fireEmitter;
	SFireEmitterData fireData;

	fireData.myNoiseTexturePath = someData.myData["NoiseTexturePath"].GetString();
	fireData.myColorTexturePath = someData.myData["ColorTexturePath"].GetString();
	fireData.myAlphaTexturePath = someData.myData["AlphaTexturePath"].GetString();

	fireData.myScrollSpeeds[0] = /*1.3f;*/someData.myData["ScrollSpeeds"][0].GetFloat();
	fireData.myScrollSpeeds[1] = /*2.1f;*/ someData.myData["ScrollSpeeds"][1].GetFloat();
	fireData.myScrollSpeeds[2] = /*2.3f;*/ someData.myData["ScrollSpeeds"][2].GetFloat();

	fireData.myScales[0] = /*1.f;*/ someData.myData["Scales"][0].GetFloat();
	fireData.myScales[1] = /*2.f;*/ someData.myData["Scales"][1].GetFloat();
	fireData.myScales[2] = /*3.f;*/ someData.myData["Scales"][2].GetFloat();

	CU::CJsonValue distortions = someData.myData["Distortions"];

	fireData.myDistortions[0].Set(/*0.1f, 0.2f);*/ distortions[0]["x"].GetFloat(), distortions[0]["y"].GetFloat());
	fireData.myDistortions[1].Set(/*0.1f, 0.3f);*/ distortions[1]["x"].GetFloat(), distortions[1]["y"].GetFloat());
	fireData.myDistortions[2].Set(/*0.1f, 0.1f);*/ distortions[2]["x"].GetFloat(), distortions[2]["y"].GetFloat());

	fireData.myDistortionScale = /*0.8f*/ someData.myData["DistortionScale"].GetFloat();
	fireData.myDistortionBias = /*0.5f*/ someData.myData["DistortionBias"].GetFloat();

	fireEmitter.Init(fireData);
	fireEmitter.GetTransformation().Rotate(/*3.141592f*/someData.myData["RotationY"].GetFloat(), CU::Axees::Y);
	float scale = someData.myData["Scale"].GetFloat();
	fireEmitter.GetTransformation().m11 *= /*2.f;*/ scale;
	fireEmitter.GetTransformation().m22 *= /*2.f;*/ scale;
	fireEmitter.GetTransformation().m33 *= /*2.f;*/ scale;

	InstanceID id = scene->AddFireEmitters(fireEmitter);

	CFireComponent* component = new CFireComponent(*scene, id);

	return component->GetId();
}
