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

#include "../BrontosaurusEngine/FBXLoader.h"
#include "../BrontosaurusEngine/Engine.h"
#include "../BrontosaurusEngine/GUIRenderer.h"

#include "../CommonUtilities/Camera.h"

#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PushState.h"
#include "../PostMaster/PopCurrentState.h"
#include "../PostMaster/Pop2States.h"

#include "../Game/PollingStation.h"

using size_ga = CU::GrowingArray<CLoaderMesh*>::size_type;

const float PI_CONSTANT = 3.141592f;

namespace GUI
{
	WidgetContainer* WidgetFactory::CreateGUIScene(const char* aFilePathFBX, bool isOld, CU::Camera*& aGUIManagerCameraOut)
	{
		isOld;

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
		CEngine::GetInstance()->GetGUIRenderer().SetCamera(guiCamera);
		aGUIManagerCameraOut = guiCamera;

		WidgetContainer* baseWidgetContainer = new WidgetContainer(CU::Vector2f::Zero, CU::Vector2f(1.f, 1.f), "BaseWidgetContainer", true);

		const CU::GrowingArray<CLoaderMesh*>& meshes = aLoaderScene->myMeshes;
		for (size_ga i = 0; i < meshes.Size(); ++i)
		{
			Widget* widget = nullptr;
			CU::DynamicString widgetName = meshes[i]->myName;
			if (CU::DynamicString(meshes[i]->myName).Find("healtBar") != CU::DynamicString::FoundNone)
			{
				widget = new ModelWidget(meshes[i], { "Models/gui/gui.dds" });
			}
			else if (CU::DynamicString(meshes[i]->myName).Find("gameOver") != CU::DynamicString::FoundNone)
			{
				widget = new ModelWidget(meshes[i], { "Models/gameover/gameover.dds" });
			}
			else
			{
				widget = new ModelWidget(meshes[i], aLoaderScene->myTextures/*myAlbedoTexture.c_str()*/);
			}

			if (widget->GetName().Find("laser") != CU::DynamicString::FoundNone ||
				widget->GetName().Find("scatterGun") != CU::DynamicString::FoundNone ||
				widget->GetName().Find("hostileMark") != CU::DynamicString::FoundNone)
			{
				widget->SetVisibility(false);
			}

			if (widget->GetName().Find("button") != CU::DynamicString::FoundNone || widget->GetName().Find("Button") != CU::DynamicString::FoundNone
				|| widget->GetName() == "Resume" || widget->GetName() == "Return")
			{
				widget = CreateButton(widget);
			}

			if (widget->GetName().Find("healthBar") != CU::DynamicString::FoundNone || widget->GetName().Find("healtBar") != CU::DynamicString::FoundNone)
			{
				widget = CreateHealthBar(widget);
			}
			else if (widget->GetName().Find("boost") != CU::DynamicString::FoundNone)
			{
				widget = CreateBoostBar(widget);
			}
			else if (widget->GetName().Find("timebar") != CU::DynamicString::FoundNone)
			{
				widget = CreateTimeBar(widget);
				//widget->SetVisibility(false);
			}

			if (widget != nullptr)
			{
				baseWidgetContainer->AddWidget(widget->GetName(), widget);
			}
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
		else
		{
			DL_PRINT_WARNING("Button created but no suitable callback function was implemented: %s", widgetName.c_str());
			Button* button = new Button(nullptr, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName() + "_EmptyCallback");
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
	}

	Widget* WidgetFactory::CreateHealthBar(Widget* aWidget)
	{
		BarWidget* healthBar = new BarWidget(aWidget->GetWorldPosition(), aWidget->GetSize(), "playerHealthBar", true, 100, 100);
		healthBar->AddWidget("model", aWidget);
		PollingStation::playerHealthBar = healthBar;

		return healthBar;
		//return nullptr;
	}

	Widget* WidgetFactory::CreateBoostBar(Widget* aModelWidget)
	{
		BarWidget* boosthBar = new BarWidget(aModelWidget->GetWorldPosition(), aModelWidget->GetSize(), "playerBoostBar", true, 100, 100);
		boosthBar->AddWidget("model", aModelWidget);
		PollingStation::playerBoostBar = boosthBar;

		return boosthBar;
	}

	CU::Camera* WidgetFactory::ParseCamera(const CLoaderCamera* aCamera)
	{
		if (aCamera == nullptr)
		{
			DL_ASSERT("GUI Widget factory got CLoaderCamera that is NULL");
			return nullptr;
		}

		CU::Camera* camera = new CU::Camera(aCamera->myFov * (180.f / PI_CONSTANT), aCamera->myAspectRatio, 1.f, aCamera->myNear, aCamera->myFar, aCamera->myTransformation.myPosition);
		camera->SetTransformation(aCamera->myTransformation);

		return camera;
	}

	Widget* WidgetFactory::CreateTimeBar(Widget* aModelWidget)
	{
		//aModelWidget->SetVisibility(false);
		BarWidget* timeBar = new BarWidget(aModelWidget->GetWorldPosition(), aModelWidget->GetSize(), "timeObjectiveBar", false, 100, 100);
		timeBar->AddWidget("model", aModelWidget);
		PollingStation::timeObjectiveBar = timeBar;

		return timeBar;
	}

}
