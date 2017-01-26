#pragma once
#include "WidgetContainer.h"
#include "../PostMaster/Subscriber.h"

namespace GUI
{
	class CMoneyWidget : public WidgetContainer, public Subscriber
	{
	public:
		CMoneyWidget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const std::string& aName);
		~CMoneyWidget();

		void Init();
		void Update(const CU::Time& aDeltaTime);
		void SetMoneyPercent(const float aMoneyPercent);

		eMessageReturn Recieve(const Message& aMessage) override;

	private:
		short GetCheapestHatPrice();
		//float myMoneyPercent;
		short& myMoney;
	};
}
