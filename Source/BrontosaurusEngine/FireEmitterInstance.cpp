#include "stdafx.h"
#include "FireEmitterInstance.h"
#include "FireEmitterManager.h"

#include "Renderer.h"

CFireEmitterInstance::CFireEmitterInstance()
	: myFireEmitter(-1)
{
}

CFireEmitterInstance::CFireEmitterInstance(const CFireEmitterInstance& aOther)
{
	self = aOther;
}

CFireEmitterInstance::CFireEmitterInstance(CFireEmitterInstance&& aTemporary)
{
	self = std::move(aTemporary);
}

CFireEmitterInstance::~CFireEmitterInstance()
{
	if (myFireEmitter != -1)
	{
		CEngine::GetInstance()->GetFireEmitterManager().DestroyFireEmitter(myFireEmitter);
	}
}

CFireEmitterInstance& CFireEmitterInstance::operator=(const CFireEmitterInstance& aOther)
{
	myFireEmitter = aOther.myFireEmitter;
	myTransformation = aOther.myTransformation;

	CEngine::GetInstance()->GetFireEmitterManager().CopyFireEmitter(myFireEmitter);

	return self;
}

CFireEmitterInstance& CFireEmitterInstance::operator=(CFireEmitterInstance&& aTemporary)
{
	myFireEmitter = aTemporary.myFireEmitter;
	aTemporary.myFireEmitter = -1;
	myTransformation = aTemporary.myTransformation;

	return self;
}

void CFireEmitterInstance::Init(const SFireEmitterData& aFireEmitterData)
{
	myFireEmitter = CEngine::GetInstance()->GetFireEmitterManager().CreateFireEmitter(aFireEmitterData);
}

void CFireEmitterInstance::Render()
{
	SRenderFireMessage* renderFireMessage = new SRenderFireMessage();
	renderFireMessage->myToWorldMatrix = myTransformation;
	renderFireMessage->myFireID = myFireEmitter;

	RENDERER.AddRenderMessage(renderFireMessage);
}