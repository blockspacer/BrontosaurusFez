#include "stdafx.h"
#include "DialogState.h"
#include "JsonValue.h"
#include "PollingStation.h"


CDialogState::CDialogState(StateStack& aStateStack): State(aStateStack)
{
}

CDialogState::~CDialogState()
{
}

void CDialogState::Init()
{
	CU::CJsonValue dialogFile;
	dialogFile.Parse(PollingStation::currentDialog);

	const CU::CJsonValue dialogArray = dialogFile.at("dialog");
	if (! dialogArray.IsArray())
	{
		DL_ASSERT("Dialog is missing main array");
	}

}
