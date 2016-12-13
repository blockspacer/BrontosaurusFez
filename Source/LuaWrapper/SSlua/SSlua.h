// 
// 2016-07-08
#pragma once
#include <functional>

#include <map>
#include "../SSArgument/SSArgument.h"
#include "../../LuaSource/lua-5.3.3/src/lua.hpp"
//#include "../../CommonUtilities/GrowingArray.h"
#include "CommonUtilities/DynamicString.h"

#define LUA_WRAPPER SSlua::LuaWrapper::GetInstance()

struct lua_State;
struct SSArgument;

namespace SSlua
{
	typedef CU::GrowingArray<SSArgument> ArgumentList;
	typedef std::function<ArgumentList(const ArgumentList&)> LuaCallbackFunction;

	class LuaWrapper
	{
	public:
		static void DestroyIfCreated();
		static LuaWrapper& GetInstance();

		void RegisterFunction(const LuaCallbackFunction &aFunction, const std::string &aName, std::string& aHelpText);
		void RegisterEngineFunction(const LuaCallbackFunction &aFunction, const std::string &aName);

		void AddScriptPath(std::string aPath);

		void LoadCode(const std::string& aFileName);
		void RunLoadedCode();

		bool ParseLuaTable(const std::string& aScriptPath, const std::string& aTableName, std::map<std::string, SSArgument>& aTableMapOut);

		SSArgument CallLuaFunction(const std::string& aFunctionName, const ArgumentList& someArguments, const bool aShouldReturnFlag = false);

		static bool CheckArguments(const CU::DynamicString &aFunctionName,const CU::GrowingArray<eSSType> &aListOfTypes,const ArgumentList &anArgumentList); 

		void PrintDocumentation();

	private:
		LuaWrapper();
		~LuaWrapper();

		std::string myEngineTableName = "Engine";

		static void CheckAndCreate();
		static bool CheckCreated();

		void CheckError(const int luaResult) const;
		static void CheckError(const int luaResult, lua_State* aLuaState);

		static int StaticLuaVoidCallback(lua_State* aState);
		int LuaVoidCallback(lua_State* aState);

		static int LuaPrint(lua_State* aState);

		static ssLuaTable GetTable(lua_State* aState, int aIndex);
		ArgumentList CreateArgumentList(lua_State* aState);

		static LuaWrapper* ourInstance;

		lua_State* myState;

		std::map<std::string, LuaCallbackFunction> myVoidFunctions;

		std::map < std::string, std::string> myExposedFunctions;
	};

}
