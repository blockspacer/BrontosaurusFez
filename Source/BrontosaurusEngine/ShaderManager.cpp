#include "stdafx.h"
#include "ShaderManager.h"
#include "VertexStructs.h"
#include "EffectHelper.h"
#include "Engine.h"

CShaderManager::CShaderManager() //precompiled shaders later
{
}

CShaderManager::~CShaderManager()
{
	//UMapDelete<unsigned int, ID3D11VertexShader*>(myVertexShaders);
	//UMapDelete<unsigned int, ID3D11PixelShader*>(myPixelShaders);
	//UMapDelete<unsigned int, ID3D11GeometryShader*>(myGeometryShaders);

	for (auto it = myVertexShaders.begin(); it != myVertexShaders.end(); it++)
	{
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			SAFE_RELEASE(it2->second);
		}
	}

	for (auto it = myPixelShaders.begin(); it != myPixelShaders.end(); it++)
	{
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			SAFE_RELEASE(it2->second);
		}
	}

	for (auto it = myGeometryShaders.begin(); it != myGeometryShaders.end(); it++)
	{
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			SAFE_RELEASE(it2->second);
		}
	}

	for (auto it = myInputLayouts.begin(); it != myInputLayouts.end(); it++)
	{
		SAFE_RELEASE(it->second);
	}
}

ID3D11VertexShader * CShaderManager::LoadVertexShader(std::wstring aShaderPath,  unsigned int aShaderBlueprint)
{
	if (myVertexShaders.find(aShaderPath) == myVertexShaders.end())
	{
		std::unordered_map<unsigned int, ID3D11VertexShader*> vertexMap;
		myVertexShaders[aShaderPath] = vertexMap;
	}

	if (myVertexShaders[aShaderPath].find(aShaderBlueprint) == myVertexShaders[aShaderPath].end())
	{
		ID3D11VertexShader* newShader = CompileVertexShader(aShaderPath, aShaderBlueprint);
		myVertexShaders[aShaderPath][aShaderBlueprint] = newShader;
	}

	return myVertexShaders[aShaderPath][aShaderBlueprint];
}

ID3D11PixelShader* CShaderManager::LoadPixelShader(std::wstring aShaderPath, unsigned int aShaderBlueprint)
{
	if (myPixelShaders.find(aShaderPath) == myPixelShaders.end())
	{
		std::unordered_map<unsigned int, ID3D11PixelShader*> pixelMap;
		myPixelShaders[aShaderPath] = pixelMap;
	}

	if (myPixelShaders[aShaderPath].find(aShaderBlueprint) == myPixelShaders[aShaderPath].end())
	{
		ID3D11PixelShader* newShader = CompilePixelShader(aShaderPath, aShaderBlueprint);
		myPixelShaders[aShaderPath][aShaderBlueprint] = newShader;
	}

	return myPixelShaders[aShaderPath][aShaderBlueprint];
}

ID3D11GeometryShader * CShaderManager::LoadGeometryShader(std::wstring aShaderPath, unsigned int aShaderBlueprint)
{
	if (myGeometryShaders.find(aShaderPath) == myGeometryShaders.end())
	{
		std::unordered_map<unsigned int, ID3D11GeometryShader*> geometryMap;
		myGeometryShaders[aShaderPath] = geometryMap;
	}

	if (myGeometryShaders[aShaderPath].find(aShaderBlueprint) == myGeometryShaders[aShaderPath].end())
	{
		ID3D11GeometryShader* newShader = CompileGeometryShader(aShaderPath, aShaderBlueprint);
		myGeometryShaders[aShaderPath][aShaderBlueprint] = newShader;
	}

	return myGeometryShaders[aShaderPath][aShaderBlueprint];

}

ID3D11InputLayout * CShaderManager::LoadInputLayout(std::wstring aShaderPath, unsigned int aShaderBlueprint)
{
	if (myInputLayouts.find(aShaderBlueprint) == myInputLayouts.end())
	{
		LoadVertexShader(aShaderPath, aShaderBlueprint);
	}

	return myInputLayouts.at(aShaderBlueprint);
}

ID3D11VertexShader* CShaderManager::CompileVertexShader(std::wstring aString, unsigned int aDataFlags)
{
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11VertexShader* vertexShader;
	ID3D10Blob* vertexBlob = nullptr;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	std::string shaderFunction = "VS_";
	shaderFunction += GetInputLayoutType(aDataFlags, inputLayoutDesc);

	std::string errorMsg;
	bool bresult;
	HRESULT result;

	bresult = EffectHelper::CompileShader(aString.c_str(), shaderFunction.c_str() , "vs_5_0", vertexBlob, errorMsg);
	CHECK_BOOL_RESULT(bresult, errorMsg.c_str()); // TODO: DL_FATAL_ERROR that crashes everytime, be kihnd to folks gpu

	result = CEngine::GetInstance()->GetFramework()->GetDevice()->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &vertexShader);
	CHECK_RESULT(result, "Failed to create Vertex Shader.");

	inputLayout = CreateInputLayout(inputLayout, vertexBlob, inputLayoutDesc);
	myInputLayouts[aDataFlags] = inputLayout;

	vertexBlob->Release();
	return vertexShader;
}

