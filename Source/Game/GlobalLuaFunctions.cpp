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

bool AssertArgumentCount(const std::string& aFunctionName, const unsigned int aPreferred, const unsigned int aActual, const bool aForceEquality);
bool AssertArgumentList(const std::string& aFunctionName, const SSlua::TypeList& aPreferred, const SSlua::ArgumentList& aRecieved, const bool aForceEquality);

SSlua::ArgumentList ComponentGetParent(const SSlua::ArgumentList& aArgumentList)
{
	SSlua::ArgumentList returnValues(1u);

	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, component manager not created");
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	if (!AssertArgumentList("GetParent", { eSSType::NUMBER }, aArgumentList, true))
	{
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	ComponentId componentID = aArgumentList[0].GetUInt();

	CComponent* component = componentManager->GetComponent(componentID);
	if (!component)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, component id %u represented NULL", componentID);
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	CGameObject* parent = component->GetParent();
	if (!parent)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, parent of component with id %u was NULL", componentID);
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	ComponentId parentID = parent->GetId();
	if (parentID == NULL_COMPONENT)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, parent of component with id %u was not registered", componentID);
		returnValues.Add(ssLuaNumber(0.0));
		return returnValues;
	}

	returnValues.Add(SSArgument(ssLuaNumber(static_cast<ssLuaNumber>(parentID))));
	return returnValues;
}

SSlua::ArgumentList ComponentNotifyParent(const SSlua::ArgumentList& aArgumentList)
{
	"Third argument (optional): the data you want to send (number, string, vector2/3 whatever)";

	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in GetParent, component manager not created");
		return SSlua::ArgumentList();		
	}
	
	if (!AssertArgumentList("NotifyParent", { eSSType::NUMBER, eSSType::NUMBER }, aArgumentList, false))
	{
		return SSlua::ArgumentList();
	}

	ComponentId id = aArgumentList[0].GetUInt();

	CComponent* component = componentManager->GetComponent(id);
	if (!component)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in NotifyParent, component id %u represented NULL", id);
		return SSlua::ArgumentList();
	}

	int messageType = aArgumentList[1].GetInt();
	if (messageType >= static_cast<int>(eComponentMessageType::eLength))
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in NotifyParent, invalid message type %d", messageType);
		return SSlua::ArgumentList();
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

	return SSlua::ArgumentList();
}

SSlua::ArgumentList ComponentSubscribeToMessage(const SSlua::ArgumentList& aArgumentList)
{
	CComponentManager* componentManager = CComponentManager::GetInstancePtr();
	if (!componentManager)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in SubscribeToMessage, component manager not created");
		return SSlua::ArgumentList();
	}

	if (!AssertArgumentList("SubscribeToMessage", { eSSType::NUMBER, eSSType::NUMBER , eSSType::STRING }, aArgumentList, true))
	{
		return SSlua::ArgumentList();
	}

	ComponentId componentID = aArgumentList[0].GetUInt();
	CComponent* component = componentManager->GetComponent(componentID);
	if (!component)
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in SubscribeToMessage, component with id %u was NULL", componentID);
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

	if (!AssertArgumentList("GetMessageData", { eSSType::LIGHTUSERDATA, eSSType::STRING }, aArgumentList, true))
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

	if (!AssertArgumentList("GetMessageType", { eSSType::STRING }, aArgumentList, true))
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

bool AssertArgumentCount(const std::string& aFunctionName, const unsigned int aPreferred, const unsigned int aRecieved, const bool aForceEquality)
{
	if (aRecieved < aPreferred || (aRecieved != aPreferred && aForceEquality))
	{
		GLOBAL_LUA_FUNCTION_ERROR("Error in %s, expected %u argument, got %u", aFunctionName.c_str(), aPreferred, aRecieved);
		return false;
	}

	return true;
}

bool AssertArgumentList(const std::string& aFunctionName, const SSlua::TypeList& aPreferred, const SSlua::ArgumentList& aRecieved, const bool aForceEquality)
{
	if (!AssertArgumentCount(aFunctionName, aPreferred.Size(), aRecieved.Size(), aForceEquality))
	{
		return false;
	}

	bool success = true;

	for (unsigned int i = 0; i < aPreferred.Size(); ++i)
	{
		if (aPreferred[i] != aRecieved[i].GetType())
		{
			GLOBAL_LUA_FUNCTION_ERROR("Argument error in %s: expected %s, got %s", aFunctionName, SSArgument::GetTypeName(aPreferred[i]), aRecieved[i].GetTypeName());
			success = false;
		}
	}

	return success;
}
