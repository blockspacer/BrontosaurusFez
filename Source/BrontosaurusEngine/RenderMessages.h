#pragma once
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/DynamicString.h"
#include "../CommonUtilities/Vector2.h"
#include "../CommonUtilities/Camera.h"

#include "../GUI/GUIPixelConstantBuffer.h"


class CSkybox;
class CModelInstance;
class CModel;
class CSprite;
class CText;
class CCoolText;
class CPointLightInstance;
class CParticleEmitter;
class CStreakEmitterInstance;

struct SPixelConstantBuffer;
struct SParticle;

enum class eRasterizerState : int;
enum class eBlendState : int;
enum class eDepthStencilState : int;
enum class eSamplerState : int;

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
		eRenderFire,
		eRenderText,
		eRenderDebugObjs,
		eRenderAnimationModel
	};

	SRenderMessage(const eRenderMessageType aRenderMessageType);
	virtual ~SRenderMessage();

	eRenderMessageType myType;

};

struct SRenderModelMessage : SRenderMessage
{
	SRenderModelMessage();
	CU::Matrix44f myTransformation;
	CU::Matrix44f myLastFrameTransformation;
	CU::GrowingArray<CPointLightInstance*>* myPointLights;
	Lights::SDirectionalLight* myDirectionalLight;

	const char* myCurrentAnimation;
	float myAnimationTime;

	int myModelID;
};

struct SRenderAnimationModelMessage : SRenderModelMessage 
{
	SRenderAnimationModelMessage();
	static const unsigned int ourMaxBoneCount = 32u;
	static const unsigned int ourMatrixSize = sizeof(CU::Matrix44f);

	char myBoneMatrices[ourMaxBoneCount * ourMatrixSize];
};

struct SRenderGUIModelMessage : SRenderMessage
{
	SRenderGUIModelMessage();

	CU::Matrix44f myToWorld;
	SPixelConstantBuffer myPixelConstantBufferStruct;
	int myModelID;

};

struct SRenderParticlesMessage : SRenderMessage
{
	SRenderParticlesMessage();
	ParticleEmitterID particleEmitter;
	CU::GrowingArray<SParticle, unsigned short, false> particleList;
	CU::Matrix44f toWorld;
};

struct SRenderStreakMessage : SRenderMessage
{
	SRenderStreakMessage();
	CStreakEmitterInstance* streakEmitter;
};

struct SRenderFireMessage : SRenderMessage
{
	SRenderFireMessage();
	CU::Matrix44f myToWorldMatrix;
	int myFireID;
};

struct SRenderSpriteMessage : SRenderMessage
{
	SRenderSpriteMessage();
	CU::Vector2f myPosition;
	CU::Vector2f mySize;
	CU::Vector4f myRect;
	CU::Vector4f myColor;
	CSprite* mySprite;
};

struct SRenderSkyboxMessage : SRenderMessage
{
	SRenderSkyboxMessage();
	CSkybox* mySkybox;
};

struct SSetCameraMessage : SRenderMessage
{
	SSetCameraMessage();
	CU::Camera myCamera;
};


struct SChangeStatesMessage : SRenderMessage
{
	SChangeStatesMessage();
	eRasterizerState myRasterizerState;
	eBlendState myBlendState;
	eDepthStencilState myDepthStencilState;
	eSamplerState mySamplerState;
};


struct SRenderTextMessage : SRenderMessage
{
	SRenderTextMessage();
	CU::Vector4f myColor;
	CU::DynamicString myString;
	CU::Vector2f myPosition;
	CCoolText *myText;
};
