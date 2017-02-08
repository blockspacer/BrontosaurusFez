#pragma once

#include "../PostMaster/Subscriber.h"

struct SHatBluePrint;

namespace GUI
{
	class IWidget;

	class HatContainer : public Subscriber
	{
	public:
		HatContainer();
		~HatContainer();

		void TryInit();
		void Destroy();

		void AddAllHats(GUIManager& aGUIManager);
		void RemoveAllHats(GUIManager& aGUIManager);

		bool TryAddHat(SHatBluePrint* aHatBluePrint);

	private:
		std::string ExtractBaseHatName(const std::string& aFullHatName);
		bool RightIsBetterThanLeft(const std::string& aFullHatNameLeft, const std::string& aFullHatNameRight);

		std::map<std::string, SHatBluePrint*> myHatBluePrints;
		CU::GrowingArray<IWidget*> myHatIcons;

		eMessageReturn Recieve(const Message& aMessage) override;
	};
}
