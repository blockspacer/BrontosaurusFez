#pragma once
#include "ShopStructs.h"
#include "StateStack/State.h"
#include "PostMaster/Subscriber.h"
#include "CommonUtilities/GrowingArray.h"
#include "CommonUtilities/vector2.h"

class CTextInstance;

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
	void SetSelected(const char aIndex);
	bool GetLetThroughRender() const override;
	bool GetLetThroughUpdate() const override;


	// Inherited via Subscriber
	eMessageReturn Recieve(const Message & aMessage) override;
private:
	void AdjustText();
	CU::GrowingArray<SShopSelection*> mySelections;
	//temp array of texts so that you can see what you wanna buy
	CU::GrowingArray<CTextInstance*> myOptionsText;
	CU::GrowingArray<CTextInstance*> myCostText;
	GUI::GUIManager* myGUIManager;
	SShopSelection* myCurrentlySelected;
	CU::Vector2f myWindowPosition;
	bool myIsInitied;
};