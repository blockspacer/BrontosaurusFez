#include "stdafx.h"
#include "WidgetFactory.h"

#include "Widget.h"
#include "WidgetContainer.h"
#include "ModelWidget.h"
#include "SpriteWidget.h"
#include "Button.h"
#include "BarWidget.h"
#include "TextWidget.h"
#include "ButtonAnimation.h"
#include "ToolTipDecorator.h"
#include "HealthWidget.h"
#include "ManaWidget.h"
#include "MoneyWidget.h"

#include "../BrontosaurusEngine/FBXLoader.h"
#include "../BrontosaurusEngine/Engine.h"
#include "../BrontosaurusEngine/ModelInstance.h"

#include "../CommonUtilities/Camera.h"

#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PushState.h"
#include "../PostMaster/PopCurrentState.h"
#include "../PostMaster/Pop2States.h"
#include "../PostMaster/HatBought.h"
#include "../PostMaster/BuyButtonPressed.h"
#include "../PostMaster/ShopItemButtonPressed.h"

#include "../Game/PollingStation.h"
#include "../Components/PlayerData.h"
#include "../CommonUtilities/JsonValue.h"
#include "../ShopItemButtonHovered.h"
#include "HatIcon.h"
#include "Components/GameObject.h"
#include "Components/ComponentMessage.h"

using size_ga = CU::GrowingArray<CLoaderMesh*>::size_type;

const float PI_CONSTANT = 3.141592f;

namespace GUI
{
	std::map<std::string, IWidget*> WidgetFactory::ourHatContainer;

	const CU::Camera* WidgetFactory::ourCurrentGUICamera = nullptr;

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

