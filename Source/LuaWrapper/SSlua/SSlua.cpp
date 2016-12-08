#include "LuaWrapper\Precompiled\stdafx.h"
#include "stdafx.h"
#include "SSlua.h"
#include "../SSArgument/SSArgument.h"
#include <codecvt>
#include <Windows.h>

//extern "C"
//{
//#include "../../LuaSource/lua-5.3.3/src/lua.h"
//#include "../../LuaSource/lua-5.3.3/src/lauxlib.h"
//#include "../../LuaSource/lua-5.3.3/src/lualib.h"
//}


#define ERROR_MESSAGE(message) MessageBeep(MB_ICONERROR); MessageBox(nullptr, message, L"ERROR", MB_OK | MB_ICONERROR);

std::wstring stringToWstring(const std::string& t_str)
{
	//setup converter
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;

	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	return converter.from_bytes(t_str);
}

namespace SSlua
{
	LuaWrapper* LuaWrapper::ourInstance = nullptr;

	void LuaWrapper::CheckAndCreate()
	{
		if (CheckCreated() == false)
		{
			//if (CheckCreated() == false)
			//{
				ourInstance = new LuaWrapper();
				assert(ourInstance != nullptr && "Failed to create Lua Wrapper");
			//}
		}
	}

	void LuaWrapper::DestroyIfCreated()
	{
		if (CheckCreated() == true)
		{
			delete ourInstance;
			ourInstance = nullptr;
		}
	}

	bool LuaWrapper::CheckCreated()
	{
		return ourInstance != nullptr;
	}

	LuaWrapper& LuaWrapper::GetInstance()
	{
		CheckAndCreate();
		return *ourInstance;
	}

	void LuaWrapper::RegisterFunction(const LuaCallbackFunction &aFunction,const std::string& aName)
	{
		myVoidFunctions[aName] = aFunction;

		lua_pushstring(myState, aName.c_str());
		lua_pushcclosure(myState, StaticLuaVoidCallback, 1);
		lua_setglobal(myState, aName.c_str());
	}

	void LuaWrapper::RegisterEngineFunction(const LuaCallbackFunction& aFunction, const std::string& aName)
	{
		std::string tempName = myEngineTableName + "." + aName;

		lua_getglobal(myState, myEngineTableName.c_str());
		if (lua_type(myState, -1) != LUA_TTABLE)
		{
			lua_pop(myState, 1);
			lua_newtable(myState);
		}

		myVoidFunctions[tempName] = aFunction;

		lua_pushstring(myState, tempName.c_str());
		lua_pushcclosure(myState, StaticLuaVoidCallback, 1);
		lua_setfield(myState, 1, aName.c_str());
		lua_setglobal(myState, myEngineTableName.c_str());
	}

	void LuaWrapper::AddScriptPath(std::string aPath)
	{
		lua_getglobal(myState, "package");
		lua_getfield(myState, -1, "path");
		std::string currentPath = lua_tostring(myState, -1);
		(currentPath += ";") += aPath + "/?.lua";
		lua_pop(myState, 1);
		lua_pushstring(myState, currentPath.c_str());
		lua_setfield(myState, -2, "path");
		lua_pop(myState, 1);
	}

	void LuaWrapper::LoadCode(const std::string & aFileName)
	{
		CheckError(luaL_loadfile(myState, aFileName.c_str()));
	}

	void LuaWrapper::RunLoadedCode()
	{
		CheckError(lua_pcall(myState, 0, LUA_MULTRET, 0));
	}

	bool LuaWrapper::ParseLuaTable(const std::string& aScriptPath, const std::string& aTableName, std::map<std::string, SSArgument>& aTableMapOut)
	{
		LoadCode(aScriptPath);
		RunLoadedCode();

		lua_getglobal(myState, aTableName.c_str());
		if (lua_istable(myState, -1) == false)
		{
			DL_MESSAGE_BOX("Failed to get lua table %s in file %s", aTableName.c_str(), aScriptPath.c_str());
			DL_ASSERT("Failed to get lua table %s in file %s", aTableName.c_str(), aScriptPath.c_str());
		}
		
		lua_pushnil(myState);

		bool result = true;
		while (lua_next(myState, -2))
		{
			if (lua_isstring(myState, -2) == true)
			{
				std::string key = lua_tostring(myState, -2);
				aTableMapOut[key] = SSArgument();

				switch (lua_type(myState, -1))
				{
				case LUA_TNUMBER:
					aTableMapOut[key] = SSArgument(lua_tonumber(myState, -1));
					break;
				case LUA_TSTRING:
					aTableMapOut[key] = SSArgument(luaL_tolstring(myState, -1, nullptr));
					break;
				case LUA_TBOOLEAN:
					aTableMapOut[key] = SSArgument(lua_toboolean(myState, -1) != 0);
					break;
				default:
					DL_ASSERT("Lua type is at the moment incompatible with Engine or at least this method");
					result = false;
					break;
				}
			}
			else
			{
				DL_ASSERT("Error in file %s, table %s: Lua key must be string", aScriptPath, aTableName);
				result = false;
			}

			lua_pop(myState, 2);
		}

		lua_pop(myState, 1);

		return result;
	}

