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
#include "HealthWidget.h"
#include "ManaWidget.h"

#include "../BrontosaurusEngine/FBXLoader.h"
#include "../BrontosaurusEngine/Engine.h"

#include "../CommonUtilities/Camera.h"

#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PushState.h"
#include "../PostMaster/PopCurrentState.h"
#include "../PostMaster/Pop2States.h"
#include "../PostMaster/HatBought.h"
#include "../PostMaster/BuyButtonPressed.h"
#include "PostMaster/ShopItemButtonPressed.h"

#include "../Game/PollingStation.h"
#include  "../Components/PlayerData.h"
#include "CommonUtilities/JsonValue.h"

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

		std::string jsonPath = aFilePathFBX;
		jsonPath -= ".fbx";
		jsonPath += ".json";
		return CreateGUIScene(&guiScene, aGUIManagerCameraOut, jsonPath);
	}

	WidgetContainer* WidgetFactory::CreateGUIScene(const CLoaderScene* aLoaderScene, CU::Camera*& aGUIManagerCameraOut, const std::string& aJsonPath)
	{
		if (aLoaderScene == nullptr)
		{
			DL_PRINT_WARNING("Create GUI scene got loader scene that was NULL\n");
			return nullptr;
		}

		CU::Camera* guiCamera = ParseCamera(aLoaderScene->myCamera);
		aGUIManagerCameraOut = guiCamera;


		CU::CJsonValue guiScene(aJsonPath);
		CU::CJsonValue visibleFromStartArray = guiScene["VisibleFromStart"];
		CU::GrowingArray<std::string> visibleWidgets(8u);
		for (int i = 0; i < visibleFromStartArray.Size(); ++i)
		{
			std::string visibleWidget = visibleFromStartArray[i].GetString();
			visibleWidgets.Add(visibleWidget);
		}

		CU::CJsonValue buttons = guiScene["Buttons"];
		CU::GrowingArray<std::string> buttonNames(8u);
		for (int i = 0; i < buttons.Size(); i++)
		{
			buttonNames.Add(buttons[i].GetString());
		}


		bool hasKeyHealthOrb = guiScene.HasKey("healthOrb");
		std::string healthOrbName("");
		std::string manaOrbName("");
		std::string orbTexture("");
		if (hasKeyHealthOrb == true)
		{
			healthOrbName = guiScene["healthOrb"].GetString();
			manaOrbName = guiScene["manaOrb"].GetString();
			orbTexture = guiScene["orbTexture"].GetString();
		}

		WidgetContainer* baseWidgetContainer = new WidgetContainer(CU::Vector2f::Zero, CU::Vector2f(1.f, 1.f), "BaseWidgetContainer", true);

		const CU::GrowingArray<CLoaderMesh*>& meshes = aLoaderScene->myMeshes;
		for (size_ga i = 0; i < meshes.Size(); ++i)
		{
			std::string widgetName = meshes[i]->myName;
			bool isVisible = visibleWidgets.Size() == 0 || visibleWidgets.Find(widgetName) != visibleWidgets.FoundNone;
			Widget* widget = nullptr;
			if (hasKeyHealthOrb == true && (widgetName == healthOrbName || widgetName == manaOrbName))
			{
				meshes[i]->myTransformation.Rotate(PI_CONSTANT, CU::Axees::Y);
				widget = new ModelWidget(meshes[i], { orbTexture }, *guiCamera, isVisible);
			}
			else
			{
				widget = new ModelWidget(meshes[i], aLoaderScene->myTextures, *guiCamera, isVisible);
			}

			if (widgetName == healthOrbName)
			{
				widget = CreateHealthBar(widget);
			}
			else if (widgetName == manaOrbName)
			{
				widget = CreateManaBar(widget);
			}
			else if (buttonNames.Find(widgetName) != buttonNames.FoundNone ||  widget->GetName().find("button") != std::string::npos || widget->GetName().find("Button") != std::string::npos
				|| widget->GetName() == "Resume" || widget->GetName() == "Return" || widget->GetName().find("Knapp") != std::string::npos
				|| widget->GetName().find("knapp") != std::string::npos)
			{
				widget = CreateButton(widget);
			}

			if (widget != nullptr)
			{
				baseWidgetContainer->AddWidget(widget->GetName(), widget);
			}
		}
		baseWidgetContainer->MoveToBack("shopWindow");
		return baseWidgetContainer;
	}

	Widget* WidgetFactory::CreateButton(Widget* aWidget)
	{
		const std::string& widgetName = aWidget->GetName();
		if (widgetName.find("Quit") != std::string::npos)
		{
			auto popStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Start") != std::string::npos)
		{
			auto pushPlayStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, PushState::_DONT_ASK_))); };
			Button* button = new Button(pushPlayStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Credits") != std::string::npos)
		{
			auto pushCreditStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::eCreditScreen, PushState::_DONT_ASK_))); };
			Button* button = new Button(pushCreditStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("LevelSelect") != std::string::npos)
		{
			auto pushCreditStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::eLevelSelect, PushState::_DONT_ASK_))); };
			Button* button = new Button(pushCreditStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Retry") != std::string::npos)
		{
			auto popStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Resume") != std::string::npos)
		{
			auto popStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Continue") != std::string::npos)
		{
			auto popStateMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Return") != std::string::npos)
		{
			auto popStateMessage = [] {	PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, Pop2States())); };
			Button* button = new Button(popStateMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Level1") != std::string::npos)
		{
			auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 11))); };
			Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Level2") != std::string::npos)
		{
			auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 12))); };
			Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Level3") != std::string::npos)
		{
			auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 13))); };
			Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("Level4") != std::string::npos)
		{
			auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 14))); };
			Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("buyK") != std::string::npos)
		{
			auto buyHatMessage = [] 
			{ 
				PostMaster::GetInstance().SendLetter(Message(eMessageType::eBuyButtonPressed, BuyButtonPressed()));
			};
			Button* button = new Button(buyHatMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		else if (widgetName.find("buyHat") != std::string::npos)
		{
			if (widgetName.rfind("1") != std::string::npos)
			{
				auto SelectItemInShopMessage = []
				{
					PostMaster::GetInstance().SendLetter(Message(eMessageType::eShopItemSelected, ShopItemButtonPressed(0)));
				};
				Button* button = new Button(SelectItemInShopMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}

			else if (widgetName.rfind("2") != std::string::npos)
			{
				auto SelectItemInShopMessage = []
				{
					PostMaster::GetInstance().SendLetter(Message(eMessageType::eShopItemSelected, ShopItemButtonPressed(1)));
				};
				Button* button = new Button(SelectItemInShopMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}

			else if (widgetName.rfind("3") != std::string::npos)
			{
				auto SelectItemInShopMessage = []
				{
					PostMaster::GetInstance().SendLetter(Message(eMessageType::eShopItemSelected, ShopItemButtonPressed(2)));
				};
				Button* button = new Button(SelectItemInShopMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}

			else if (widgetName.rfind("4") != std::string::npos)
			{
				auto SelectItemInShopMessage = []
				{
					PostMaster::GetInstance().SendLetter(Message(eMessageType::eShopItemSelected, ShopItemButtonPressed(3)));
				};
				Button* button = new Button(SelectItemInShopMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}

			else if (widgetName.rfind("5") != std::string::npos)
			{
				auto SelectItemInShopMessage = []
				{
					PostMaster::GetInstance().SendLetter(Message(eMessageType::eShopItemSelected, ShopItemButtonPressed(4)));
				};
				Button* button = new Button(SelectItemInShopMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}

			else if (widgetName.rfind("6") != std::string::npos)
			{
				auto SelectItemInShopMessage = []
				{
					PostMaster::GetInstance().SendLetter(Message(eMessageType::eShopItemSelected, ShopItemButtonPressed(5)));
				};
				Button* button = new Button(SelectItemInShopMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}
		}
		else
		{
			DL_PRINT_WARNING("Button created but no suitable callback function was implemented: %s", widgetName.c_str());
			Button* button = new Button(nullptr, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName() + "_EmptyCallback");
			button->AddWidget("Animation", new ButtonAnimation(aWidget));
			return button;
		}
		return nullptr;
	}

	CU::Camera* WidgetFactory::ParseCamera(const CLoaderCamera* aCamera)
	{
		if (aCamera == nullptr)
		{
			DL_MESSAGE_BOX("GUI Widget factory got CLoaderCamera that is NULL");
			return nullptr;
		}

		CU::Camera* camera = new CU::Camera(aCamera->myFov * (180.f / PI_CONSTANT), aCamera->myAspectRatio, 1.f, aCamera->myNear, aCamera->myFar);
		camera->SetTransformation(aCamera->myTransformation);

		return camera;
	}

	GUI::Widget* WidgetFactory::CreateHealthBar(Widget* aWidget)
	{
		CHealthWidget* healthWidget = new CHealthWidget(aWidget->GetWorldPosition(), aWidget->GetSize(), "PlayerHealthWidget");
		healthWidget->AddWidget("Model", aWidget);
		healthWidget->Init();
		return healthWidget;
	}

	GUI::Widget* WidgetFactory::CreateManaBar(Widget* aWidget)
	{
		CManaWidget* manaWidget = new CManaWidget(aWidget->GetWorldPosition(), aWidget->GetSize(), "PlayerManaWidget");
		manaWidget->AddWidget("Model", aWidget);
		manaWidget->Init();
		return manaWidget;
	}

}
