#pragma once
#include "StateStack/State.h"
#include "DynamicString.h"
#include "GrowingArray.h"

struct SDialogPiece
{
	CU::DynamicString myCurrentActor;

	CU::GrowingArray<CU::GrowingArray<CU::DynamicString, unsigned short>, unsigned short> myDialogTexts;
};

class CDialogState: public State
{
public:
	CDialogState(StateStack& aStateStack);
	~CDialogState();

	void Init() override;
	eStateStatus Update(const CU::Time& aDeltaTime) override;
	void Render() override;

	void OnEnter();
	void OnExit();

private:
	CU::GrowingArray<SDialogPiece, unsigned short> myCurrentDialog;

};

