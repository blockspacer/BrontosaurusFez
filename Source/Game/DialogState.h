#pragma once
#include "StateStack/State.h"
#include "DynamicString.h"
#include "GrowingArray.h"
#include "../BrontosaurusEngine/TextInstance.h"
#include "PostMaster/Subscriber.h"

struct SDialogPiece
{
	SDialogPiece() { myDialogTexts.Init(2); }
	CU::DynamicString myCurrentActor;

	CU::GrowingArray<CU::DynamicString> myDialogTexts;
};

class CDialogState: public State, Subscriber
{
public:
	CDialogState(StateStack& aStateStack);
	~CDialogState();

	void Init() override;
	eStateStatus Update(const CU::Time& aDeltaTime) override;
	void Render() override;

	void OnEnter() override;
	void OnExit() override;

	void ClearScreen();

	eMessageReturn Recieve(const Message& aMessage) override;

	void Next();
private:
	CU::GrowingArray<SDialogPiece, unsigned short> myCurrentDialog;
	int myCurrentPiece;

	CTextInstance myDialogTextInstance;
	CTextInstance myActorNameText;

	CU::DynamicString myErrorString;
	bool myHasFailed;
	bool myIsDone;
	eStateStatus myState;
	CU::TimeUnit myCurrentTime;
};

