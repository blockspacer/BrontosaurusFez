#pragma once
#include <Widget.h>

class ModelComponent;
class CGameObject;

namespace GUI
{
	class SpriteWidget : public Widget
	{
	public:
		SpriteWidget(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::DynamicString& aName, ModelComponent* aModelComponent, const bool aIsVisible = true);
		~SpriteWidget();

		void Render() override;

		AUTO_IMPLEMENT_SIZE;

	private:
		CGameObject* myRenderObject;
	};
}