	const CU::Camera* WidgetFactory::GetCurrentGUICamera()
	{
		return ourCurrentGUICamera;
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
		ourCurrentGUICamera = guiCamera;

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

		CU::GrowingArray<std::string> guiHatnames(8u);
		CU::GrowingArray<std::string> guiRealHatNames(8u);
		if (guiScene.HasKey("Hats"))
		{
			CU::CJsonValue guiHats = guiScene["Hats"];
			int startIndex = guiHats["start"].GetInt();
			int endIndex = guiHats["end"].GetInt();
			const std::string& nameStart = guiHats["name"].GetString();
			for (int i = startIndex; i <= endIndex; ++i)
			{
				guiHatnames.Add(nameStart + std::to_string(i));
			}

			CU::CJsonValue guiHatNames = guiScene["HatNames"];
			for (int i = 0; i < guiHatNames.Size(); ++i)
			{
				guiRealHatNames.Add(guiHatNames[i].GetString());
			}
		}

		bool hasKeyHealthOrb = guiScene.HasKey("healthOrb");
		std::string healthOrbName("");
		std::string manaOrbName("");
		std::string orbTexture("");
		std::string moneyName("");
		std::string moneyTexture("");
		if (hasKeyHealthOrb == true)
		{
			healthOrbName = guiScene["healthOrb"].GetString();
			manaOrbName = guiScene["manaOrb"].GetString();
			orbTexture = guiScene["orbTexture"].GetString();
			moneyTexture = guiScene["moneyTexture"].GetString();
			moneyName = guiScene["moneyName"].GetString();
		}

		WidgetContainer* baseWidgetContainer = new WidgetContainer(CU::Vector2f::Zero, CU::Vector2f(1.f, 1.f), "BaseWidgetContainer", true);

		const CU::GrowingArray<CLoaderMesh*>& meshes = aLoaderScene->myMeshes;
		for (size_ga i = 0; i < meshes.Size(); ++i)
		{
			std::string widgetName = meshes[i]->myName;
			bool isVisible = visibleWidgets.Size() == 0 || visibleWidgets.Find(widgetName) != visibleWidgets.FoundNone;
			IWidget* widget = nullptr;
			if (hasKeyHealthOrb == true && (widgetName == healthOrbName || widgetName == manaOrbName))
			{
				widget = new ModelWidget(meshes[i], { orbTexture }, *guiCamera, isVisible);
			}
			else if (hasKeyHealthOrb == true && widgetName == moneyName)
			{
				widget = new ModelWidget(meshes[i], { moneyTexture }, *guiCamera, isVisible);
			}
			else
			{
				widget = new ModelWidget(meshes[i], aLoaderScene->myTextures, *guiCamera, isVisible);
			}

			int hatIndex = guiHatnames.Find(widgetName);
			if (hatIndex != guiHatnames.FoundNone)
			{
				widget->SetVisibility(true);
				//CHatIcon* guiHat = new CHatIcon(*widget);
				IWidget* guiHat = widget;
				widget = nullptr;

				CToolTipDecorator* finalhat = new CToolTipDecorator(guiHat, nullptr, nullptr, nullptr);
				WidgetFactory::ourHatContainer[guiRealHatNames[hatIndex]] = finalhat;

				continue;
			}

			if (widgetName == healthOrbName)
			{
				widget = CreateHealthBar(widget);
			}
			else if (widgetName == manaOrbName)
			{
				widget = CreateManaBar(widget);
			}
			else if (widgetName == moneyName)
			{
				widget = CreateMoney(widget);
			}
			else if (widgetName.find("skillKnapp0") != std::string::npos)
			{
				if (widgetName.rfind("1") != std::string::npos)
				{
					widget->SetVisibility(true);
					std::string basicAttackName = "Basic attack\nLeft button";
					widget = new CToolTipDecorator(widget, nullptr, &basicAttackName, nullptr);
				}
				else if (widgetName.rfind("2") != std::string::npos)
				{
					std::string sweepAttackName = "Sweep attack - Right button\nDeals more damage over a wider area but costs some mana";
					widget = new CToolTipDecorator(widget, nullptr, &sweepAttackName, nullptr);
				}
				else if (widgetName.rfind("3") != std::string::npos)
				{
					std::string whirlwindAttackName = "Whirlwind - Space\nDeals damage in a circle around you";
					widget = new CToolTipDecorator(widget, nullptr, &whirlwindAttackName, nullptr);
				}
			}
			else if (buttonNames.Find(widgetName) != buttonNames.FoundNone ||  widget->GetName().find("button") != std::string::npos || widget->GetName().find("Button") != std::string::npos
				|| widget->GetName() == "Resume" || widget->GetName() == "Return" || widget->GetName().find("Knapp") != std::string::npos
				|| widget->GetName().find("knapp") != std::string::npos)
			{
				if (isVisible)
				{
					widget = CreateButton(widget);
				}
			}
			if (widgetName == "guiBase")
			{
				WidgetContainer* guiBase = new WidgetContainer(widget->GetWorldPosition(), widget->GetSize(), widget->GetName(), true);
				guiBase->AddWidget("Model", widget);
				widget = guiBase;
			}

			if (widget != nullptr)
			{
				baseWidgetContainer->AddWidget(widget->GetName(), widget);
			}
		}

		baseWidgetContainer->MoveToBack("shopWindow");
		baseWidgetContainer->MoveToBack("guiBase");
		baseWidgetContainer->MoveToBack("PlayerHealthWidget");
		baseWidgetContainer->MoveToBack("PlayerManaWidget");
		baseWidgetContainer->MoveToFront(moneyName);

		if (guiScene.HasKey("remove"))
		{
			if (guiScene["remove"].IsString())
			{
				delete baseWidgetContainer->RemoveWidget(guiScene["remove"].GetString());
			}
		}

		return baseWidgetContainer;
	}

