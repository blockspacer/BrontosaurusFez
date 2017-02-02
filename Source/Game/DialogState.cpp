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

	CU::DynamicString currentActor = "";

	for (unsigned i = 0; i < dialogArray.Size(); ++i)
	{
		CU::CJsonValue currentDilogJsonPiece = dialogArray[i];
		if (currentDilogJsonPiece.GetType() == CU::eJsoneValueType::ARRAY)
		{
			SDialogPiece newDialogPiece;
			newDialogPiece.myCurrentActor = currentActor;

			for (int j = 0; j < currentDilogJsonPiece.Size(); ++j)
			{
				//CU::DynamicString dialogLine;
				//dialogLine = currentDilogJsonPiece[i].GetString();
				//newDialogPiece.myDialogTexts.Add(dialogLine);
			}
		}
	}
}
