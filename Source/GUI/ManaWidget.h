#pragma once
#include "WidgetContainer.h"
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

		inline float GetManaPercent() const;

	private:
		float myManaPercent;
	};

	inline float CManaWidget::GetManaPercent() const
	{
		return myManaPercent;
	}
}
