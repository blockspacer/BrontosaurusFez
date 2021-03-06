#pragma once
#include "stdafx.h"
#include "GlobalLuaFunctions.h"
#include "ScriptHelperFunctions.h"

#include "../LuaWrapper/SSlua/SSlua.h"
#include "../LuaWrapper/SSArgument/SSArgument.h"

#include "../Components/ComponentMessage.h"
#include "../Components/ComponentManager.h"
#include "../Components/Component.h"
#include "../Components/GameObject.h"
#include "../Components/ComponentMessageCallback.h"

#include "ComponentMessageTypeToLua.h"
#include "ParticleEffectManager.h"
#include "PollingStation.h"

#define GLOBAL_LUA_FUNCTION_ERROR DL_MESSAGE_BOX
#define RETURN_VOID() return SSlua::ArgumentList()
#define RETURN_ZERO() return SSlua::ArgumentList({ SSArgument(ssLuaNumber(0.0)) })
#define PAPA CGameObject* parent

SSlua::ArgumentList GetPlayer(const SSlua::ArgumentList & aArgumentList)
{
	if (!ScriptHelper::AssertArgumentCount("GetPlayer", 0, aArgumentList.Size(), true))
	{
		GLOBAL_LUA_FUNCTION_ERROR("GetPlayer does not take any arguments, discarding them, not aborting");
	}

	CGameObject* player = PollingStation::playerObject;
	if (!player)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Player is NULL, returning 0 (null component id)");
		RETURN_ZERO();
	}

	return{ ssLuaNumber(player->GetId()) };
}

SSlua::ArgumentList ComponentGetParent(const SSlua::ArgumentList& aArgumentList)
{
	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, component manager not created");
		RETURN_ZERO();
	}

	if (!ScriptHelper::AssertArgumentList("GetParent", { eSSType::NUMBER }, aArgumentList, true))
	{
		RETURN_ZERO();
	}

	ComponentId componentID = aArgumentList[0].GetUInt();

	CComponent* component = componentManager->GetComponent(componentID);
	if (!component)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, component id %u represented NULL", componentID);
		RETURN_ZERO();
	}

	PAPA = component->GetParent();
	if (!parent)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, parent of component with id %u was NULL", componentID);
		RETURN_ZERO();
	}

	ComponentId parentID = parent->GetId();
	if (parentID == NULL_COMPONENT)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, parent of component with id %u was not registered", componentID);
		RETURN_ZERO();
	}

	SSlua::ArgumentList returnValues(1u);

	returnValues.Add(SSArgument(ssLuaNumber(static_cast<ssLuaNumber>(parentID))));
	return returnValues;
}

SSlua::ArgumentList GameObjectGetPosition(const SSlua::ArgumentList& aArgumentList)
{
	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetPosition, component manager not created");
		return{ ssLuaNumber(0.0), ssLuaNumber(0.0) , ssLuaNumber(0.0) };
	}

	if (!ScriptHelper::AssertArgumentList("GetPosition", { eSSType::NUMBER }, aArgumentList, true))
	{
		return{ ssLuaNumber(0.0), ssLuaNumber(0.0) , ssLuaNumber(0.0) };
	}

	ComponentId gameObjectID = aArgumentList[0].GetUInt();
	CComponent* gameObjectComponent = componentManager->GetComponent(gameObjectID);
	if (!gameObjectComponent)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetPosition, it was called on object that was null");
		return{ ssLuaNumber(0.0), ssLuaNumber(0.0) , ssLuaNumber(0.0) };
	}

	if (!gameObjectComponent->IsGameObject())
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetPosition, it was called on object that is not a gameobject");
		return{ ssLuaNumber(0.0), ssLuaNumber(0.0) , ssLuaNumber(0.0) };
	}

	CGameObject* gameObject = static_cast<CGameObject*>(gameObjectComponent);
	CU::Vector3f gameObjectPosition(gameObject->GetWorldPosition());
	
	SSlua::ArgumentList position(3);
		
	position.Add(ssLuaNumber(gameObjectPosition.x));
	position.Add(ssLuaNumber(gameObjectPosition.y));
	position.Add(ssLuaNumber(gameObjectPosition.z));

	return position;
}

SSlua::ArgumentList ComponentNotifyParent(const SSlua::ArgumentList& aArgumentList)
{
	"Third argument (optional): the data you want to send (number, string, vector2/3 whatever)";

	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, component manager not created");
		RETURN_VOID();
	}
	
	if (!ScriptHelper::AssertArgumentList("NotifyParent", { eSSType::NUMBER, eSSType::NUMBER }, aArgumentList, false))
	{
		RETURN_VOID();
	}

	ComponentId id = aArgumentList[0].GetUInt();

	CComponent* component = componentManager->GetComponent(id);
	if (!component)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in NotifyParent, component id %u represented NULL", id);
		RETURN_VOID();
	}

	int messageType = aArgumentList[1].GetInt();
	if (messageType >= static_cast<int>(eComponentMessageType::eLength))
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in NotifyParent, invalid message type %d", messageType);
		RETURN_VOID();
	}

	eComponentMessageType messageTypeE = static_cast<eComponentMessageType>(messageType);
	SComponentMessageData messageData;

	if (aArgumentList.Size() > 2u)
	{
		const SSArgument& thirdArgument = aArgumentList[2u];

		if (messageTypeE == eComponentMessageType::eOnCollisionEnter && thirdArgument.GetType() == eSSType::NUMBER)
		{
			messageData.myComponent = componentManager->GetComponent(thirdArgument.GetUInt());
			messageData.myGameObject->GetName();
		}
		else
		{
			std::string typeName("unknown");
			ComponentMessage::GetTypeName(messageTypeE, typeName);
			DL_MESSAGE_BOX("Third argument is not implemented for %s and %s. This is fine but you may not get the results you expect mvh carl", typeName.c_str(), thirdArgument.GetTypeName());
		}
	}

	component->NotifyParent(messageTypeE, messageData);

	RETURN_VOID();
}

