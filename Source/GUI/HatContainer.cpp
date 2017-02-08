#include "stdafx.h"
#include "HatContainer.h"
#include "Game\HatMaker.h"
#include "WidgetFactory.h"
#include "GUIManager.h"
#include "Widget.h"
#include "ToolTipDecorator.h"

#include "../Components/InventoryComponent.h"
#include "WidgetContainer.h"

namespace GUI
{
	HatContainer::HatContainer()
		: myHatIcons(8u)
	{
	}

	HatContainer::~HatContainer()
	{
	}

	void HatContainer::TryInit()
	{
		static bool isInit = false;
		if (!isInit)
		{
			isInit = true;
			PostMaster::GetInstance().Subscribe(this, eMessageType::ePlayerGotHat);
		}
	}

	void HatContainer::Destroy()
	{
		PostMaster::GetInstance().UnSubscribe(this, eMessageType::ePlayerGotHat);
	}

	void HatContainer::AddAllHats(GUIManager& aGUIManager)
	{
		IWidget* baseWidget = aGUIManager.FindWidget("guiBase");
		if (baseWidget)
		{
			for (IWidget* hatIcon : myHatIcons)
			{
				static_cast<GUI::WidgetContainer*>(baseWidget)->AddWidget(hatIcon->GetName(), hatIcon);
			}
		}
		else
		{
			for (IWidget* hatIcon : myHatIcons)
			{
				aGUIManager.AddWidget(hatIcon->GetName(), hatIcon);
			}
		}
	}

	void HatContainer::RemoveAllHats(GUIManager& aGUIManager)
	{
		IWidget* baseWidget = aGUIManager.FindWidget("guiBase");
		if (baseWidget)
		{
			for (IWidget* hatIcon : myHatIcons)
			{
				static_cast<GUI::WidgetContainer*>(baseWidget)->RemoveWidget(hatIcon->GetName());
			}
		}
		else
		{
			for (IWidget* hatIcon : myHatIcons)
			{
				aGUIManager.RemoveWidget(hatIcon->GetName());
			}
		}
	}

	bool HatContainer::TryAddHat(SHatBluePrint* aHatBluePrint)
	{
		std::string cleanHatName = ExtractBaseHatName(aHatBluePrint->HatName);

		auto it = myHatBluePrints.find(cleanHatName);
		bool hatIsNew = it == myHatBluePrints.end();
		if (hatIsNew
			|| RightIsBetterThanLeft(it->second->HatName, aHatBluePrint->HatName))
		{
			myHatBluePrints[cleanHatName] = aHatBluePrint;
			CToolTipDecorator* tooltipz = *WidgetFactory::ourHatContainer[cleanHatName];
			if (tooltipz)
			{
				auto getToolTipText = [this, cleanHatName](std::string& aTooltipTextOut) -> bool
				{
					aTooltipTextOut = myHatBluePrints[cleanHatName]->myHatStruct->description;
					return !aTooltipTextOut.empty();
				};

				tooltipz->SetVisibility(true);

				if (hatIsNew)
				{
					tooltipz->SetTextFunction(getToolTipText);
					myHatIcons.Add(tooltipz);
				}
				else
				{
					tooltipz->ChangeTextFunction(getToolTipText);
				}

				return true;
			}

			assert(false && "Tell carl about this");
		}

		return false;
	}

	std::string HatContainer::ExtractBaseHatName(const std::string& aFullHatName)
	{
		std::string cleanHatName(aFullHatName);

		cleanHatName -= "V1";
		cleanHatName -= "V2";
		cleanHatName -= "V3";
		
		return std::move(cleanHatName);
	}

	bool HatContainer::RightIsBetterThanLeft(const std::string& aFullHatNameLeft, const std::string& aFullHatNameRight)
	{
		if (aFullHatNameLeft.find("V1") != std::string::npos)
		{
			if (aFullHatNameRight.find("V2") != std::string::npos)
			{
				return true;
			}
			if (aFullHatNameRight.find("V3") != std::string::npos)
			{
				return true;
			}
		}
		else if (aFullHatNameLeft.find("V2") != std::string::npos)
		{
			if (aFullHatNameRight.find("V3") != std::string::npos)
			{
				return true;
			}
		}

		return false;
	}

	eMessageReturn HatContainer::Recieve(const Message& aMessage)
	{
		return aMessage.myEvent.DoEvent(this);
	}
}
