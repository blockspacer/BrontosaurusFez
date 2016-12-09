#pragma once

#include <unordered_map>
//#include "../CommonUtilities/GrowingArray.h"

namespace CU
{
	class Camera;

	template <typename TYPE>
	class Matrix44;
	using Matrix44f = Matrix44<float>;
}

class CModel;
class CEffect;
class CSurface;
class CLoaderMesh;

using ModelHandle = int;
using WidgetID = void*;

class CGUIRenderer
{
	struct SGUIModel;

public:
	friend class CEngine;

	inline void SetCamera(CU::Camera* aCamera);
	//void Render();

	//ModelHandle CreateModel(const CLoaderMesh* aLoaderMesh, const char* aTexturePath, const WidgetID aWidgetID, const CU::Matrix44f* aWidgetTransformation);
	//void SetVisibility(const WidgetID aWidgetID, const bool aIsVisible);

	inline const CU::Camera* GetCamera() const;

private:
	CGUIRenderer();
	~CGUIRenderer();

	//CEffect* CreateEffect(const CLoaderMesh* aLoaderMesh);
	//CSurface* CreateSurface(const char* aTexturePath);

	//CU::GrowingArray<SGUIModel, ModelHandle> myModels;
	//std::unordered_map<WidgetID, ModelHandle> myModelHandles;

	CU::Camera* myCamera;
};

inline void CGUIRenderer::SetCamera(CU::Camera* aCamera)
{
	myCamera = aCamera;
}

inline const CU::Camera* CGUIRenderer::GetCamera() const
{
	return myCamera;
}
