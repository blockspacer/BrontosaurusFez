#include "stdafx.h"
#include "GUIManager.h"

#include <WidgetContainer\WidgetContainer.h>
#include <WidgetFactory\WidgetFactory.h>
#include <GUICursor\GUICursor.h>

#include "../PostMaster\PostMaster.h"
#include "../PostMaster\Event.h"
#include "../PostMaster\Message.h"

#include "BrontosaurusEngine\Engine.h"
#include "BrontosaurusEngine\Renderer.h"

#include "../Audio/AudioInterface.h"

static CU::Vector2f locMousePosition;

GUI::GUIManager::GUIManager(const bool aShouldStealInput)
	: myWidgetContainer(nullptr)
	, myFocusedWidget(nullptr)
	, myWidgetAtMouse(nullptr)
	, myCursor(nullptr)
	, myShouldUpdate(true)
	, myShouldRender(true)
	, myShouldStealInput(aShouldStealInput)
	, myCamera(nullptr)
	, myShouldRenderMouse(true)
{
}

GUI::GUIManager::~GUIManager()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::ePlayerGotNewWeapon);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::ePlayerChangedWeapon);

	myFocusedWidget = nullptr;
	myWidgetAtMouse = nullptr;
	SAFE_DELETE(myWidgetContainer);

	PostMaster::GetInstance().UnSubscribe(myCursor, eMessageType::eMouseMessage);
	SAFE_DELETE(myCursor);
	SAFE_DELETE(myCamera);
}

void GUI::GUIManager::Init()
{
	myWidgetContainer = new WidgetContainer(CU::Vector2f::Zero, CU::Vector2f::One, "BaseWidget", true);

	myCursor = new GUICursor();
	PostMaster::GetInstance().AppendSubscriber(myCursor, eMessageType::eMouseMessage);
}

void GUI::GUIManager::Init(const char* aGUIScenePath, bool isOld)
{
	myWidgetContainer = WidgetFactory::CreateGUIScene(aGUIScenePath, isOld, myCamera);

	myCursor = new GUICursor();
	PostMaster::GetInstance().AppendSubscriber(myCursor, eMessageType::eMouseMessage);


	Widget* retryButton = myWidgetContainer->FindWidget("buttonRetry");
	if (retryButton != nullptr)
	{
		retryButton->SetVisibility(false);
	}
}

//void GUI::GUIManager::Init(const char* aLuaScriptPath)
//{
//	myWidgetContainer = WidgetFactory::CreateGUIScene(aLuaScriptPath, myCamera);
//}

void GUI::GUIManager::Update(const CU::Time& aDeltaTime)
{
	if (myShouldUpdate == true)
	{
		myWidgetContainer->Update(aDeltaTime);

		locMousePosition = myCursor->GetPosition();
	}
}

void GUI::GUIManager::Render()
{
	SChangeStatesMessage* changeStateMessage = new SChangeStatesMessage();
	changeStateMessage->myDepthStencilState = eDepthStencilState::eDefault; //if mouse is under buttons, this is the problem
	changeStateMessage->myRasterizerState = eRasterizerState::eDefault;
	changeStateMessage->myBlendState = eBlendState::eAlphaBlend;
	changeStateMessage->mySamplerState = eSamplerState::eClamp;
	RENDERER.AddRenderMessage(changeStateMessage);

	CU::Camera* previousCamera = CEngine::GetInstance()->GetCamera();
	SSetCameraMessage* setCameraMessage = new SSetCameraMessage();
	setCameraMessage->myCamera = *myCamera;
	RENDERER.AddRenderMessage(setCameraMessage);

	myWidgetContainer->Render();
	if (myShouldStealInput == true)
	{
		//changeStateMessage = new SChangeStatesMessage()
		if (myShouldRenderMouse == true)
		{
			myCursor->Render();
		}
	}

	setCameraMessage = new SSetCameraMessage();
	setCameraMessage->myCamera = *previousCamera;
	RENDERER.AddRenderMessage(setCameraMessage);
}

void GUI::GUIManager::AddWidget(const CU::DynamicString& aName, Widget* aWidget)
{
	if (aName.Empty() == true)
	{
		DL_ASSERT("Tried to add widget without a name to GUI manager");
		return;
	}
	if (aWidget == nullptr)
	{
		DL_ASSERT("Tried to add null-widget to GUI manager");
		return;
	}

	myWidgetContainer->AddWidget(aName, aWidget);
}

void GUI::GUIManager::DeleteWidget(const CU::DynamicString& aName)
{
	Widget* widget = myWidgetContainer->RemoveWidget(aName);
	SAFE_DELETE(widget);
}

GUI::Widget* GUI::GUIManager::RemoveWidget(const CU::DynamicString& aName)
{
	if (aName.Empty() == true)
	{
		DL_ASSERT("Tried to remove widget with empty name in GUI manager");
		return nullptr;
	}

	return myWidgetContainer->RemoveWidget(aName);
}

GUI::Widget* GUI::GUIManager::FindWidget(const CU::DynamicString& aName)
{
	if (aName.Empty() == true)
	{
		DL_ASSERT("Tried to remove widget with empty name in GUI manager");
		return nullptr;
	}

	return myWidgetContainer->FindWidget(aName);
}

