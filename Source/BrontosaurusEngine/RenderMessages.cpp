#include "stdafx.h"
#include "RenderMessages.h"
#include "Renderer.h"

//#include "../CommonUtilities/matrix44.h"
//#include "../CommonUtilities/DynamicString.h"
//#include "../CommonUtilities/Vector2.h"
//#include "../CommonUtilities/Camera.h"
//
//#include "../GUI/GUIPixelConstantBuffer.h"
#include "CoolText.h"

#include "EmitterData.h"

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

SRenderAnimationModelMessage::SRenderAnimationModelMessage()
{
	myType = eRenderMessageType::eRenderAnimationModel;
	memset(myBoneMatrices, 0, ourMaxBoneCount * ourMatrixSize);
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
