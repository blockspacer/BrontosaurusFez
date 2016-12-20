#pragma once
#include <WidgetContainer/WidgetContainer.h>

namespace GUI
{
	class BarWidget : public WidgetContainer
	{
	public:
		BarWidget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, const bool aIsVisible, const int aMaxValue, const int aStartValue);
		~BarWidget();

		void Update(const CU::Time& aDeltaTime) override;
		//void ValueChanged(const int aValue) override;
		//void SetMaxValue(const int aValue) override;

	private:
		void FlashAndRemove();
		Widget* GetModelChild();

		Widget* myCachedChild;
		int myProgressValue;
		int myMaxValue;
		float myMillisecondsLeftSinceMouseEnter;
	};
}
