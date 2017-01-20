#pragma once
#include "WidgetContainer/WidgetContainer.h"
#include "../PostMaster/Subscriber.h"

namespace GUI
{
	class CManaWidget : public WidgetContainer, public Subscriber
	{
	public:
		CManaWidget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const std::string& aName);
		~CManaWidget();

		void Init();
		void SetManaPercent(const float aManaPercent);

		eMessageReturn Recieve(const Message& aMessage) override;

	private:
		float myManaPercent;
	};
}
