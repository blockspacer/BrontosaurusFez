#pragma once
#include "Component.h"
//#include "..\PostMaster\Subscriber.h"

class CGameObject;
namespace Stats
{
	struct SBonusStats;	

};
class HatActivator;

struct SHat
{
	Stats::SBonusStats* stat;
	CGameObject* gameObject; // GameObj hatObj  -> AddComp(statComp)
	HatActivator* hatActivator;
	const char* skillname;
};

class CInventoryComponent : public CComponent//, public Subscriber
{
public:
	CInventoryComponent();
	~CInventoryComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
//	eMessageReturn Receive(const Message& aMessage) override;
	void Destroy() override;
	void AddHat(SHat& aHat);
	void AddGold(unsigned int aAmountOfGold);

private:
	CU::GrowingArray<SHat> myHats; //byt till stat component.
	unsigned int myGold;
};

