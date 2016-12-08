#pragma once
#include "Widget.h"

namespace GUI
{
	class DraggableArea : public Widget
	{
	public:
		DraggableArea(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, Widget* aParent, const bool aIsVisible = false);
		~DraggableArea();

		void OnMouseDrag(const CU::Vector2f& aDisplacement, const CU::eMouseButtons aButton) override;
		AUTO_IMPLEMENT_SIZE;
	};
}