eMessageReturn GUI::GUIManager::MouseClicked(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition)
{
	SUPRESS_UNUSED_WARNING(aMousePosition);
	CU::Vector2f mousePosition = myCursor->GetPosition();

	Widget* widget = myWidgetContainer->MouseIsOver(mousePosition);

	if (myFocusedWidget != nullptr && myFocusedWidget != widget)
	{
		myFocusedWidget->OnLostFocus();
	}

	myFocusedWidget = widget;

	if (myFocusedWidget == nullptr || myFocusedWidget == myWidgetContainer)
	{
		return eMessageReturn::eContinue;
	}
	else
	{
		//Audio::CAudioInterface::GetInstance()->PostEvent("ButtonClick");
		myFocusedWidget->OnGotFocus();
		myFocusedWidget->OnMousePressed(mousePosition, aMouseButton);

		return (myShouldStealInput == true) ? eMessageReturn::eStop : eMessageReturn::eContinue;
	}
}

eMessageReturn GUI::GUIManager::MouseReleased(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition)
{
	SUPRESS_UNUSED_WARNING(aMousePosition);
	CU::Vector2f mousePosition = myCursor->GetPosition();

	Widget* widget = myWidgetContainer->MouseIsOver(mousePosition);
	bool stoleInput = false;

	if (myFocusedWidget != nullptr && myFocusedWidget != widget)
	{
		stoleInput = true;
		myFocusedWidget->OnMouseReleased(mousePosition, aMouseButton);
	}
	if (widget != nullptr && widget != myWidgetContainer)
	{
		stoleInput = true;
		widget->OnMouseReleased(mousePosition, aMouseButton);
	}

	return (myShouldStealInput == true && stoleInput == true) ? eMessageReturn::eStop : eMessageReturn::eContinue;
}

eMessageReturn GUI::GUIManager::MouseMoved(const CU::Vector2f& aMousePosition)
{
	SUPRESS_UNUSED_WARNING(aMousePosition);
	CU::Vector2f mousePosition = myCursor->GetPosition();

	Widget* widget = myWidgetContainer->MouseIsOver(mousePosition);

	if (widget == myWidgetAtMouse)
	{
		if (myWidgetAtMouse != nullptr && myWidgetAtMouse != myWidgetContainer)
		{
			myWidgetAtMouse->OnMouseMove(mousePosition);
		}
	}
	else
	{
		if (myWidgetAtMouse != nullptr && myWidgetAtMouse != myWidgetContainer)
		{
			myWidgetAtMouse->OnMouseExit(mousePosition);
		}
		if (widget != nullptr && widget != myWidgetContainer)
		{
			//Audio::CAudioInterface::GetInstance()->PostEvent("ButtonHover");
			widget->OnMouseEnter(mousePosition);
		}

		myWidgetAtMouse = widget;
	}

	return eMessageReturn::eContinue;
}

eMessageReturn GUI::GUIManager::MouseDragged(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition, const CU::Vector2f& aLastMousePosition)
{
	Widget* widget = myWidgetContainer->MouseIsOver(aMousePosition);
	if (widget != nullptr && widget != myWidgetContainer)
	{
		widget->OnMouseDrag(aMousePosition - aLastMousePosition, aMouseButton);
		return (myShouldStealInput == true) ? eMessageReturn::eStop : eMessageReturn::eContinue;
	}

	return eMessageReturn::eContinue;
}

eMessageReturn GUI::GUIManager::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

static const CU::Vector3f inactiveMainMenuOffset(0.f, -10.f, 0.f);
#include "RenderableWidgets\ModelWidget\ModelWidget.h" //last day fulhax :)
void GUI::GUIManager::SetSkipEmissive(const bool aShouldSkipEmissive)
{
	ModelWidget* button = nullptr;
	float floatBool = aShouldSkipEmissive ? 1.f : 0.f;

	button = static_cast<ModelWidget*>(myWidgetContainer->FindWidget("ButtonStart"));
	button->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eSkipEmissive] = floatBool;
	button->SetInactivePosition(inactiveMainMenuOffset * floatBool);

	button = static_cast<ModelWidget*>(myWidgetContainer->FindWidget("buttonCredits"));
	button->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eSkipEmissive] = floatBool;
	button->SetInactivePosition(inactiveMainMenuOffset * floatBool);

	button = static_cast<ModelWidget*>(myWidgetContainer->FindWidget("buttonQuit"));
	button->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eSkipEmissive] = floatBool;
	button->SetInactivePosition(inactiveMainMenuOffset * floatBool);

	button = static_cast<ModelWidget*>(myWidgetContainer->FindWidget("buttonLevelSelect"));
	button->GetPixelConstantBufferStruct().myValues[SPixelConstantBuffer::eSkipEmissive] = floatBool;
	button->SetInactivePosition(inactiveMainMenuOffset * floatBool);
}

void GUI::GUIManager::PauseRenderAndUpdate()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::ePlayerGotNewWeapon);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::ePlayerChangedWeapon);
	PostMaster::GetInstance().UnSubscribe(myCursor, eMessageType::eMouseMessage);

	myShouldUpdate = false;
	myShouldRender = false;

	if (myWidgetContainer != nullptr)
	{
		myWidgetContainer->SetVisibility(false);
		myWidgetContainer->RemoveDebugLines();
	}
}

void GUI::GUIManager::RestartRenderAndUpdate()
{
	myShouldUpdate = true;
	myShouldRender = true;

	myWidgetContainer->SetVisibility(true);

	PostMaster::GetInstance().InsertSubscriber(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().InsertSubscriber(this, eMessageType::eKeyboardMessage);
	PostMaster::GetInstance().InsertSubscriber(this, eMessageType::ePlayerGotNewWeapon);
	PostMaster::GetInstance().InsertSubscriber(this, eMessageType::ePlayerChangedWeapon);
	PostMaster::GetInstance().InsertSubscriber(myCursor, eMessageType::eMouseMessage);


	if (locMousePosition != CU::Vector2f::Zero && myCursor != nullptr)
	{
		myCursor->SetPosition(locMousePosition);
	}
}
