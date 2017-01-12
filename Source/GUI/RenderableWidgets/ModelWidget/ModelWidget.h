#pragma once
//#include <Widget.h>
#include "../../Widget/Widget.h"
#include "../CommonUtilities/matrix44.h"

class CLoaderMesh;
class CModelInstance;

struct SPixelConstantBuffer;

namespace CU
{
	class Camera;
}

namespace GUI
{
	class ModelWidget : public Widget
	{
	public:
		friend class ButtonAnimation;

		ModelWidget(CLoaderMesh* aLoaderMesh, const CU::GrowingArray<CU::DynamicString>& aTexturePaths, const CU::Camera& aGUICamera);
		~ModelWidget();

		void Update(const CU::Time& aDeltaTime) override;
		void Render() override;

		void SetInactivePosition(const CU::Vector3f& aPosition);
		void SetVisibility(const bool aVisibility) override;
		void OnMouseEnter(const CU::Vector2f& aMousePosition) override;

		SPixelConstantBuffer& GetPixelConstantBufferStruct();

		void SetFlashTimeToMax();
		inline const CU::Matrix44f& GetOriginalTransformation() const;
		inline bool IsFlashing() const;

		virtual operator class ModelWidget*() override { return this; }

	private:
		void ConvertPosition3DTo2D(const CU::Camera& aGUICamera, const CU::Vector3f& aPosition3D, CU::Vector2f& aPosition2D);

		CU::Matrix44f myOriginalTransformation;

		CModelInstance* myModelInstance;
		SPixelConstantBuffer* myPixelConstantBufferStruct;

		float myMillisecondsLeftSinceMouseEnter;
	};

	inline bool ModelWidget::IsFlashing() const
	{
		return myMillisecondsLeftSinceMouseEnter > 0.f;
	}

	inline const CU::Matrix44f& ModelWidget::GetOriginalTransformation() const
	{
		return myOriginalTransformation;
	}
}
