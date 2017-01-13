#include "stdafx.h"
#include "WidgetFactory.h"

#include <Widget.h>
#include <WidgetContainer/WidgetContainer.h>
#include <RenderableWidgets/ModelWidget/ModelWidget.h>
#include <RenderableWidgets/SpriteWidget/SpriteWidget.h>
#include <Button/Button.h>
#include <BarWidget/BarWidget.h>
#include <WidgetDecorator/TextWidget/TextWidget.h>
#include "Button/ButtonAnimation.h"
#include "ToolTipDecorator.h"

#include "../BrontosaurusEngine/FBXLoader.h"
#include "../BrontosaurusEngine/Engine.h"

#include "../CommonUtilities/Camera.h"

#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PushState.h"
#include "../PostMaster/PopCurrentState.h"
#include "../PostMaster/Pop2States.h"
#include "../PostMaster/HatBought.h"

#include "../Game/PollingStation.h"
#include  "../Components/PlayerData.h"

using size_ga = CU::GrowingArray<CLoaderMesh*>::size_type;

const float PI_CONSTANT = 3.141592f;

namespace GUI
{
	WidgetContainer* WidgetFactory::CreateGUIScene(const char* aFilePathFBX, CU::Camera*& aGUIManagerCameraOut)
	{
		CFBXLoader loader;
		CLoaderScene guiScene;
		if (loader.LoadGUIScene(aFilePathFBX, guiScene) == false)
		{
			DL_PRINT_WARNING("Failed to load GUI scene: %s\n", aFilePathFBX);
		}

		return CreateGUIScene(&guiScene, aGUIManagerCameraOut);
	}

	WidgetContainer* WidgetFactory::CreateGUIScene(const CLoaderScene* aLoaderScene, CU::Camera*& aGUIManagerCameraOut)
	{
		if (aLoaderScene == nullptr)
		{
			DL_PRINT_WARNING("Create GUI scene got loader scene that was NULL\n");
			return nullptr;
		}

		CU::Camera* guiCamera = ParseCamera(aLoaderScene->myCamera);
		aGUIManagerCameraOut = guiCamera;

		WidgetContainer* baseWidgetContainer = new WidgetContainer(CU::Vector2f::Zero, CU::Vector2f(1.f, 1.f), "BaseWidgetContainer", true);

		const CU::GrowingArray<CLoaderMesh*>& meshes = aLoaderScene->myMeshes;
		for (size_ga i = 0; i < meshes.Size(); ++i)
		{
			CU::DynamicString widgetName = meshes[i]->myName;

			Widget* widget = new ModelWidget(meshes[i], aLoaderScene->myTextures, *guiCamera);

			if (widget->GetName().Find("button") != CU::DynamicString::FoundNone || widget->GetName().Find("Button") != CU::DynamicString::FoundNone
				|| widget->GetName() == "Resume" || widget->GetName() == "Return" || widget->GetName().Find("Knapp") != CU::DynamicString::FoundNone || widget->GetName() == "knapp")
			{
				widget = CreateButton(widget);
			}

			if (widget != nullptr)
			{
				baseWidgetContainer->AddWidget(widget->GetName(), widget);
			}
		}

		CU::DynamicString widgetName = "CarlWasHere";
		CFBXLoader loader;
		CLoaderModel* loaderMOdel = loader.LoadModel("Models/gui/knapp01.fbx");
		Widget* widget = new ModelWidget(loaderMOdel->myMeshes.at(0), aLoaderScene->myTextures, *guiCamera);

		CLoaderModel* loaderMOdel2 = loader.LoadModel("Models/gui/guiTooltip.fbx");
		ModelWidget* backgroundModel = new ModelWidget(loaderMOdel2->myMeshes.at(0), aLoaderScene->myTextures, *guiCamera);
		std::string tooltipText = "hey im a tooltip";
		widget = new CToolTipDecorator(widget, backgroundModel, tooltipText);

		if (widget != nullptr)
		{
			baseWidgetContainer->AddWidget(widget->GetName(), widget);
		}


		return baseWidgetContainer;
	}

	Widget* WidgetFactory::CreateButton(Widget* aWidget)
	{
		const CU::DynamicString& widgetName = aWidget->GetName();
		if (widgetName.Find("Quit") != CU::DynamicString::FoundNone)
		{
			auto popStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Start") != CU::DynamicString::FoundNone)
		{
			auto pushPlayStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, PushState::_DONT_ASK_))); };
			Button* button = new Button(pushPlayStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Credits") != CU::DynamicString::FoundNone)
		{
			auto pushCreditStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::eCreditScreen, PushState::_DONT_ASK_))); };
			Button* button = new Button(pushCreditStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("LevelSelect") != CU::DynamicString::FoundNone)
		{
			auto pushCreditStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::eLevelSelect, PushState::_DONT_ASK_))); };
			Button* button = new Button(pushCreditStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Retry") != CU::DynamicString::FoundNone)
		{
			auto popStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Resume") != CU::DynamicString::FoundNone)
		{
			auto popStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Continue") != CU::DynamicString::FoundNone)
		{
			auto popStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Return") != CU::DynamicString::FoundNone)
		{
			auto popStateMessage = [] {	PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, Pop2States())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Level1") != widgetName.FoundNone)
		{
			auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 11))); };
			Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Level2") != widgetName.FoundNone)
		{
			auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 12))); };
			Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Level3") != widgetName.FoundNone)
		{
			auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 13))); };
			Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("Level4") != widgetName.FoundNone)
		{
			auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 14))); };
			Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.Find("buy") != widgetName.FoundNone)
		{
			auto buyHatMessage = [] 
			{ 
				if (PollingStation::playerData->myGold >= 50 /*Should be something like hat->GetCost() later*/)
				{
					PostMaster::GetInstance().SendLetter(Message(eMessageType::eHatAdded, HatBought()));
					PollingStation::playerData->myGold -= 50;
				}
			};
			Button* button = new Button(buyHatMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else
		{
			DL_PRINT_WARNING("Button created but no suitable callback function was implemented: %s", widgetName.c_str());
			Button* button = new Button(nullptr, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName() + "_EmptyCallback");
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
	}

	CU::Camera* WidgetFactory::ParseCamera(const CLoaderCamera* aCamera)
	{
		if (aCamera == nullptr)
		{
			DL_ASSERT("GUI Widget factory got CLoaderCamera that is NULL");
			return nullptr;
		}

		CU::Camera* camera = new CU::Camera(aCamera->myFov * (180.f / PI_CONSTANT), aCamera->myAspectRatio, 1.f, aCamera->myNear, aCamera->myFar);
		camera->SetTransformation(aCamera->myTransformation);

		return camera;
	}
}
