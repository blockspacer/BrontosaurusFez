#include "stdafx.h"
#include "ScriptLoader.h"
#include "../LuaWrapper/SSlua/SSlua.h"

#include "GlobalLuaFunctions.h"
#include "ComponentMessageTypeToLua.h"

namespace SSlua
{
	typedef CU::GrowingArray<SSArgument> ArgumentList;
	typedef std::function<ArgumentList(const ArgumentList&)> LuaCallbackFunction;
}

void ScriptLoader::RegisterLuaFunctions(SSlua::LuaWrapper& aLuaWrapper)
{
	aLuaWrapper.RegisterFunction(ComponentGetParent, "GetParent", "Argument: component id (number), Return value: the component's parent's id. Returns 0 (zero) if it fails", false);
	aLuaWrapper.RegisterFunction(ComponentSubscribeToMessage, "SubscribeToMessage", "First argument: component id (number), second argument: messagetype (number), third argument: name of callback function to call in lua (string). Returns nothing. The callback function must take two arguments, where the first is the component id, and the second is a pointer to the message data", false);
	aLuaWrapper.RegisterFunction(ComponentGetMessageData, "GetMessageData", "First argument: pointer to the message data ('lightuserdata'), second argument: type of data (string). Returns the data specified as second argument", false);
	aLuaWrapper.RegisterFunction(ComponentNotifyParent, "NotifyParent", "First arument: component id (number), second argument: message type (number), third argument (optional): the data you want to send (number, string, vector2/3 whatever). Returns nothing", false);
	aLuaWrapper.RegisterFunction(GetMessageType, "GetMessageType", "Argument: string representing a message type, return value: number that is the actual message type. Returns -1 if the string doesn't match any id. Use this when subscribing to messages", true);
	
	aLuaWrapper.PrintDocumentation();
	ComponentMessage::PrintComponentsToFile("Script/Documentation/ComponentMessageTypes.txt");
}