#include "stdafx.h"
#include "ModelLoader.h"
#include "Model.h"
#include "Sprite.h"
#include "ModelShapes.h"
#include "BufferStructs.h"
#include "FBXLoader.h"
#include "Effect.h"
#include "Engine.h"
#include "ShaderManager.h"
#include "Surface.h"

const wchar_t* ConvertStringToWideString(const std::string& aString, std::wstring& aWideString) //remove or move when found real one
{
	aWideString = std::wstring(aString.cbegin(), aString.cend());
	return aWideString.c_str();
}

CModelLoader::CModelLoader()
{

}	

CModelLoader::~CModelLoader()
{
}

CModel* CModelLoader::CreateShape(SShape aShape)
{
	CModel* newModel = new CModel(); //TODO: MEMORY LEAK bc this is dereferenced and copied into myShapeList in model manager :(
	int blueprint = 1; // EModelBluePrint_Color | EModelBluePrint_Position;

	ID3D11VertexShader* vertexShader = SHADERMGR->LoadVertexShader(L"Shaders/vertex_shader.fx", blueprint);
	ID3D11PixelShader* pixelShader = SHADERMGR->LoadPixelShader(L"Shaders/pixel_shader.fx", blueprint);
	ID3D11GeometryShader* geometryShader = nullptr;// CEngine::GetInstance()->GetShaderManager()->LoadGeometryShader(L"Shaders/geometry_shader.fx", blueprint);
	ID3D11InputLayout* inputLayout = SHADERMGR->LoadInputLayout(L"Shaders/vertex_shader.fx", blueprint);
	D3D_PRIMITIVE_TOPOLOGY topology	= D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
	//Put effects in a manager mebe?
	CEffect* effect = new CEffect(vertexShader, pixelShader, geometryShader, inputLayout, topology);

	switch (aShape.shape)
	{
	case eModelShape::eTriangle:
		assert(!"This shape is not implemented, and the function is out commented");
		//CreateTriangle(newModel);
		break;
	case eModelShape::eQuad:
		assert(!"This shape is not implemented, and the function is out commented");
		//CreateQuad(newModel);
		break;
	case eModelShape::eCube:
		CreateCube(newModel);
		break;
	case eModelShape::eSphere:
		CreateSphere(newModel, aShape.size);
		break;
	}

	newModel->Initialize(effect, nullptr);
	return newModel;
}

void CModelLoader::LoadModel(const char * aPath, CModel* aNewModel) //TODO: FIX THIS SHIT.
{
	// MEBE move into model?!
	CFBXLoader loader;
	//CLoaderModel* loadedModel = loader.LoadModel(aPath);
	CLoaderScene scene;
	bool res = loader.LoadModelScene(aPath, scene);

	if (res == false)
	{
		//DL_ASSERT("Failed To load model with path: %s", aPath);
		return;
	}

	if (scene.myMeshes.Size() == 0)
	{
		DL_ASSERT("Could not find meshes in file: %s", aPath);
		return;
	}



	
	std::string modelPath = aPath;
	std::wstring directory = std::wstring(modelPath.begin(), modelPath.end());
	directory = directory.substr(0, directory.find_last_of('/') + 1);


	CU::GrowingArray<const wchar_t*> texturePaths;
	texturePaths.Init(8);

	//diffuse
	std::string str = scene.myTextures[0].c_str();
	std::wstring diffuse = std::wstring(str.begin(), str.end());
	diffuse = directory + diffuse;
	const wchar_t* diffusePath = diffuse.c_str();

	//Roughness
	str = scene.myTextures[1].c_str();
	std::wstring roughness = std::wstring(str.begin(), str.end());
	roughness = directory + roughness;
	const wchar_t* roughnessPath = roughness.c_str();

	//AO
	str = scene.myTextures[2].c_str();
	std::wstring ambientOclution = std::wstring(str.begin(), str.end());
	ambientOclution = directory + ambientOclution;
	const wchar_t* AOPath = ambientOclution.c_str();
	
	//Emissive
	str = scene.myTextures[3].c_str();
	std::wstring emissive = std::wstring(str.begin(), str.end());
	emissive = directory + emissive;
	const wchar_t* emissivePath = emissive.c_str();

	//normal
	str = scene.myTextures[5].c_str();
	std::wstring normal = std::wstring(str.begin(), str.end());
	normal = directory + normal;
	const wchar_t* normalPath = normal.c_str();

	//metalness
	str = scene.myTextures[10].c_str();
	std::wstring metalness = std::wstring(str.begin(), str.end());
	metalness = directory + metalness;
	const wchar_t* metalnessPath = metalness.c_str();

	if (scene.myTextures[0] == "")
		diffusePath = L"Error.dds";
	texturePaths.Add(diffusePath);

	if (scene.myTextures[1] == "")
		roughnessPath = L"";
	texturePaths.Add(roughnessPath);

	if (scene.myTextures[2] == "")
		AOPath = L"";
	texturePaths.Add(AOPath);

	if (scene.myTextures[3] == "")
		emissivePath = L"";
	texturePaths.Add(emissivePath);

	if (scene.myTextures[5] == "")
		normalPath = L"";
	texturePaths.Add(normalPath);

	if (scene.myTextures[10] == "")
		metalnessPath = L"";
	texturePaths.Add(metalnessPath);
	

	int shaderType = scene.myMeshes[0]->myShaderType;
	std::string shaderPath = scene.myMeshes[0]->myShaderFile.c_str();
	std::wstring wShaderPath = std::wstring(shaderPath.begin(), shaderPath.end());

	//wShaderPath = L"";
	ID3D11VertexShader* vertexShader = SHADERMGR->LoadVertexShader(wShaderPath != L"" ? directory + wShaderPath + L".fx" : L"Shaders/vertex_shader.fx", shaderType);
	ID3D11PixelShader* pixelShader = SHADERMGR->LoadPixelShader(wShaderPath != L"" ? directory + wShaderPath + L".fx" : L"Shaders/pixel_shader.fx", shaderType);


	ID3D11GeometryShader* geometryShader = nullptr;// CEngine::GetInstance()->GetShaderManager()->LoadGeometryShader(L"Shaders/geometry_shader.fx", shaderType);
	ID3D11InputLayout* inputLayout = SHADERMGR->LoadInputLayout(wShaderPath != L"" ? directory + wShaderPath + L".fx" : L"Shaders/vertex_shader.fx", shaderType);


	D3D_PRIMITIVE_TOPOLOGY topology			= D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//Put effects in a manager mebe?
	CEffect* effect = new CEffect(vertexShader, pixelShader, geometryShader, inputLayout, topology);
	CSurface* surface = new CSurface(texturePaths);
	
	if (scene.mySphereColData.radius > 0.0f)
	{
		aNewModel->mySphereColData = /*SSphereColData*/(scene.mySphereColData);
	}
	aNewModel->Initialize(effect, surface, scene.myMeshes);
	aNewModel->SetScene(scene.myScene);
}

