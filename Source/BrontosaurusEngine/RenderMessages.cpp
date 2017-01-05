#include "stdafx.h"
#include "RenderMessages.h"
#include "Renderer.h"

SRenderMessage::SRenderMessage(const eRenderMessageType aRenderMessageType)
	: myType(aRenderMessageType)
{
}

SRenderMessage::~SRenderMessage()
{
}

SRenderModelMessage::SRenderModelMessage()
	: SRenderMessage(eRenderMessageType::eRenderModel)
{
}

SRenderGUIModelMessage::SRenderGUIModelMessage()
	: SRenderMessage(eRenderMessageType::eRenderGUIModel)
{
}

SRenderParticlesMessage::SRenderParticlesMessage()
	: SRenderMessage(eRenderMessageType::eRenderParticles)
{
}

SRenderStreakMessage::SRenderStreakMessage()
	: SRenderMessage(eRenderMessageType::eRenderStreak)
{
}

SRenderFireMessage::SRenderFireMessage()
	: SRenderMessage(eRenderMessageType::eRenderFire)
{
}

SRenderSpriteMessage::SRenderSpriteMessage()
	: SRenderMessage(eRenderMessageType::eRenderSprite)
{
}

SRenderSkyboxMessage::SRenderSkyboxMessage()
	: SRenderMessage(eRenderMessageType::eRenderSkybox)
{
}

SSetCameraMessage::SSetCameraMessage()
	: SRenderMessage(eRenderMessageType::eSetCamera)
{
}

SChangeStatesMessage::SChangeStatesMessage()
	: SRenderMessage(eRenderMessageType::eChangeStates)
{
	mySamplerState = eSamplerState::eSize;
}

SRenderTextMessage::SRenderTextMessage()
	: SRenderMessage(eRenderMessageType::eRenderText)
{
}

SRenderModelDepthMessage::SRenderModelDepthMessage()
{
	myType = eRenderMessageType::eRenderModelDepth;
}

SRenderCameraQueueMessage::SRenderCameraQueueMessage()
	: SRenderMessage(eRenderMessageType::eRenderCameraQueue)
{

}

SRenderFullscreenEffectMessage::SRenderFullscreenEffectMessage()
	:SRenderMessage(SRenderMessage::eRenderMessageType::eRenderFullscreenEffect)
{
	myFirstUseDepthResource = false;
	mySecondUseDepthResource = false;
}

SActivateRenderPackageMessage::SActivateRenderPackageMessage()
	: SRenderMessage(SRenderMessage::eRenderMessageType::eActivateRenderPackage)
{

}