#pragma once
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/DynamicString.h"
#include "../CommonUtilities/Vector2.h"
#include "../CommonUtilities/Camera.h"

#include "../GUI/GUIPixelConstantBuffer.h"
#include "CoolText.h"


class CSkybox;
class CModelInstance;
class CModel;
class CSprite;
class CText;
class CPointLightInstance;
class CParticleEmitter;
class CStreakEmitterInstance;

struct SPixelConstantBuffer;
struct SParticle;

typedef unsigned int ParticleEmitterID;

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
		eRenderAnimationModel
	};
	eRenderMessageType myType;

};

struct SRenderModelMessage : SRenderMessage
{
	SRenderModelMessage() { myType = SRenderMessage::eRenderMessageType::eRenderModel; }
	CU::Matrix44f myTransformation;
	CU::Matrix44f myLastFrameTransformation;
	int myModelID;
	Lights::SDirectionalLight* myDirectionalLight;
	CU::GrowingArray<CPointLightInstance*>* myPointLights;

	float myAnimationTime;
	const char* myCurrentAnimation;

};

struct SRenderAnimationModelMessage : SRenderModelMessage 
{
	SRenderAnimationModelMessage() { myType = SRenderMessage::eRenderMessageType::eRenderAnimationModel; memset(myBoneMatrices, 0, ourMaxBoneCount * ourMatrixSize); }
	static const unsigned int ourMaxBoneCount = 32u;
	static const unsigned int ourMatrixSize = sizeof(CU::Matrix44f);

	char myBoneMatrices[ourMaxBoneCount * ourMatrixSize];
};

struct SRenderGUIModelMessage : SRenderMessage
{
	SRenderGUIModelMessage() { myType = SRenderMessage::eRenderMessageType::eRenderGUIModel; }

	CU::Matrix44f myToWorld;
	SPixelConstantBuffer myPixelConstantBufferStruct;
	int myModelID;

};

struct SRenderParticlesMessage : SRenderMessage
{
	SRenderParticlesMessage() { myType = SRenderMessage::eRenderMessageType::eRenderParticles; }
	ParticleEmitterID particleEmitter;
	CU::GrowingArray<SParticle, unsigned short, false> particleList;
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
	CCoolText* myText;

};
