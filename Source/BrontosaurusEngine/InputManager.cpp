#include "stdafx.h"
#include "InputManager.h"

#include "Engine.h"
#include "WindowsWindow.h"

#include "../CommonUtilities/EKeyboardKeys.h"
#include "../CommonUtilities/EMouseButtons.h"
#include "../CommonUtilities/EInputMessage.h"
#include "../CommonUtilities/InputWrapper.h"
#include "../CommonUtilities/XInputWrapper.h"
#include "../CommonUtilities/vector2.h"

#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/MouseClicked.h"
#include "../PostMaster/MouseReleased.h"
#include "../PostMaster/MouseMoved.h"
#include "../PostMaster/KeyPressed.h"
#include "../PostMaster/KeyReleased.h"
#include "../PostMaster//InputMessagePressed.h"

CInputManager::CInputManager()
	: myDInputWrapper(nullptr)
	, myXInputWrapper(nullptr)
{
	myDInputWrapper = new CU::InputWrapper();
	void* hingsten = CEngine::GetInstance()->GetWindow()->GetHinstance(),
		* hunden = CEngine::GetInstance()->GetWindow()->GetHWND();

	/*bool directInputSuccess =*/ myDInputWrapper->Init(reinterpret_cast<HINSTANCE>(hingsten), reinterpret_cast<HWND>(hunden));
	//assert(directInputSuccess == true && "Failed to init direct input wrapper :(");

	myXInputWrapper = new CU::XInputWrapper();
	myXInputWrapper->Init(4);

	myDInputWrapper->Init(reinterpret_cast<HINSTANCE>(CEngine::GetInstance()->GetWindow()->GetHinstance()), reinterpret_cast<HWND>(CEngine::GetInstance()->GetWindow()->GetHWND()));
	myKeyDowns.Init(20);

	myHasFocus = true;

	PostMaster::GetInstance().Subscribe(this, eMessageType::eFokusChanged);
}

CInputManager::~CInputManager()
{
	SAFE_DELETE(myDInputWrapper);
	SAFE_DELETE(myXInputWrapper);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eFokusChanged);
}

eMessageReturn CInputManager::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

void CInputManager::SetMousePosition(const CU::Vector2f& aMousePosition)
{
	myDInputWrapper->SetMousePosition(static_cast<int>(aMousePosition.x), static_cast<int>(aMousePosition.y));
}

void CInputManager::LockUnlockMouse(const bool aHasFocus)
{
	myHasFocus = aHasFocus;
	ShowCursor(!aHasFocus);
}

void CInputManager::Update()
{
	myDInputWrapper->Update();
	myXInputWrapper->UpdateStates();

	UpdateMouse();
	UpdateKeyboard();
	UpdateGamePad();
}

void CInputManager::UpdateMouse()
{
	if (myHasFocus == true)
	{
		static CU::Vector2f lastMousePosition(0, 0);
		CU::Vector2f mousePosition(myDInputWrapper->GetMousePositionX(), myDInputWrapper->GetMousePositionY());

		if (lastMousePosition != mousePosition)
		{
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eMouseMessage, MouseMoved(mousePosition)));
			lastMousePosition = mousePosition;
		}
		if (myDInputWrapper->IsMouseButtonPressed(CU::eMouseButtons::LBUTTON) == true)
		{
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eMouseMessage, MouseClicked(mousePosition, CU::eMouseButtons::LBUTTON)));
		}
		if (myDInputWrapper->IsMouseButtonReleased(CU::eMouseButtons::LBUTTON) == true)
		{
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eMouseMessage, MouseReleased(mousePosition, CU::eMouseButtons::LBUTTON)));
		}
		if (myDInputWrapper->IsMouseButtonPressed(CU::eMouseButtons::RBUTTON) == true)
		{
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eMouseMessage, MouseClicked(mousePosition, CU::eMouseButtons::RBUTTON)));
		}
		if (myDInputWrapper->IsMouseButtonReleased(CU::eMouseButtons::RBUTTON) == true)
		{
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eMouseMessage, MouseReleased(mousePosition, CU::eMouseButtons::RBUTTON)));
		}
		//myDInputWrapper->SetMousePosition(CEngine::GetInstance()->GetWindowSize().x / 2, CEngine::GetInstance()->GetWindowSize().y / 2);
	}
	
}

void CInputManager::UpdateKeyboard()
{
	if (myDInputWrapper->GetKeysPressed(myKeyDowns) == true)
	{
		for (unsigned int i = 0; i < myKeyDowns.Size(); ++i)
		{
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eKeyboardMessage, KeyPressed(myKeyDowns[i])));
			//PostMaster::GetInstance().SendLetter(Message(eMessageType::eKeyPressed, KeyPressed(myKeyDowns[i])));
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eInputMessagePressed, InputMessagePressed(static_cast<CU::eInputMessage>(myKeyDowns[i]))));
		}
	}

	if (myDInputWrapper->GetKeysReleased(myKeyDowns) == true)
	{
		for (unsigned int i = 0; i < myKeyDowns.Size(); ++i)
		{
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eKeyboardMessage, KeyReleased(myKeyDowns[i])));
		}
	}
}

void CInputManager::UpdateGamePad()
{
	for (unsigned i = 0; i < myXInputWrapper->GetConnectedJoystickCount(); ++i)
	{
		if (myXInputWrapper->GetKeyPressed(0, CU::XInputWrapper::GAMEPAD::A) == true)
		{
			DL_PRINT("'a' pressed!");
		}
	}
}
