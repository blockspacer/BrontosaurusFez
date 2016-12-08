#include "stdafx.h"
#include "ParticleEmitterInstance.h"
#include "Renderer.h"
#include "Engine.h"
#include "ParticleEmitter.h"
#include "../CommonUtilities/CUTime.h"

CParticleEmitterInstance::CParticleEmitterInstance()
{
	myIsVisible = true;

	SEmitterData emitterData;

	emitterData.TexturePath = "";

	emitterData.EmissionRate = 1000.f;

	emitterData.StartAlpha = 1.0f;
	emitterData.EndAlpha = 0.0f;

	emitterData.StartSize = 5.0f;
	emitterData.EndSize = 15.0f;

	emitterData.MinParticleLifeTime = 1.0f;
	emitterData.MaxParticleLifeTime = 1.0f;

	emitterData.MinEmissionVelocity = { -10.0f, -10.0f, -10.0f };
	emitterData.MaxEmissionVelocity = { 10.0f, 10.0f, 10.0f };

	emitterData.NumOfParticles = 64;
	emitterData.EmissonLifeTime = -1.f;

	myEmitter = new CParticleEmitter(emitterData);
	myEmitter->Init();
}

CParticleEmitterInstance::CParticleEmitterInstance(const SEmitterData& aEmitterData)
{
	myInstanceID = 0;
	myIsVisible = true;
	myEmitter = new CParticleEmitter(aEmitterData);
	myEmitter->Init();
}


CParticleEmitterInstance::~CParticleEmitterInstance()
{
	SAFE_DELETE(myEmitter);
}


void CParticleEmitterInstance::Update(const CU::Time& aDeltaTime)
{
	myEmitter->Update(aDeltaTime, myToWorldSpace);
}

void CParticleEmitterInstance::Render()
{
	SRenderParticlesMessage msg;
	msg.particleEmitter = myEmitter;
	msg.toWorld = myToWorldSpace;
	RENDERER.AddRenderMessage(new SRenderParticlesMessage(msg));
	//myEmitter->Render(aCamera, myToWorldSpace);
}

void CParticleEmitterInstance::Activate()
{
	myEmitter->Activate();
}

void CParticleEmitterInstance::Deactivate()
{
	myEmitter->Deactivate();
}
