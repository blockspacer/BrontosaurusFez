#pragma once
#include <WidgetContainer\WidgetContainer.h>

namespace CU
{
	template <typename T>
	class Vector2;
	using Vector2f = Vector2<float>;
}

namespace GUI
{
	class Window : public WidgetContainer
	{
	public:
		Window(const CU::Vector2f& aPosition, const CU::Vector2f& aSize);
		Window(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName);
		~Window();

		void Render() override;
		void OnPositionChanged() override;

		AUTO_IMPLEMENT_SIZE;

	private:
		void Init(const CU::DynamicString& aBackgroundSpritePath, const CU::Vector2f& aBackgroundCoords);
	};
}