SSlua::ArgumentList ComponentSubscribeToMessage(const SSlua::ArgumentList& aArgumentList)
{
	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in SubscribeToMessage, component manager not created");
		RETURN_VOID();
	}

	if (!ScriptHelper::AssertArgumentList("SubscribeToMessage", { eSSType::NUMBER, eSSType::NUMBER , eSSType::STRING }, aArgumentList, true))
	{
		RETURN_VOID();
	}

	ComponentId componentID = aArgumentList[0].GetUInt();
	CComponent* component = componentManager->GetComponent(componentID);
	if (!component)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in SubscribeToMessage, component with id %u was NULL", componentID);
		RETURN_VOID();
	}

	SComponentMessageCallback callbackInfo;
	callbackInfo.myFunctionName = aArgumentList[2].GetString();
	callbackInfo.myMaybeEnum = aArgumentList[1].GetInt();

	SComponentMessageData messageData;
	messageData.myComponentMessageCallback = &callbackInfo;
	component->Receive(eComponentMessageType::eAddMessageSubscription, messageData);

	RETURN_VOID();
}

SSlua::ArgumentList ComponentGetParentPosition(const SSlua::ArgumentList& aArgumentList)
{
	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParentPosition, component manager not created");
		return{ ssLuaNumber(0.0), ssLuaNumber(0.0) , ssLuaNumber(0.0) };
	}

	if (!ScriptHelper::AssertArgumentList("GetParentPosition", { eSSType::NUMBER }, aArgumentList, true))
	{
		return{ ssLuaNumber(0.0), ssLuaNumber(0.0) , ssLuaNumber(0.0) };
	}

	ComponentId componentID = aArgumentList[0].GetUInt();
	CComponent* component = componentManager->GetComponent(componentID);
	if (!component)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParentPosition, component is null");
		return{ ssLuaNumber(0.0), ssLuaNumber(0.0) , ssLuaNumber(0.0) };
	}

	PAPA = component->GetParent();
	if (!parent)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParentPosition, component's parent is null");
		return{ ssLuaNumber(0.0), ssLuaNumber(0.0) , ssLuaNumber(0.0) };
	}

	CU::Vector3f parentPosition(parent->GetWorldPosition());

	SSlua::ArgumentList position(3);

	position.Add(ssLuaNumber(parentPosition.x));
	position.Add(ssLuaNumber(parentPosition.y));
	position.Add(ssLuaNumber(parentPosition.z));

	return position;
}

SSlua::ArgumentList GetMessageData(const SSlua::ArgumentList& aArgumentList)
{
	SSlua::ArgumentList messageData(1);

	if (!ScriptHelper::AssertArgumentList("GetMessageData", { eSSType::LIGHTUSERDATA, eSSType::STRING }, aArgumentList, true))
	{
		messageData.Add(SSArgument());
		return messageData;
	}

	void* rawData = aArgumentList[0].GetUserData();
	SComponentMessageData* realData = static_cast<SComponentMessageData*>(rawData);
	if (!realData || !realData->myVoidPointer)
	{
		messageData.Add(SSArgument());
		return messageData;
	}

	const std::string stringArg = aArgumentList[1].GetString();
	if (stringArg == "number")
	{
		messageData.Add(ssLuaNumber(realData->myInt));
		return messageData;
	}
	if (stringArg == "string")
	{
		const char* str = realData->myString;
		if (strlen(str) < 100u) // if it is to large, it is probably an error
		{
			messageData.Add(ssLuaString(realData->myString));
			return messageData;
		}
	}
	if (stringArg == "gameobject")
	{
		messageData.Add(ssLuaNumber(realData->myGameObject->GetId()));
		return messageData;
	}

	messageData.Add(SSArgument());
	return messageData;
}

SSlua::ArgumentList GetMessageType(const SSlua::ArgumentList& aArgumentList)
{
	SSlua::ArgumentList messageType(1);

	if (!ScriptHelper::AssertArgumentList("GetMessageType", { eSSType::STRING }, aArgumentList, true))
	{
		messageType.Add(ssLuaNumber(-1.0));
		return messageType;
	}

	const std::string typeName = aArgumentList[0].GetString();
	int typeInt = ComponentMessage::GetType(typeName);
	if (typeInt == -1)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetMessageType, message type name %s could not match any message type", typeName.c_str());
		messageType.Add(ssLuaNumber(-1.0));
		return messageType;
	}

	messageType.Add(ssLuaNumber(typeInt));
	return messageType;
}

SSlua::ArgumentList SpawnParticles(const SSlua::ArgumentList& aArgumentList)
{
	CParticleEffectManager* particleEffectManager = CParticleEffectManager::GetInstance();
	if (!particleEffectManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in SpawnParticles, particle effect manager was null");
		RETURN_VOID();
	}

	if (!ScriptHelper::AssertArgumentList("SpawnParticles", { eSSType::STRING, eSSType::NUMBER, eSSType::NUMBER, eSSType::NUMBER }, aArgumentList, true))
	{
		RETURN_VOID();
	}

	const std::string particleType = aArgumentList[0].GetString(); // type
	CU::Vector3f spawnPosition(aArgumentList[1].GetFloat(), aArgumentList[2].GetFloat(), aArgumentList[3].GetFloat());

	particleEffectManager->SpawnParticle(particleType, spawnPosition);

	RETURN_VOID();
}
