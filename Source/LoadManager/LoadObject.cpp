#include "stdafx.h"
#include "LoadObject.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "Game/PlayState.h"
#include "GameObject.h"
#include "ComponentManager.h"
#include "DL_Debug.h"
#include "GameObjectManager.h"
#include "CommonUtilities.h"

int LoadObject(KLoader::SLoadedComponentData someData)
{
	CGameObject* const gameObject = LoadManager::GetInstance().GetCurrentPLaystate()->GetObjectManager()->CreateGameObject();

	CU::Matrix44f& currentMatrix = gameObject->GetLocalTransform();

	CU::JsonObject PositionObject = someData.myData.at("position").GetJsonObject();

	const float unityScale = 100;

	const float positionX = PositionObject.at("x").GetNumber() * unityScale * -1;
	const float positionY = PositionObject.at("y").GetNumber() * unityScale;
	const float positionZ = PositionObject.at("z").GetNumber() * unityScale * -1;

	currentMatrix.SetPosition({positionX, positionY, positionZ});

	CU::JsonObject RotationObject = someData.myData.at("rotation").GetJsonObject();

	const float rotationX = RotationObject.at("x").GetNumber() / 180 * PI;
	const float rotationY = RotationObject.at("y").GetNumber() / 180 * PI;
	const float rotationZ = RotationObject.at("z").GetNumber() / 180 * PI;

	currentMatrix.Rotate(rotationX, rotationY, rotationZ);

	CU::JsonObject ScalingObject = someData.myData.at("scale").GetJsonObject();

	const float scaleX = ScalingObject.at("x").GetNumber();
	const float scaleY = ScalingObject.at("y").GetNumber();
	const float scaleZ = ScalingObject.at("z").GetNumber();

	currentMatrix.Scale({ scaleX, scaleY, scaleZ });

	gameObject->SetName(someData.myData.at("name").GetString().c_str());

	return gameObject->GetId();
}

void LinkObject(const int target, const int anOtherComponent)
{
	CComponent* const objectComponent = CComponentManager::GetInstance().GetComponent(target);
	CGameObject* const gameObject = dynamic_cast<CGameObject* const>(objectComponent);

	if (gameObject == nullptr)
	{
		DL_ASSERT("ERROR: component is not a game object !!");
	}

	CComponent* const otherComponent = CComponentManager::GetInstance().GetComponent(anOtherComponent);

	gameObject->AddComponent(otherComponent);
}