//void CModelLoader::CreateTriangle(CModel* /*aModel*/)
//{
//	CU::GrowingArray<SVertexData>& modelVertices = aModel->GetVertices();
//	CU::GrowingArray<unsigned int>& modelIndices = aModel->GetIndices();
//
//
//	modelVertices.Init(36);
//
//	modelVertices.Add(SVertexData());
//	modelVertices[0].position.x = 0.0f;
//	modelVertices[0].position.y = -0.8f;
//	modelVertices[0].position.z = 0.5f;
//	modelVertices[0].position.w = 1.0f;
//
//	modelVertices.Add(SVertexData());
//	modelVertices[1].position.x = -0.8f;
//	modelVertices[1].position.y = 0.8f;
//	modelVertices[1].position.z = 0.5f;
//	modelVertices[1].position.w = 1.0f;
//
//	modelVertices.Add(SVertexData());
//	modelVertices[2].position.x = 0.8f;
//	modelVertices[2].position.y = 0.8f;
//	modelVertices[2].position.z = 0.5f;
//	modelVertices[2].position.w = 1.0f;
//
//	modelIndices.Add(0);
//	modelIndices.Add(1);
//	modelIndices.Add(2);
//}

//void CModelLoader::CreateQuad(CModel* /*aModel*/)
//{
//	CU::GrowingArray<SVertexData>& modelVertices = aModel->GetVertices();
//	CU::GrowingArray<unsigned int>& modelIndices = aModel->GetIndices();
//
//	CU::Vector4f topLeft = {-0.5f, -0.5f, 0.5f, 1.0f};
//	CU::Vector4f topRight = { 0.5f, -0.5f, 0.5f, 1.0f };
//	CU::Vector4f botLeft = { -0.5f, 0.5f, 0.5f, 1.0f };
//	CU::Vector4f botRight = { 0.5f, 0.5f, 0.5f, 1.0f };
//
//	modelVertices.Add(SVertexData());
//	modelVertices[0].position = topLeft;
//
//	modelVertices.Add(SVertexData());
//	modelVertices[1].position = botLeft;
//
//	modelVertices.Add(SVertexData());
//	modelVertices[2].position = botRight;
//
//	modelVertices.Add(SVertexData());
//	modelVertices[3].position = topRight;
//
//	modelIndices.Add(0);
//	modelIndices.Add(1);
//	modelIndices.Add(3);
//
//	modelIndices.Add(3);
//	modelIndices.Add(1);
//	modelIndices.Add(2);
//}

