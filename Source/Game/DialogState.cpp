#include "stdafx.h"
#include "DialogState.h"
#include "JsonValue.h"
#include "PollingStation.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"
#include "PostMaster/PostMaster.h"
#include "CommonUtilities.h"
#include "SpriteInstance.h"
#include "Engine.h"
#include "RenderStates.h"
#include "RenderMessages.h"
#include "Renderer.h"


CDialogState::CDialogState(StateStack& aStateStack) : State(aStateStack), myCurrentPiece(0), myHasFailed(false), myIsDone(false), myState(eStateStatus::eKeep), myCurrentTime(0), myBackground(nullptr)
{
	myCurrentDialog.Init(2);
}

CDialogState::~CDialogState()
{
}

void CDialogState::Init()
{
	CU::CJsonValue configFile;
	configFile.Parse("Json/Dialogs/config.json");
	if (configFile.Count("font") == 0)
	{
		myErrorString = "error in dialog config file: missing font";
		myHasFailed = true;
		return;
	}

	const CU::DynamicString font = configFile["font"].GetString().c_str();

	const CU::CJsonValue actorNamePosValue = configFile["actorNamePsoition"];

	myActorNameText.Init(font);
	myActorNameText.SetPosition(CU::Vector2f(actorNamePosValue.at("x").GetFloat(), actorNamePosValue.at("y").GetFloat()));

	const CU::CJsonValue positionValue = configFile["dialogPosition"];

	myDialogTextInstance.Init(font);
	myDialogTextInstance.SetPosition({ positionValue["x"].GetFloat() , positionValue["y"].GetFloat() });

	CU::CJsonValue dialogFile;
	dialogFile.Parse(PollingStation::currentDialog);

	const CU::CJsonValue dialogArray = dialogFile.at("dialog");
	if (!dialogArray.IsArray())
	{
		DL_ASSERT("Dialog is missing main array");
	}

	CU::DynamicString currentActor = "";

	for (unsigned i = 0; i < dialogArray.Size(); ++i)
	{
		const CU::CJsonValue currentDilogJsonPiece = dialogArray[i];
		if (currentDilogJsonPiece.GetType() == CU::eJsoneValueType::ARRAY)
		{
			SDialogPiece newDialogPiece;
			newDialogPiece.myCurrentActor = currentActor;

			for (int j = 0; j < currentDilogJsonPiece.Size(); ++j)
			{
				newDialogPiece.myDialogTexts.Add(currentDilogJsonPiece[j].GetString().c_str());
			}

			myCurrentDialog.Add(newDialogPiece);
		}
		else if (currentDilogJsonPiece.GetType() == CU::eJsoneValueType::OBJECT)
		{
			if (currentDilogJsonPiece.Count("actor") == 0)
			{
				myErrorString = "actor object is missing actor var";
				myHasFailed = true;
				return;
			}

			currentActor = currentDilogJsonPiece["actor"].GetString().c_str();
		}
	}

	myActorNameText.SetTextLines({ myCurrentDialog[myCurrentPiece].myCurrentActor });

	myBackground = new CSpriteInstance("Sprites/Dialog/background.dds", { 1, 1 }, { 0,0 });
}

eStateStatus CDialogState::Update(const CU::Time& aDeltaTime)
{
	const float MaxTime = 0.1;

	myCurrentTime += aDeltaTime.GetSeconds();

	if (myIsDone == false && myCurrentTime >= MaxTime)
	{
		typedef CU::GrowingArray<CU::DynamicString>::size_type textInstanceSize_T;

		CU::GrowingArray<CU::DynamicString>  printedStrings = myDialogTextInstance.GetTextLines();

		if (printedStrings.Size() == 0)
		{
			printedStrings.Add("");
		}

		textInstanceSize_T currentRow = MAX(printedStrings.Size() - 1, 0);
		textInstanceSize_T currentLetter = printedStrings[currentRow].Size();

		const SDialogPiece & currentDialogPiece = myCurrentDialog[myCurrentPiece];
		if (currentLetter >= currentDialogPiece.myDialogTexts[currentRow].Size())
		{
			++currentRow;
			currentLetter = 0;
			printedStrings.Add("");
		}

		if (currentRow >= currentDialogPiece.myDialogTexts.Size())
		{
			myIsDone = true;
			return myState;
		}

		printedStrings[currentRow] += currentDialogPiece.myDialogTexts[currentRow][currentLetter];

		myDialogTextInstance.SetTextLines(printedStrings);
		myCurrentTime = 0;
	}
	return myState;
}

void CDialogState::Render()
{
	if (myHasFailed == false)
	{
		SChangeStatesMessage* changeStateMessage = new SChangeStatesMessage();
		changeStateMessage->myBlendState = eBlendState::eNoBlend;
		changeStateMessage->myDepthStencilState = eDepthStencilState::eDefault;
		changeStateMessage->myRasterizerState = eRasterizerState::eDefault;
		changeStateMessage->mySamplerState = eSamplerState::eClamp;

		CEngine::GetInstance()->GetRenderer().AddRenderMessage(changeStateMessage);

		myBackground->Render();

		changeStateMessage = new SChangeStatesMessage();
		changeStateMessage->myBlendState = eBlendState::eAlphaBlend;
		changeStateMessage->myDepthStencilState = eDepthStencilState::eDisableDepth;
		changeStateMessage->myRasterizerState = eRasterizerState::eNoCulling;
		changeStateMessage->mySamplerState = eSamplerState::eClamp;

		CEngine::GetInstance()->GetRenderer().AddRenderMessage(changeStateMessage);

		myDialogTextInstance.Render();
		myActorNameText.Render();
	}
	else
	{
		myDialogTextInstance.SetText(myErrorString);
		myDialogTextInstance.Render();
	}
}

void CDialogState::OnEnter(const bool aLetThroughRender)
{
	DL_PRINT("Entered dialog");
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyboardMessage, 8);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eMouseMessage, 8);
}

void CDialogState::OnExit(const bool /*aLetThroughRender*/)
{
	DL_PRINT("Exited dialog");
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
}

void CDialogState::ClearScreen()
{
	myDialogTextInstance.SetText("");
	myActorNameText.SetText("");
}

eMessageReturn CDialogState::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

bool CDialogState::GetLetThroughRender() const
{
	return true;
}

bool CDialogState::GetLetThroughUpdate() const
{
	return true;
}

void CDialogState::SetSatus(const eStateStatus aStateStatus)
{
	myState = aStateStatus;
}

void CDialogState::Next()
{
	if (myIsDone == false)
	{
		myDialogTextInstance.SetTextLines(myCurrentDialog[myCurrentPiece].myDialogTexts);
		myIsDone = true;
	}
	else
	{
		ClearScreen();
		++myCurrentPiece;
		unsigned short dialogSize = myCurrentDialog.Size();

		if (myCurrentPiece < dialogSize)
		{
			myActorNameText.SetTextLines({ myCurrentDialog[myCurrentPiece].myCurrentActor });
			myDialogTextInstance.SetTextLines({ "" });
			myIsDone = false;
		}
		else
		{
			myState = eStateStatus::ePop;
		}
	}
}
