#pragma once
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/DynamicString.h"
//#include "../CommonUtilities/GrowingArray.h"
#include "../CommonUtilities/Vector2.h"
#include "../CommonUtilities/Camera.h"

#include "../GUI/GUIPixelConstantBuffer.h"


class CSkybox;
class CModelInstance;
class CModel;
class CSprite;
class CText;
class CPointLightInstance;
class CParticleEmitter;
class CStreakEmitterInstance;

struct SPixelConstantBuffer;

namespace CU
{
	class Camera;
}

namespace Lights
{
	struct SDirectionalLight;
}


struct SRenderMessage
{
	virtual ~SRenderMessage() {}
	enum class eRenderMessageType
	{
		eSetCamera,
		eRenderModel,
		eRenderGUIModel,
		eRenderSprite,
		eChangeStates,
		eRenderSkybox,
		eRenderParticles,
		eRenderStreak,
		eRenderText,
		eRenderDebugObjs,
	};
	eRenderMessageType myType;

};

struct SRenderModelMessage : SRenderMessage
{
	SRenderModelMessage() { myType = SRenderMessage::eRenderMessageType::eRenderModel; }
	CU::Matrix44f myTransformation;
	CU::Matrix44f myLastFrameTransformation;
	CModel* myModel;
	unsigned int myModelIndex; // This is how it shuld be, YEYE-YEA
	Lights::SDirectionalLight* myDirectionalLight;
	CU::GrowingArray<CPointLightInstance*>* myPointLights;

};

struct SRenderGUIModelMessage : SRenderMessage
{
	SRenderGUIModelMessage() { myType = SRenderMessage::eRenderMessageType::eRenderGUIModel; }

	CU::Matrix44f myToWorld;
	SPixelConstantBuffer myPixelConstantBufferStruct;
	CModel* myModel;
};

struct SRenderParticlesMessage : SRenderMessage
{
	SRenderParticlesMessage() { myType = SRenderMessage::eRenderMessageType::eRenderParticles; }
	CParticleEmitter* particleEmitter;
	CU::Matrix44f toWorld;
};

struct SRenderStreakMessage : SRenderMessage
{
	SRenderStreakMessage() { myType = SRenderMessage::eRenderMessageType::eRenderStreak; }
	CStreakEmitterInstance* streakEmitter;
};

struct SRenderSpriteMessage : SRenderMessage
{
	SRenderSpriteMessage() { myType = SRenderMessage::eRenderMessageType::eRenderSprite; }
	CU::Vector2f myPosition;
	CU::Vector2f mySize;
	CU::Vector4f myRect;
	CU::Vector4f myColor;
	CSprite* mySprite;
};

struct SRenderSkyboxMessage : SRenderMessage
{
	SRenderSkyboxMessage() { myType = SRenderMessage::eRenderMessageType::eRenderSkybox; }
	CSkybox* mySkybox;
};

struct SSetCameraMessage : SRenderMessage
{
	SSetCameraMessage() { myType = SRenderMessage::eRenderMessageType::eSetCamera; }
	CU::Camera myCamera;
};


struct SChangeStatesMessage : SRenderMessage
{
	SChangeStatesMessage() { myType = SRenderMessage::eRenderMessageType::eChangeStates; mySamplerState = eSamplerState::eSize; }

	eRasterizerState myRasterizerState;
	eBlendState myBlendState;
	eDepthStencilState myDepthStencilState;
	eSamplerState mySamplerState;
};


struct SRenderTextMessage : SRenderMessage
{
	SRenderTextMessage() { myType = SRenderMessage::eRenderMessageType::eRenderText; }
	CU::Vector4f myColor;
	CU::DynamicString myString;
	CU::Vector2f myPosition;
	CText* myText;

};
