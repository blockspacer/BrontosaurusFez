#pragma once

struct SSArgument;
enum class eSSType;

namespace SSlua
{
	typedef CU::GrowingArray<SSArgument> ArgumentList;
	typedef CU::GrowingArray<eSSType> TypeList;
	typedef std::function<ArgumentList(const ArgumentList&)> LuaCallbackFunction;
}

SSlua::ArgumentList GetPlayer(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentGetParent(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentNotifyParent(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentSubscribeToMessage(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList ComponentGetParentPosition(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList GameObjectGetPosition(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList GetMessageData(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList GetMessageType(const SSlua::ArgumentList& aArgumentList);
SSlua::ArgumentList SpawnParticles(const SSlua::ArgumentList& aArgumentList);