	IWidget* WidgetFactory::CreateButton(IWidget* aWidget)
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
		else if (widgetName.find("buttonlevel") != std::string::npos)
		{
			if (widgetName.rfind("1") != std::string::npos)
			{
				auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 10))); };
				Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}
			else if (widgetName.rfind("2") != std::string::npos)
			{
				auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 11))); };
				Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}
			else if (widgetName.rfind("3") != std::string::npos)
			{
				auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 12))); };
				Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}
			else if (widgetName.rfind("4") != std::string::npos)
			{
				auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 13))); };
				Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Animation", new ButtonAnimation(aWidget));
				return button;
			}
		}
		//else if (widgetName.find("Level1") != std::string::npos)
		//{
		//	auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 10))); };
		//	Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
		//	button->AddWidget("Animation", new ButtonAnimation(aWidget));
		//	return button;
		//}
		//else if (widgetName.find("Level2") != std::string::npos)
		//{
		//	auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 11))); };
		//	Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
		//	button->AddWidget("Animation", new ButtonAnimation(aWidget));
		//	return button;
		//}
		//else if (widgetName.find("Level3") != std::string::npos)
		//{
		//	auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 12))); };
		//	Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
		//	button->AddWidget("Animation", new ButtonAnimation(aWidget));
		//	return button;
		//}
		//else if (widgetName.find("Level4") != std::string::npos)
		//{
		//	auto pushLevelMessage = [] { PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePlayState, 13))); };
		//	Button* button = new Button(pushLevelMessage, aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
		//	button->AddWidget("Animation", new ButtonAnimation(aWidget));
		//	return button;
		//}
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
			auto getToolTipText = [](int aIndex, std::string& aTooltipTextOut) -> bool
			{
				PostMaster::GetInstance().SendLetter(Message(eMessageType::eShopItemSelected, CQueryShopItemButtonHovered(aIndex, &aTooltipTextOut)));
				return !aTooltipTextOut.empty();
			};
			auto selectItemInShopMessage = [](const char aIndex)
			{
				PostMaster::GetInstance().SendLetter(Message(eMessageType::eShopItemSelected, ShopItemButtonPressed(aIndex)));
			};
			for (char i = 0; i < 6; ++i)
			{
				if (widgetName.rfind(std::to_string(static_cast<int>(i + 1))) == std::string::npos) continue;

				Button* button = new Button(std::bind(selectItemInShopMessage, i), aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
				button->AddWidget("Model", aWidget);

				CToolTipDecorator* toolTip = new CToolTipDecorator(button, nullptr, nullptr, std::bind(getToolTipText, i, std::placeholders::_1));
				return toolTip;
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
			DL_MESSAGE_BOX("GUI IWidget factory got CLoaderCamera that is NULL");
			return nullptr;
		}

		CU::Camera* camera = new CU::Camera(aCamera->myFov * (180.f / PI_CONSTANT), aCamera->myAspectRatio, 1.f, aCamera->myNear, aCamera->myFar);
		camera->SetTransformation(aCamera->myTransformation);

		return camera;
	}

	GUI::IWidget* WidgetFactory::CreateHealthBar(IWidget* aWidget)
	{
		CHealthWidget* healthWidget = new CHealthWidget(aWidget->GetWorldPosition(), aWidget->GetSize(), "PlayerHealthWidget");
		healthWidget->AddWidget("Model", aWidget);
		healthWidget->Init();

		auto getCurrentHealth = [healthWidget](std::string& aTextOut) -> bool
		{
			if (!PollingStation::playerObject) return false;

			SComponentMessageData healthData;
			int health = 0;
			healthData.myVoidPointer = &health;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eQueryHealth, healthData);

			//float health = healthWidget->GetHealthPercent();
			//health *= 100.f;
			//int healthFloored = static_cast<int>(health);
			aTextOut = "Health:\n";
			aTextOut += std::to_string(/*healthFloored*/health);
			//aTextOut += "%";
			return !aTextOut.empty();
		};

		CToolTipDecorator* toolTip = new CToolTipDecorator(healthWidget, nullptr, nullptr, getCurrentHealth);
		return toolTip;

		//return healthWidget;
	}

	GUI::IWidget* WidgetFactory::CreateManaBar(IWidget* aWidget)
	{
		CManaWidget* manaWidget = new CManaWidget(aWidget->GetWorldPosition(), aWidget->GetSize(), "PlayerManaWidget");
		manaWidget->AddWidget("Model", aWidget);
		manaWidget->Init();


		auto getCurrentMana = [manaWidget](std::string& aTextOut) -> bool
		{
			if (!PollingStation::playerObject) return false;

			SComponentMessageData healthData;
			int mana = 0;
			healthData.myVoidPointer = &mana;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eQueryMana, healthData);

			//float mana = manaWidget->GetManaPercent();
			//mana *= 100.f;
			//int manaFloored = static_cast<int>(mana);
			aTextOut = "Mana:\n";
			aTextOut += std::to_string(/*manaFloored*/mana);
			//aTextOut += "%";
			return !aTextOut.empty();
		};

		CToolTipDecorator* toolTip = new CToolTipDecorator(manaWidget, nullptr, nullptr, getCurrentMana);
		return toolTip;

		//return manaWidget;
	}

	IWidget* WidgetFactory::CreateMoney(IWidget* aWidget)
	{
		CMoneyWidget* moneyWidget = new CMoneyWidget(aWidget->GetWorldPosition(), aWidget->GetSize(), aWidget->GetName());
		
		moneyWidget->AddWidget("Model", aWidget);
		moneyWidget->Init();

		return moneyWidget;
	}
}