	SSArgument LuaWrapper::CallLuaFunction(const std::string& aFunctionName, const ArgumentList& someArguments, const bool aShouldReturnFlag)
	{
		lua_getglobal(myState, aFunctionName.c_str());
		
		for (unsigned i = 0; i < someArguments.Size(); ++i)
		{
			const SSArgument& explainingArgument = someArguments[i];
			switch (explainingArgument.GetType())
			{
			case eSSType::NUMBER:
				lua_pushnumber(myState, explainingArgument.GetNumber());
				break;
			case eSSType::STRING: 
				lua_pushstring(myState, explainingArgument.GetString());
				break;
			case eSSType::BOOL: 
				lua_pushboolean(myState, explainingArgument.GetBool());
				break;
			case eSSType::NIL: break;
			default: break;
			}
		}

		int numberOfreturnValues = 0;
		if (aShouldReturnFlag == true)
		{
			numberOfreturnValues = 1;
		}

		const int result = lua_pcall(myState, someArguments.Size(), numberOfreturnValues,0);

		if (result != NULL)
		{
			std::string errorString = "error running function " + aFunctionName + " : " + lua_tostring(myState, -1);
			ERROR_MESSAGE(stringToWstring(errorString).c_str());
		}

		if (numberOfreturnValues == 0)
		{
			return SSArgument();
		}

		switch (lua_type(myState, -1))
		{
		case LUA_TNUMBER:
			return  SSArgument(lua_tonumber(myState,-1));
		case LUA_TSTRING:
			return  SSArgument(luaL_tolstring(myState, -1, nullptr));
		case LUA_TBOOLEAN:
			return  SSArgument(lua_toboolean(myState, -1) != 0);
		case LUA_TNIL:
			break;
		default:
			DL_ASSERT("Lua type is at the moment incompatible with Engine or at least this method");
			return  SSArgument();
		}

		return SSArgument();
	}

	bool LuaWrapper::CheckArguments(const CU::DynamicString& aFunctionName, const CU::GrowingArray<eSSType>& aListOfTypes, const ArgumentList& anArgumentList)
	{
		if (aListOfTypes.Size() > anArgumentList.Size())
		{
			CU::DynamicString errorMsg = CU::DynamicString("ERROR! Lua C function ") + aFunctionName + " got " + anArgumentList.Size() + "arguments expected " + aListOfTypes.Size();
			errorMsg += " ( ";
			for (unsigned int i = 0; i < aListOfTypes.Size(); ++i)
			{
				if (i != 0)
				{
					errorMsg += " , ";
				}

				errorMsg += SSArgument::GetTypeName(aListOfTypes[i]);
			}
			errorMsg += " )";

			ERROR_MESSAGE(stringToWstring(errorMsg.c_str()).c_str());
			DL_ASSERT(errorMsg.c_str());
			return false;
		}

		for (unsigned int i = 0; i < aListOfTypes.Size(); ++i)
		{
			if (aListOfTypes[i] != anArgumentList[i].GetType())
			{
				CU::DynamicString errorMsg = CU::DynamicString("ERROR! Lua C function ") + aFunctionName + " got a " + anArgumentList[i].GetTypeName() +" as argument nr "+ i + 1 +" expected a " + SSArgument::GetTypeName(aListOfTypes[i]);
				ERROR_MESSAGE(stringToWstring(errorMsg.c_str()).c_str());
				DL_ASSERT(errorMsg.c_str());
				return false;
			}
		}
		return true;
	}

	LuaWrapper::LuaWrapper()
	{
		myState = luaL_newstate();
		luaL_openlibs(myState);

		lua_pushcfunction(myState, LuaPrint);
		lua_setglobal(myState, "print");
		//lua_pop(myState, 1);
	}

	LuaWrapper::~LuaWrapper()
	{
	}

	void LuaWrapper::CheckError(const int luaResult) const
	{
		CheckError(luaResult, myState);
	}

