#include "stdafx.h"
#include "Navmesh.h"
#include "ObjLoader.h"
#include "Engine.h"
#include "ShaderManager.h"
#include "ModelManager.h"

#include "Renderer.h"


CNavmesh::CNavmesh()
{
	myEdges.Init(128);
	myPoints.Init(128);
	myTriangles.Init(128);
	myModel = nullptr;
}


CNavmesh::~CNavmesh()
{
}

void CNavmesh::Render()
{
	SRenderNavmeshMessage* msg = new SRenderNavmeshMessage();
	msg->myTransformation = CU::Matrix44f::Identity;
	msg->myModel = myModel;
	RENDERER.AddRenderMessage(msg);
}

void CNavmesh::LoadFromFile(const char * aFilePath)
{
	
	
	ObjLoader::SObjLoaderModel loadedModel;
	ObjLoader::LoadFromFile(aFilePath, loadedModel);

	BuildNavmeshFromOBJ(loadedModel);

	myIndices.Init(64);
	for (int i = 0; i < loadedModel.FaceList.Size(); ++i)
	{
		myIndices.Add(loadedModel.FaceList[i].myVerteces[0].VertexIndex);
		myIndices.Add(loadedModel.FaceList[i].myVerteces[1].VertexIndex);
		myIndices.Add(loadedModel.FaceList[i].myVerteces[2].VertexIndex);
	}

	CreateModel();

}

void CNavmesh::CreateModel()
{
	myModel = new CModel(); 
	int blueprint = 1; 

	ID3D11VertexShader* vertexShader = SHADERMGR->LoadVertexShader(L"Shaders/vertex_shader.fx", blueprint);
	ID3D11PixelShader* pixelShader = SHADERMGR->LoadPixelShader(L"Shaders/pixel_shader.fx", blueprint);
	ID3D11InputLayout* inputLayout = SHADERMGR->LoadInputLayout(L"Shaders/vertex_shader.fx", blueprint);
	D3D_PRIMITIVE_TOPOLOGY topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	CEffect* effect = new CEffect(vertexShader, pixelShader, nullptr, inputLayout, topology);
	myModelVertices.Init(myPoints.Size());
	for (int i = 0; i < myPoints.Size(); ++i)
	{
		SVertexDataCube vertex;
		vertex.position = myPoints[i].Position;
		vertex.position.w = 1.0f;

		myModelVertices.Add(vertex);
	}

	myModel->Initialize(effect, nullptr, myModelVertices, myIndices);
}

void CNavmesh::BuildNavmeshFromOBJ(const ObjLoader::SObjLoaderModel & aModel)
{
	for (int i = 0; i < aModel.VerticesList.Size(); ++i)
	{
		//mebe memcpy?
		myPoints.Add(SNavmeshPoint(aModel.VerticesList[i]));
		myPoints.GetLast().Index = myPoints.Size() - 1;
	}

	for (int i = 0; i < aModel.FaceList.Size(); ++i)
	{
		BuildEdgesFromFace(aModel.FaceList[i]);
	}
}

void CNavmesh::BuildEdgesFromFace(const ObjLoader::SObjLoaderFace & aFace)
{
	SNavmeshEdge first;
	SNavmeshEdge second;
	SNavmeshEdge third;

	first.FirstVertex = &myPoints[aFace.myVerteces[0].VertexIndex];
	first.SecondVertex = &myPoints[aFace.myVerteces[1].VertexIndex];

	second.FirstVertex = &myPoints[aFace.myVerteces[1].VertexIndex];
	second.SecondVertex = &myPoints[aFace.myVerteces[2].VertexIndex];

	third.FirstVertex = &myPoints[aFace.myVerteces[2].VertexIndex];
	third.SecondVertex = &myPoints[aFace.myVerteces[0].VertexIndex];

	unsigned int edgeIndices[3];

	edgeIndices[0] = myEdges.Find(first);
	edgeIndices[1] = myEdges.Find(second);
	edgeIndices[2] = myEdges.Find(third);


	if (edgeIndices[0] == myEdges.FoundNone)
	{
		int index = myEdges.Size();
		myEdges.Add(first);
		myEdges.GetLast().Index = index;
		edgeIndices[0] = myEdges.Size() - 1;
	}
	if (edgeIndices[1] == myEdges.FoundNone)
	{
		int index = myEdges.Size();
		myEdges.Add(second);
		myEdges.GetLast().Index = index;
		edgeIndices[1] = myEdges.Size() - 1;
	}
	if (edgeIndices[2] == myEdges.FoundNone)
	{
		int index = myEdges.Size();
		myEdges.Add(third);
		myEdges.GetLast().Index = index;
		edgeIndices[2] = myEdges.Size() - 1;
	}

	SNavmeshTriangle tris;
	tris.Edges[0] = &myEdges[edgeIndices[0]];
	tris.Edges[1] =	&myEdges[edgeIndices[1]];
	tris.Edges[2] =	&myEdges[edgeIndices[2]];

	int index = myTriangles.Size();
	myTriangles.Add(tris);
	myTriangles.GetLast().Index = index;

	SNavmeshTriangle* trisp = &myTriangles.GetLast();
	myEdges[edgeIndices[0]].AddTriangle(trisp);
	myEdges[edgeIndices[1]].AddTriangle(trisp);
	myEdges[edgeIndices[2]].AddTriangle(trisp);
}
