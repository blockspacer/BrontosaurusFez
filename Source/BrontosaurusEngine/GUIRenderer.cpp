#include "stdafx.h"
#include "GUIRenderer.h"

#include "Engine.h"
#include "Model.h"

#include "ShaderManager.h"
#include "FBXLoader.h"
#include "Effect.h"
#include "Surface.h"

#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/StringHelper.h"
#include "ConstBufferTemplate.h"
#include "../GUI/GUIPixelConstantBuffer.h"

struct CGUIRenderer::SGUIModel
{
	CModel myModel;
	std::unordered_map<WidgetID, const CU::Matrix44f*> myWidgetTransformations;
	bool myIsVisible;
};

//ModelHandle CGUIRenderer::CreateModel(const CLoaderMesh* aLoaderMesh, const char* aTexturePath, const WidgetID aWidgetID, const CU::Matrix44f* aWidgetTransformation)
//{
//	CEffect* effect = CreateEffect(aLoaderMesh);
//	CSurface* surface = CreateSurface(aTexturePath);
//
//	ModelHandle handle = myModels.Size();
//	myModelHandles[aWidgetID] = handle;
//
//	myModels.Add(SGUIModel());
//	myModels.GetLast().myModel.Initialize(effect, surface, aLoaderMesh);
//	myModels.GetLast().myWidgetTransformations[aWidgetID] = aWidgetTransformation;
//	myModels.GetLast().myIsVisible = true;
//
//	return handle;
//}
//
//void CGUIRenderer::SetVisibility(const WidgetID aWidgetID, const bool aIsVisible)
//{
//	ModelHandle index = myModelHandles[aWidgetID];
//	myModels[index].myIsVisible = aIsVisible;
//}
//
//void CGUIRenderer::Render()
//{
//	CU::Camera* previousCamera = CEngine::GetInstance()->GetCamera();
//	CEngine::GetInstance()->SetCamera(myCamera);
//
//	for (ModelHandle i = 0; i < myModels.Size(); ++i)
//	{
//		if (myModels[i].myIsVisible == true)
//		{
//			auto& widgetTransforms = myModels[i].myWidgetTransformations;
//			for (auto it = widgetTransforms.begin(); it != widgetTransforms.end(); ++it)
//			{
//				myModels[i].myModel.Render(*it->second, *it->second, nullptr, nullptr);
//			}
//		}
//	}
//
//	CEngine::GetInstance()->SetCamera(previousCamera);
//}

CGUIRenderer::CGUIRenderer()
	//: myModels(4)
{
}

CGUIRenderer::~CGUIRenderer()
{
}

//CEffect* CGUIRenderer::CreateEffect(const CLoaderMesh* aLoaderMesh)
//{
//	struct ID3D11VertexShader* vertexShader = CEngine::GetInstance()->GetShaderManager()->LoadVertexShader(L"Shaders/gui_shader.fx", aLoaderMesh->myShaderType);
//	struct ID3D11InputLayout* inputLayout = CEngine::GetInstance()->GetShaderManager()->LoadInputLayout(L"Shaders/gui_shader.fx", aLoaderMesh->myShaderType);
//	struct ID3D11PixelShader* pixelShader = CEngine::GetInstance()->GetShaderManager()->LoadPixelShader(L"Shaders/gui_shader.fx", aLoaderMesh->myShaderType);
//
//	CEffect* effect = new CEffect(vertexShader, pixelShader, nullptr, inputLayout, D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	
//	return effect;
//}
//
//CSurface* CGUIRenderer::CreateSurface(const char* aTexturePath)
//{
//	wchar_t buffer[512];
//	CU::GrowingArray<const wchar_t*> texturePaths(1);
//	texturePaths.Add(CU::CharToWChar(buffer, aTexturePath));
//	CSurface* surface = new CSurface(texturePaths);
//
//	return surface;
//}
