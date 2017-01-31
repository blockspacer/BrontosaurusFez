#pragma once
#include "stdafx.h"
#include "GlobalLuaFunctions.h"
#include "../LuaWrapper/SSlua/SSlua.h"
#include "../LuaWrapper/SSArgument/SSArgument.h"
#include "Component.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include "ComponentMessage.h"
#include "../Components/ComponentMessageCallback.h"
#include "ComponentMessageTypeToLua.h"

#define GLOBAL_LUA_FUNCTION_ERROR DL_MESSAGE_BOX

SSlua::ArgumentList ComponentGetParent(const SSlua::ArgumentList& aArgumentList)
{
	SSlua::ArgumentList returnValues(1);

	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, component manager not created");
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	if (aArgumentList.Size() != 1)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, expected 1 argument, got %d", (int)aArgumentList.Size());
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	if (aArgumentList[0].GetType() != eSSType::NUMBER)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, expected number, got %s", aArgumentList[0].GetTypeName());
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	ComponentId componentID = aArgumentList[0].GetUInt();

	CComponent* component = componentManager->GetComponent(componentID);
	if (!component)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, component id %d represented NULL", componentID);
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	CGameObject* parent = component->GetParent();
	if (!parent)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, parent of component with id %d was NULL", componentID);
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	ComponentId parentID = parent->GetId();
	if (parentID == NULL_COMPONENT)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, parent of component with id %d was not registered", componentID);
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	returnValues.Add(SSArgument(ssLuaNumber(static_cast<ssLuaNumber>(parentID))));
	return returnValues;
}

SSlua::ArgumentList ComponentSubscribeToMessage(const SSlua::ArgumentList& aArgumentList)
{
	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in ComponentSubscribe, component manager not created");
		return SSlua::ArgumentList();
	}

	if (aArgumentList.Size() != 3)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in ComponentSubscribe, expected 2 argument, got %d", static_cast<int>(aArgumentList.Size()));
		return SSlua::ArgumentList();
	}

	if (aArgumentList[0].GetType() != eSSType::NUMBER)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in ComponentSubscribe, expected number, got %s", aArgumentList[0].GetTypeName());
		return SSlua::ArgumentList();
	}

	if (aArgumentList[1].GetType() != eSSType::NUMBER)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in ComponentSubscribe, expected number, got %s", aArgumentList[0].GetTypeName());
		return SSlua::ArgumentList();
	}

	if (aArgumentList[2].GetType() != eSSType::STRING)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in ComponentSubscribe, expected string, got %s", aArgumentList[0].GetTypeName());
		return SSlua::ArgumentList();
	}

	ComponentId componentID = aArgumentList[0].GetUInt();
	CComponent* component = componentManager->GetComponent(componentID);
	if (!component)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in ComponentSubscribe, component with id %d was NULL", (int)componentID);
		return SSlua::ArgumentList();
	}

	SComponentMessageCallback callbackInfo;
	callbackInfo.myFunctionName = aArgumentList[2].GetString();
	callbackInfo.myMaybeEnum = aArgumentList[1].GetInt();

	SComponentMessageData messageData;
	messageData.myComponentMessageCallback = &callbackInfo;
	component->Receive(eComponentMessageType::eAddMessageSubscription, messageData);

	return SSlua::ArgumentList();
}

SSlua::ArgumentList ComponentGetMessageData(const SSlua::ArgumentList& aArgumentList)
{
	SSlua::ArgumentList messageData(1);
	if (aArgumentList.Size() != 2)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetMessageType, expected 2 argument, got %d", static_cast<int>(aArgumentList.Size()));
		messageData.Add(SSArgument());
		return messageData;
	}

	if (aArgumentList[0].GetType() != eSSType::LIGHTUSERDATA)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetMessageType, expected pointer, got %s", aArgumentList[0].GetTypeName());
		messageData.Add(SSArgument());
		return messageData;
	}

	if (aArgumentList[1].GetType() != eSSType::STRING)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetMessageType, expected string, got %s", aArgumentList[1].GetTypeName());
		messageData.Add(SSArgument());
		return messageData;
	}

	void* rawData = aArgumentList[0].GetUserData();
	SComponentMessageData* realData = static_cast<SComponentMessageData*>(rawData);
	const std::string stringArg = aArgumentList[1].GetString();
	if (stringArg == "number")
	{
		messageData.Add(ssLuaNumber(realData->myInt));
		return messageData;
	}

	messageData.Add(SSArgument());
	return messageData;
}

SSlua::ArgumentList GetMessageType(const SSlua::ArgumentList& aArgumentList)
{
	SSlua::ArgumentList messageType(1);
	if (aArgumentList.Size() != 1)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetMessageType, expected 1 argument, got %d", static_cast<int>(aArgumentList.Size()));
		messageType.Add(ssLuaNumber(-1.0));
		return messageType;
	}

	if (aArgumentList[0].GetType() != eSSType::STRING)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in ComponentSubscribe, expected string, got %s", aArgumentList[0].GetTypeName());
		messageType.Add(ssLuaNumber(-1.0));
		return messageType;
	}

	std::string typeName = aArgumentList[0].GetString();
	int typeInt = ComponentMessage::GetType(typeName);
	if (typeInt == -1)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in ComponentSubscribe, message type name %s could not match any message type", typeName.c_str());
		messageType.Add(ssLuaNumber(-1.0));
		return messageType;
	}

	messageType.Add(ssLuaNumber(typeInt));
	return messageType;
}
