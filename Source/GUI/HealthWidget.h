#pragma once
#include "WidgetContainer.h"
#include "../PostMaster/Subscriber.h"

namespace GUI
{
	class CHealthWidget : public WidgetContainer, public Subscriber
	{
	public:
		CHealthWidget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const std::string& aName);
		~CHealthWidget();

		void Init();
		void SetHealthPercent(const float aHealthPercent);

		eMessageReturn Recieve(const Message& aMessage) override;

	private:
		float myHealthPercent;
	};
}
