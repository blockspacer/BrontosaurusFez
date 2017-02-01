#pragma once

struct SSArgument;
enum class eSSType;

namespace SSlua
{
	typedef CU::GrowingArray<SSArgument> ArgumentList;
	typedef CU::GrowingArray<eSSType> TypeList;
	typedef std::function<ArgumentList(const ArgumentList&)> LuaCallbackFunction;
}

SSlua::ArgumentList ComponentGetParent(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentNotifyParent(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentSubscribeToMessage(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentGetMessageData(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList GetMessageType(const SSlua::ArgumentList& aArgumentList);

