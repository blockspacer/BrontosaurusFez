#pragma once
#include <map>
#include "../LuaWrapper/SSlua/SSlua.h"
#include "../CommonUtilities/GrowingArray.h"
#include "../PostMaster/Subscriber.h"

namespace CU
{
	enum class eKeys;
}
class CTextInstance;
class CConsole : public Subscriber
{
public:
	CConsole();
	~CConsole();

	void Init();
	void GetLuaFunctions();
	void Activate();
	void Deactivate();
	eMessageReturn Recieve(const Message & aMessage) override;
	eMessageReturn TakeKeyBoardInputPressedChar(const char aKey);
	

	void Update(float aDeltaTime);
	void Render();

private:
	void UpdateCommandSuggestions(const std::string& aStringToCompare);
	size_t CConsole::MakeCommandSuggestions(const std::string& aStringToCompare, const std::string& aStringToEvaluate);
	//const CU::DynamicString CheckIfTextIsCommand(const CU::DynamicString& aText);


private:
	std::map<std::string, SSlua::LuaCallbackFunction> myLuaFunctions;
	CU::GrowingArray<CTextInstance*> myTextLog;
	CTextInstance* myCurrentText;
	std::string mySuggestedCommand;

	float myElapsedAnimationTimer;
	float myAnimationTimerCooldown;
	bool myHaveIAfterCurrentText;
	bool myIsActive;
};

