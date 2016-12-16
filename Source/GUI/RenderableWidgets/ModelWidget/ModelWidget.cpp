#include "stdafx.h"
#include "ModelWidget.h"

#include "../BrontosaurusEngine/Engine.h"
#include "../BrontosaurusEngine/FBXLoader.h"

#include "../BrontosaurusEngine/Renderer.h"
#include "../BrontosaurusEngine/ModelManager.h"
#include "../BrontosaurusEngine/ModelInstance.h"

#include "../CommonUtilities/Camera.h"
#include "GUIPixelConstantBuffer.h"

const float MillisecondsToFlash = 500.f;

namespace GUI
{
	ModelWidget::ModelWidget(CLoaderMesh* aLoaderMesh, const CU::GrowingArray<CU::DynamicString>& aTexturePaths, const CU::Camera& aGUICamera)
		: Widget(CU::Vector2f::Zero, CU::Vector2f::One, aLoaderMesh->myName, true)
		, myModelInstance(nullptr)
		, myPixelConstantBufferStruct(nullptr)
		, myMillisecondsLeftSinceMouseEnter(0.f)
	{
		CModel* model = MODELMGR->LoadGUIModel(aLoaderMesh, aTexturePaths);
		myModelInstance = new CModelInstance(model, aLoaderMesh->myTransformation);
		myPixelConstantBufferStruct = new SPixelConstantBuffer();
		myOriginalTransformation = aLoaderMesh->myTransformation;

		CU::Vector2f screenMinPosition;
		ConvertPosition3DTo2D(aGUICamera, aLoaderMesh->myMinPoint, screenMinPosition);

		CU::Vector2f screenMaxPosition;
		ConvertPosition3DTo2D(aGUICamera, aLoaderMesh->myMaxPoint, screenMaxPosition);

		SetWorldPosition(CU::Vector2f(screenMinPosition.x, 1.f - screenMaxPosition.y));
		SetSize(screenMaxPosition - screenMinPosition);
		AddDebugLines();
	}

	//ModelWidget::ModelWidget(CModelInstance* aModelInstance, const CU::DynamicString& aName)
	//	: Widget(CU::Vector2f::Zero, CU::Vector2f::One, aName, true)
	//	, myModelInstance(nullptr)
	//	, myPixelConstantBufferStruct(nullptr)
	//	, myMillisecondsLeftSinceMouseEnter(0.f)
	//{
	//	myModelInstance = aModelInstance;
	//	myPixelConstantBufferStruct = new SPixelConstantBuffer();
	//	myOriginalTransformation = aModelInstance->GetTransformation();

	//	CU::Vector2f screenMinPosition;
	//	
	//	ConvertPosition3DTo2D(aModelInstance->GetModel()->GetBoundingBox().myMinPos, screenMinPosition);

	//	CU::Vector2f screenMaxPosition;
	//	ConvertPosition3DTo2D(aModelInstance->GetModel()->GetBoundingBox().myMaxPos, screenMaxPosition);

	//	SetWorldPosition(CU::Vector2f(screenMinPosition.x, 1.f - screenMaxPosition.y));
	//	SetSize(screenMaxPosition - screenMinPosition);
	//	AddDebugLines();
	//}

	ModelWidget::~ModelWidget()
	{
		SAFE_DELETE(myModelInstance);
		SAFE_DELETE(myPixelConstantBufferStruct);
	}

	void ModelWidget::SetInactivePosition(const CU::Vector3f& aPosition)
	{
		if (aPosition == aPosition.Zero)
		{
			myModelInstance->SetTransformation(myOriginalTransformation);
		}
		else
		{
			CU::Matrix44f newTransformation(myOriginalTransformation);
			newTransformation.myPosition += aPosition;
			myModelInstance->SetTransformation(newTransformation);
		}
	}

	void ModelWidget::SetVisibility(const bool aVisibility)
	{
		Widget::SetVisibility(aVisibility);
		myModelInstance->SetVisibility(aVisibility);
	}

	SPixelConstantBuffer& ModelWidget::GetPixelConstantBufferStruct()
	{
		assert(myPixelConstantBufferStruct != nullptr && "Pixel constant buffer struct is NULL in ModelWidet :-(");
		return *myPixelConstantBufferStruct;
	}

	void ModelWidget::Update(const CU::Time& aDeltaTime)
	{
		if (myMillisecondsLeftSinceMouseEnter > 0.f)
		{
			myMillisecondsLeftSinceMouseEnter = max(0.f, myMillisecondsLeftSinceMouseEnter - aDeltaTime.GetMilliseconds());

			myPixelConstantBufferStruct->myValues[SPixelConstantBuffer::eMember::eFlashButton] = myMillisecondsLeftSinceMouseEnter / MillisecondsToFlash;
		}
	}

	void ModelWidget::OnMouseEnter(const CU::Vector2f& aMousePosition)
	{
		SUPRESS_UNUSED_WARNING(aMousePosition);
		//myMillisecondsLeftSinceMouseEnter = MillisecondsToFlash;
	}

	void ModelWidget::Render()
	{
		if (IsVisible() == true)
		{
			SRenderGUIModelMessage* renderMessage = new SRenderGUIModelMessage();
			if (renderMessage != nullptr)
			{
				renderMessage->myModel = myModelInstance->GetModel();
				renderMessage->myToWorld = myModelInstance->GetTransformation();
				renderMessage->myPixelConstantBufferStruct = *myPixelConstantBufferStruct;

				RENDERER.AddRenderMessage(renderMessage);
			}
		}
	}

	void ModelWidget::SetFlashTimeToMax()
	{
		myMillisecondsLeftSinceMouseEnter = MillisecondsToFlash;
	}

	void ModelWidget::ConvertPosition3DTo2D(const CU::Camera& aGUICamera, const CU::Vector3f& aPosition3D, CU::Vector2f& aPosition2D)
	{
		CU::Vector4f minPosition(aPosition3D);
		minPosition = minPosition * myModelInstance->GetTransformation();
		minPosition = minPosition * aGUICamera.GetInverse();
		minPosition = minPosition * aGUICamera.GetProjection();
		minPosition /= minPosition.w;
		CU::Vector2f screenMinPosition(minPosition.x, minPosition.y);
		screenMinPosition = (screenMinPosition + CU::Vector2f::One) / 2.f;
		aPosition2D = screenMinPosition;
	}
}