ID3D11PixelShader* CShaderManager::CompilePixelShader(std::wstring aString, unsigned int aDataFlags)
{
	ID3D10Blob* pixelBlob = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	std::string shaderFunction = "PS_";
	shaderFunction += GetInputLayoutType(aDataFlags, inputLayoutDesc);

	std::string errorMsg;
	bool bresult;

	bresult = EffectHelper::CompileShader(aString.c_str(), shaderFunction.c_str(), "ps_5_0", pixelBlob, errorMsg);
	CHECK_BOOL_RESULT(bresult, errorMsg.c_str());

	HRESULT result;
	result = CEngine::GetInstance()->GetFramework()->GetDevice()->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &pixelShader);
	CHECK_RESULT(result, "Failed to create Pixel Shader.");

	pixelBlob->Release();
	return pixelShader;
}

ID3D11GeometryShader * CShaderManager::CompileGeometryShader(std::wstring aString, unsigned int aDataFlags)
{
	ID3D10Blob* geometryBlob = nullptr;
	ID3D11GeometryShader* geometryShader;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	std::string shaderFunction = "GS_";
	shaderFunction += GetInputLayoutType(aDataFlags, inputLayoutDesc);

	std::string errorMsg;
	bool bresult;

	bresult = EffectHelper::CompileShader(aString.c_str(), shaderFunction.c_str(), "gs_5_0", geometryBlob, errorMsg);
	CHECK_BOOL_RESULT(bresult, errorMsg.c_str());

	HRESULT result;
	result = FRAMEWORK->GetDevice()->CreateGeometryShader(geometryBlob->GetBufferPointer(), geometryBlob->GetBufferSize(), NULL, &geometryShader);

	geometryBlob->Release();
	return geometryShader;
}

//returns string and sets the layout. eehehe
std::string CShaderManager::GetInputLayoutType(unsigned int aShaderBlueprint, std::vector<D3D11_INPUT_ELEMENT_DESC>& aInputLayout)
{
	std::string layoutName = "";

	if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_Position)
	{
		layoutName += "Pos";
		EffectHelper::CreateLayout(aInputLayout, "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	}
	else if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_Sprite2D)
	{
		layoutName += "Sprite";
		EffectHelper::CreateLayout(aInputLayout, "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);

		return layoutName;
	}
	else if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_Text2D)
	{
		layoutName += "Text";
		EffectHelper::CreateLayout(aInputLayout, "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
		EffectHelper::CreateLayout(aInputLayout, "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_INPUT_PER_VERTEX_DATA, 0);
		EffectHelper::CreateLayout(aInputLayout, "SCALE", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_INPUT_PER_VERTEX_DATA, 0);
		return layoutName;
	}
	else
	{
		assert(false && "You need a position!");
	}
	if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_Normal)
	{
		layoutName += "Norm";
		EffectHelper::CreateLayout(aInputLayout, "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	}
	if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_BinormTan)
	{
		layoutName += "BinormTan";
		EffectHelper::CreateLayout(aInputLayout, "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
		EffectHelper::CreateLayout(aInputLayout, "BITANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	}
	if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_UV)
	{
		layoutName += "Tex";
		EffectHelper::CreateLayout(aInputLayout, "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	}
	if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_Size)
	{
		layoutName += "Size";
		EffectHelper::CreateLayout(aInputLayout, "SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	}
	if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_Alpha)
	{
		layoutName += "Alpha";
		EffectHelper::CreateLayout(aInputLayout, "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	}
	if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_Bones)
	{
		layoutName += "Bones";
		EffectHelper::CreateLayout(aInputLayout, "BONE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
		EffectHelper::CreateLayout(aInputLayout, "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	}
	if (aShaderBlueprint & EModelBluePrint::EModelBluePrint_Color)
	{
		layoutName += "Color";
		EffectHelper::CreateLayout(aInputLayout, "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	}

	return layoutName;
}

ID3D11InputLayout* CShaderManager::CreateInputLayout(ID3D11InputLayout* aLayout, ID3D10Blob* aVertexBlob, std::vector<D3D11_INPUT_ELEMENT_DESC>& aInputLayout)
{
	HRESULT result;
	result = CEngine::GetInstance()->GetFramework()->GetDevice()->CreateInputLayout(&aInputLayout[0], static_cast<UINT>(aInputLayout.size()), aVertexBlob->GetBufferPointer(), aVertexBlob->GetBufferSize(), &aLayout);
	CHECK_RESULT(result, "Failed to create inputlayout.");
	return aLayout;
}