void CModelLoader::CreateCube(CModel* aModel)
{
	CU::GrowingArray<SVertexDataCube> modelVertices;// = aModel->GetVertices();
	CU::GrowingArray<unsigned int> modelIndices;// = aModel->GetIndices();

	modelVertices.Init(8);
	modelIndices.Init(36);

	float size = 500;

	modelVertices.Add(SVertexDataCube());
	modelVertices[0].position.x = -size;
	modelVertices[0].position.y = size;
	modelVertices[0].position.z = -size;
	modelVertices[0].position.w = 1.0f;

	//modelVertices[0].UV.u = 0.0f;
	//modelVertices[0].UV.v = 0.0f;

	modelVertices.Add(SVertexDataCube());
	modelVertices[1].position.x = -size;
	modelVertices[1].position.y = size;
	modelVertices[1].position.z = size;
	modelVertices[1].position.w = 1.0f;

	//modelVertices[1].UV.u = 1.0f;
	//modelVertices[1].UV.v = 0.0f;

	modelVertices.Add(SVertexDataCube());
	modelVertices[2].position.x = size;
	modelVertices[2].position.y = size;
	modelVertices[2].position.z = size;
	modelVertices[2].position.w = 1.0f;

	//modelVertices[2].UV.u = 0.0f;
	//modelVertices[2].UV.v = 0.0f;

	modelVertices.Add(SVertexDataCube());
	modelVertices[3].position.x = size;
	modelVertices[3].position.y = size;
	modelVertices[3].position.z = -size;
	modelVertices[3].position.w = 1.0f;

	//modelVertices[3].UV.u = 1.0f;
	//modelVertices[3].UV.v = 0.0f;

	// BOT
	modelVertices.Add(SVertexDataCube());
	modelVertices[4].position.x = -size;
	modelVertices[4].position.y = -size;
	modelVertices[4].position.z = -size;
	modelVertices[4].position.w = 1.0f;

	//modelVertices[4].UV.u = 0.0f;
	//modelVertices[4].UV.v = 1.0f;

	modelVertices.Add(SVertexDataCube());
	modelVertices[5].position.x = -size;
	modelVertices[5].position.y = -size;
	modelVertices[5].position.z = size;
	modelVertices[5].position.w = 1.0f;

	//modelVertices[5].UV.u = 1.0f;
	//modelVertices[5].UV.v = 1.0f;

	modelVertices.Add(SVertexDataCube());
	modelVertices[6].position.x = size;
	modelVertices[6].position.y = -size;
	modelVertices[6].position.z = size;
	modelVertices[6].position.w = 1.0f;

	//modelVertices[6].UV.u = 0.0f;
	//modelVertices[6].UV.v = 1.0f;

	modelVertices.Add(SVertexDataCube());
	modelVertices[7].position.x = size;
	modelVertices[7].position.y = -size;
	modelVertices[7].position.z = -size;
	modelVertices[7].position.w = 1.0f;

	//modelVertices[7].UV.u = 1.0f;
	//modelVertices[7].UV.v = 1.0f;


	modelIndices.Add(0);
	modelIndices.Add(1);
	modelIndices.Add(2);
	modelIndices.Add(0);
	modelIndices.Add(2);
	modelIndices.Add(3);

	modelIndices.Add(4);
	modelIndices.Add(6);
	modelIndices.Add(5);
	modelIndices.Add(4);
	modelIndices.Add(7);
	modelIndices.Add(6);
	
	modelIndices.Add(4);
	modelIndices.Add(5);
	modelIndices.Add(1);
	modelIndices.Add(4);
	modelIndices.Add(1);
	modelIndices.Add(0);
	
	modelIndices.Add(6);
	modelIndices.Add(7);
	modelIndices.Add(3);
	modelIndices.Add(6);
	modelIndices.Add(3);
	modelIndices.Add(2);

	modelIndices.Add(4);
	modelIndices.Add(0);
	modelIndices.Add(3);
	modelIndices.Add(4);
	modelIndices.Add(3);
	modelIndices.Add(7);
	
	modelIndices.Add(6);
	modelIndices.Add(2);
	modelIndices.Add(1);
	modelIndices.Add(6);
	modelIndices.Add(1);
	modelIndices.Add(5);

	aModel->InitBuffers(modelVertices, modelIndices);
}

void CModelLoader::CreateSphere(CModel* aModel, const float aSize)
{
	CU::GrowingArray<SVertexDataCube> modelVertices;
	modelVertices.Init(3000);

	int radius = static_cast<int>(aSize);
	const float PI = 3.1415926535f;
	int index = 0;


	for (float theta = 0.; theta < PI; theta += PI / 10.)
	{
		for (float phi = 0.; phi < 2 * PI; phi += PI / 10.)
		{

			CU::Vector4d point;
			point.x = radius * cos(phi) * sin(theta);
			point.y = radius * sin(phi) * sin(theta);
			point.z = radius            * cos(theta);
			point.w = 1.f;
			modelVertices.Add(SVertexDataCube());
			modelVertices[index].position = CU::Vector4f(point);
			index++;
		}
	}
	aModel->InitBuffers(modelVertices);
}

