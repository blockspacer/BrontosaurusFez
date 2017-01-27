#include "stdafx.h"
#include "ScriptComponent.h"
#include "../LuaWrapper/SSlua/SSlua.h"

std::string CScriptComponent::ourLastErrorMessage("");

CScriptComponent::CScriptComponent()
	: mySubscribedComponentMessages(4)
	, myFunctionTableIndex(-1)
{
}

CScriptComponent::~CScriptComponent()
{
}

CScriptComponent::eInitSuccess CScriptComponent::Init(const std::string& aScriptPath, const int aFunctionTableIndex)
{
	ComponentId id = GetId();
	if (id == NULL_COMPONENT)
	{
		return eInitSuccess::eNotRegisteredComponent;
	}

	std::ifstream scriptFile(aScriptPath);
	if (!scriptFile.good())
	{
		ourLastErrorMessage = aScriptPath;
		return eInitSuccess::eInvalidPath;
	}

	std::stringstream scriptContent;
	scriptContent << scriptFile.rdbuf();
	if (!scriptContent.good())
	{
		return eInitSuccess::eFailedToReadFile;
	}

	std::string code = "local self = (" + std::to_string(id) + "); ";
	code += scriptContent.str();

	SSlua::LuaWrapper& luaWrapper = SSlua::LuaWrapper::GetInstance();

	int type = -1;
	luaWrapper.GetGlobal("GlobalFunctionTable", &type); // 1 = function table
	if (type != LUA_TTABLE)
	{
		if (type == LUA_TNIL)
		{
			int br = 0; br++;
		}
		return eInitSuccess::eNoFunctionTable;
	}

	luaWrapper.Push<int>(aFunctionTableIndex); // 2 = function index

	bool succeeded = luaWrapper.LoadLuaString(code); // 3 = actual function
	if (!succeeded)
	{
		luaWrapper.GetLastError(ourLastErrorMessage);
		return eInitSuccess::eBadLuaCode;
	}

	myFunctionTableIndex = aFunctionTableIndex;

	luaWrapper.AssignePairToTableAt(-3);
	luaWrapper.Pop<void>();

	return eInitSuccess::eOK;
}

void CScriptComponent::Call()
{
	if (myFunctionTableIndex == -1)
	{
		return;
	}

	SSlua::LuaWrapper& luaWrapper = SSlua::LuaWrapper::GetInstance();

	int type = -1;
	luaWrapper.GetGlobal("GlobalFunctionTable", &type); // 1 = function table
	if (type != LUA_TTABLE) return;

	luaWrapper.Push<int>(myFunctionTableIndex);
	luaWrapper.GetValueInTableAt(-2);
	luaWrapper.RunLoadedCode();
	luaWrapper.Pop<void>();
}

void CScriptComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (mySubscribedComponentMessages.Find(aMessageType) != mySubscribedComponentMessages.FoundNone)
	{
	}
}

bool CScriptComponent::HandleError(const eInitSuccess aErrorCode)
{
	bool good = false;
	switch (aErrorCode)
	{
	case CScriptComponent::eInitSuccess::eOK:
		good = true;
		break;
	case CScriptComponent::eInitSuccess::eNotRegisteredComponent:
		DL_MESSAGE_BOX("Script component not registered so it cannot initialize 'self'");
		break;
	case CScriptComponent::eInitSuccess::eInvalidPath:
		DL_MESSAGE_BOX("Script component got a non existing file path: %s", ourLastErrorMessage);
		break;
	case CScriptComponent::eInitSuccess::eFailedToReadFile:
		DL_MESSAGE_BOX("Script component could not read the content of the file in to the buffer, weird error :/");
		break;
	case CScriptComponent::eInitSuccess::eBadLuaCode:
		DL_MESSAGE_BOX("Lua code in script component could not run, last error: %s", ourLastErrorMessage);
		break;
	}

	return good;
}

void CScriptComponent::Destroy()
{
}
