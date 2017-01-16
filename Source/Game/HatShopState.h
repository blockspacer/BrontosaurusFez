#pragma once
#include "StateStack/State.h"
#include "PostMaster/Subscriber.h"
#include "CommonUtilities/GrowingArray.h"
#include <string>

struct SShopSelection
{
	typedef unsigned short Cost;
	std::string HatName;
	Cost myCost;

	const bool operator == (SShopSelection aRight)
	{
		for (unsigned int i = 0; i < HatName.length(); ++i)
		{
			if (HatName[i] != aRight.HatName[i])
			{
				return false;
			}
		}
		if (myCost != aRight.myCost)
		{
			return false;
		}
		return true;
	}
	void operator = (SShopSelection aRight)
	{
		HatName = aRight.HatName;
		myCost = aRight.myCost;
	}
};

namespace GUI
{
	class GUIManager;
}

class HatShopState : public State, public Subscriber
{
public:
	HatShopState(StateStack& aStateStack);
	~HatShopState();

	void CloseShop();

	// Inherited via State
	void Init() override;
	eStatus Update(const CU::Time & aDeltaTime) override;
	void Render() override;
	void OnEnter() override;
	void OnExit() override;
	void ValidatePurchase();
	bool GetLetThroughRender() const override;

	// Inherited via Subscriber
	eMessageReturn Recieve(const Message & aMessage) override;
private:
	CU::GrowingArray<SShopSelection> mySelections;
	GUI::GUIManager* myGUIManager;
	SShopSelection* myCurrentlySelected;
	bool myIsInitied;
};