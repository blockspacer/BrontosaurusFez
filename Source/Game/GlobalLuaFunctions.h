#pragma once

struct SSArgument;

namespace SSlua
{
	typedef CU::GrowingArray<SSArgument> ArgumentList;
	typedef const ArgumentList& InArgumentList;
	typedef std::function<ArgumentList(InArgumentList)> LuaCallbackFunction;
}

SSlua::ArgumentList ComponentGetParent(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentNotifyParent(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentSubscribeToMessage(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentGetMessageData(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList GetMessageType(const SSlua::ArgumentList& aArgumentList);