	void LuaWrapper::CheckError(const int luaResult, lua_State* aLuaState)
	{
		if (luaResult != LUA_OK)
		{
			std::string message = lua_tostring(aLuaState, -1);
			lua_pop(aLuaState, 1);
			//DL_ASSERT(false, message.c_str());
			ERROR_MESSAGE(stringToWstring(message).c_str());
			exit(0);
		}
	}

	int LuaWrapper::StaticLuaVoidCallback(lua_State* aState)
	{
		return GetInstance().LuaVoidCallback(aState);
	}

	int LuaWrapper::LuaVoidCallback(lua_State* aState)
	{
		std::string functionName = lua_tostring(aState, lua_upvalueindex(1));

		ArgumentList argumentList = CreateArgumentList(aState);

		const ArgumentList returnArguments = myVoidFunctions[functionName](argumentList);

		for (unsigned int i = 0; i < returnArguments.Size(); i++)
		{
			switch (returnArguments[i].GetType())
			{
			case eSSType::NUMBER: 
				lua_pushnumber(aState, returnArguments[i].GetNumber());
				break;
			case eSSType::STRING:
				lua_pushstring(aState, returnArguments[i].GetString());
				break;
			case eSSType::BOOL:
				lua_pushboolean(aState, returnArguments[i].GetBool());
				break;
			case eSSType::NIL:
				lua_pushnil(aState);
				break;
			default: break;
			}
		}

		return returnArguments.Size();
	}

	int LuaWrapper::LuaPrint(lua_State* aState)
	{
		int numberOfArguments = lua_gettop(aState);

		for (int i = 0; i < numberOfArguments; ++i)
		{
			if (lua_isstring(aState, i + 1))
			{
				const char * explainingString = lua_tostring(aState, i +1 );
#ifndef RESOURCES_LOG
				DL_PRINT(explainingString);
#else
				RESOURCES_LOG(explainingString);
#endif
			}
			else
			{
				MessageBeep(MB_ICONERROR);
				MessageBox(nullptr, L"lua Can only print strings at the moment. sorry :( use tostring or something", L"lua error", MB_OK | MB_ICONERROR);
				DL_ASSERT("lua Can only print strings at the moment. sorry :( use tostring or something");
			}
		}
		return 0;
	}

	ssLuaTable LuaWrapper::GetTable(lua_State* aState, int aIndex)
	{
		ssLuaTable tempLuaTable;
		lua_pushnil(aState);
		while (lua_next(aState, aIndex) != 0)
		{
			SSArgument tempArgument;
			switch (lua_type(aState, -1))
			{
			case LUA_TNUMBER:
				tempArgument = SSArgument(lua_tonumber(aState, -1));
				break;
			case LUA_TSTRING:
				tempArgument = SSArgument(luaL_tolstring(aState, -1, nullptr));
				break;
			case LUA_TBOOLEAN:
				tempArgument = SSArgument(lua_toboolean(aState, -1) != 0);
				break;
			case LUA_TNIL:
				break;
			case LUA_TTABLE:
				tempArgument = SSArgument(GetTable(aState, -1));
				break;
			default:
				DL_ASSERT("Lua type is at the moment incompatible with Engine or at least this method");
			}
			if (lua_isstring(aState, -2))
			{
				tempLuaTable[lua_tolstring(aState, -2, nullptr)] = tempArgument;
			}
			else if (lua_isnumber(aState, -2))
			{
				std::string key;
				key += lua_tointeger(aState, -2);
				tempLuaTable[key] = tempArgument;
			}
			lua_pop(aState, 1);
		}

		return tempLuaTable;
	}

	ArgumentList LuaWrapper::CreateArgumentList(lua_State* aState)
	{
		ArgumentList arguments;
		arguments.Init(1);

		const int nArgs = lua_gettop(aState);

		if (nArgs == 0)
		{
			return arguments;
		}

		for (int i = 1; i < nArgs + 1; i++)
		{
			SSArgument tempArgument;
			switch (lua_type(aState, i))
			{
			case LUA_TNUMBER:
				tempArgument = SSArgument(lua_tonumber(aState, i));
				break;
			case LUA_TSTRING:
				tempArgument = SSArgument(luaL_tolstring(aState, i, nullptr));
				break;
			case LUA_TBOOLEAN:
				tempArgument = SSArgument(lua_toboolean(aState, i) != 0);
				break;
			case LUA_TNIL:
				break;
			case LUA_TTABLE:
				tempArgument = SSArgument(GetTable(aState, i));
				break;
			default:
				DL_ASSERT("Lua type is at the moment incompatible with Engine or at least this method");
			}
			arguments.Add(tempArgument);
		}

		return arguments;
	}
}
