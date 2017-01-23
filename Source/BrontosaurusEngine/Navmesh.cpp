#include "stdafx.h"
#include "Navmesh.h"
#include "ObjLoader.h"
#include "Engine.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include <Plane.h>
#include <Line3D.h>
#include <line.h>
#include <limits>
#include "Renderer.h"
#include <Heap.h>

namespace 
{
	float dist2P2(const CU::Vector3f & p1, const CU::Vector3f & p2)
	{
		return pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2) + pow((p1.z - p2.z), 2);
	}
}



CNavmesh::CNavmesh()
{
	//myEdges.Init(128);
	//myPoints.Init(128);
	//myTriangles.Init(128);
	myModel = nullptr;
	myPositionsModel = nullptr;
	myShouldRender = 1;
}


CNavmesh::~CNavmesh()
{
}

void CNavmesh::Render()
{
	if (!myShouldRender)
		return;

	if (myModel != nullptr)
	{
		SRenderNavmeshMessage* msg = new SRenderNavmeshMessage();
		msg->myTransformation = CU::Matrix44f::Identity;
		msg->myModel = myModel;
		RENDERER.AddRenderMessage(msg);
	}

	//SRenderNavmeshMessage* msg2 = new SRenderNavmeshMessage();
	//msg2->myTransformation = CU::Matrix44f::Identity;
	//msg2->myModel = myPositionsModel;
	//RENDERER.AddRenderMessage(msg2);

}

void CNavmesh::LoadFromFile(const char * aFilePath)
{
	
	
	ObjLoader::SObjLoaderModel loadedModel;
	ObjLoader::LoadFromFile(aFilePath, loadedModel);


	myEdges.Init(loadedModel.FaceList.Size() * 3);
	myPoints.Init(loadedModel.VerticesList.Size());
	myTriangles.Init(loadedModel.FaceList.Size() * 3);
	myIndices.Init(loadedModel.FaceList.Size() * 3);

	BuildNavmeshFromOBJ(loadedModel);
	for (int i = 0; i < loadedModel.FaceList.Size(); ++i)
	{
		myIndices.Add(loadedModel.FaceList[i].myVerteces[0].VertexIndex);
		myIndices.Add(loadedModel.FaceList[i].myVerteces[1].VertexIndex);
		myIndices.Add(loadedModel.FaceList[i].myVerteces[2].VertexIndex);
	}

	CreateModel();

}

CNavmesh::SPath CNavmesh::CalculatePath(const SNavmeshNode & aStartPoint, const SNavmeshNode & aEndPoint)
{
	CU::GrowingArray<CU::Vector3f, unsigned int, false> waypoints(16);
	CU::Heap<SNavmeshNode*, NavmeshLesser> open;
	CU::GrowingArray<SNavmeshNode*> closed(myTriangles.Size() + 2);
	CU::GrowingArray<SNavmeshNode> nodes(myTriangles.Size() + 2);
	std::unordered_map<SNavmeshTriangle*, SNavmeshNode*> triangleNodeMap;

	nodes.Add(aStartPoint);
	SNavmeshNode* start = &nodes.GetLast();
	nodes.Add(aEndPoint);
	SNavmeshNode* end = &nodes.GetLast();
	
	open.Enqueue(start);


	for (int i = 0; i < myTriangles.Size(); ++i)
	{
		CreateNode(nodes, myTriangles[i], aEndPoint, triangleNodeMap);
	}
	BuildNodeNetwork(nodes, triangleNodeMap);
	
	start->myNeighbours.Clear();
	start->myNeighbours.Add(triangleNodeMap[start->myTriangle]);
	
	triangleNodeMap[end->myTriangle]->myNeighbours.Add(end);

	while (open.IsEmpty() == false)
	{
		SNavmeshNode* current = open.Dequeue();
		closed.Add(current);

		if (current == end)
		{
			while (current != nullptr)
			{
				waypoints.Insert(0, current->myPosition);
				current = current->myParent;
			}

			break; // found end;
		}


		for (int i = 0; i < current->myNeighbours.Size(); ++i)
		{
			SNavmeshNode* neighbour = current->myNeighbours[i];

			if (closed.Find(neighbour) != closed.FoundNone)
			{
				continue;
			}


			float newG = current->myG + CalculateHeuristics(neighbour->myPosition, current->myPosition);

			if (newG < neighbour->myG || open.Find(neighbour) == false)
			{
				neighbour->myH = CalculateHeuristics(neighbour->myPosition, end->myPosition);
				neighbour->myG = newG;

				neighbour->myParent = current;

				if (open.Find(neighbour) == false)
				{
					open.Enqueue(neighbour);
				}
			}
		}
	}

	return SPath{ waypoints };
}

CNavmesh::SNavmeshTriangle & CNavmesh::GetClosestTriangle(const CU::Vector3f & aPosition)
{
	float d2 = 99999999999999.f;
	float temp;
	SNavmeshTriangle* closestTriangle = nullptr;
	for (int i = 0; i < myTriangles.Size(); ++i)
	{
		temp = dist2P2(aPosition, myTriangles[i].CenterPosition);
		if (temp < d2)
		{
			closestTriangle = &myTriangles[i];
			d2 = temp;
		}
	}


	return *closestTriangle;
}

bool CNavmesh::GetPointOnNavmesh(const CU::Vector3f & aOrigin, const CU::Vector3f & aDirection, CNavmesh::SNavmeshTriangle& aIntersectingTriangle, CU::Vector3f& aIntersectionPoint)
{
	SNavmeshTriangle* triangle = nullptr; 
	CU::Vector3f v1;
	CU::Vector3f v2;
	CU::Vector3f v3;
	CU::Vector3f triCentre;

	for (int i = 0; i < myTriangles.Size(); ++i)
	{
		triangle = &myTriangles[i];
		aIntersectingTriangle = myTriangles[i];

		v1 = triangle->Edges[0]->FirstVertex->Position;
		v2 = triangle->Edges[0]->SecondVertex->Position;

		if (triangle->Edges[1]->FirstVertex->Position != v1 &&
			triangle->Edges[1]->FirstVertex->Position != v2)
		{
			v3 = triangle->Edges[1]->FirstVertex->Position;
		}
		else if (triangle->Edges[1]->SecondVertex->Position != v1 &&
			triangle->Edges[1]->SecondVertex->Position != v2)
		{
			v3 = triangle->Edges[1]->SecondVertex->Position;
		}

		CU::Plane<float> plane(v1, v2, v3);

		float d = plane.myNormal.Dot(plane.myPoint);

		if (plane.myNormal.Dot(aDirection) == 0)
		{
			continue; // No intersection, the line is parallel to the plane
		}

		// Compute the X value for the directed line ray intersecting the plane
		float x = (d - plane.myNormal.Dot(aOrigin)) / plane.myNormal.Dot(aDirection);

		// output contact point
		aIntersectionPoint = aOrigin + aDirection.GetNormalized() * x; //Make sure your ray vector is normalized



		triCentre = myTriangles[i].CenterPosition;
		CU::Vector2f Centre2D = {triCentre.x, triCentre.z};
		CU::Vector2f Intersection2D = {aIntersectionPoint.x, aIntersectionPoint.z};

		CU::Line<float> line;
		float tempX1 = triangle->Edges[0]->FirstVertex->Position.x;
		float tempZ1 = triangle->Edges[0]->FirstVertex->Position.z;
		float tempX2 = triangle->Edges[0]->SecondVertex->Position.x;
		float tempZ2 = triangle->Edges[0]->SecondVertex->Position.z;

		line.SetWith2Points({tempX1, tempZ1}, {tempX2, tempZ2});


		if (line.IsInside(Intersection2D) == line.IsInside(Centre2D))
		{
			tempX1 = triangle->Edges[1]->FirstVertex->Position.x;
			tempZ1 = triangle->Edges[1]->FirstVertex->Position.z;
			tempX2 = triangle->Edges[1]->SecondVertex->Position.x;
			tempZ2 = triangle->Edges[1]->SecondVertex->Position.z;
			line.SetWith2Points({ tempX1, tempZ1 }, { tempX2, tempZ2 });


			if (line.IsInside(Intersection2D) == line.IsInside(Centre2D))
			{

				tempX1 = triangle->Edges[2]->FirstVertex->Position.x;
				tempZ1 = triangle->Edges[2]->FirstVertex->Position.z;
				tempX2 = triangle->Edges[2]->SecondVertex->Position.x;
				tempZ2 = triangle->Edges[2]->SecondVertex->Position.z;
				line.SetWith2Points({ tempX1, tempZ1 }, { tempX2, tempZ2 });

				if (line.IsInside(Intersection2D) == line.IsInside(Centre2D))
				{
					DL_PRINT("%i", i);
					return true;
				}
			}
		}
	}

	aIntersectionPoint = CU::Vector3f::Zero;
	return false;
}

bool CNavmesh::IsValid(const CU::Vector3f & aPosition, SNavmeshTriangle *& aIntersectingTriangle, CU::Vector3f& aIntersectingPoint)
{
	return IsValid({ aPosition.x, aPosition.z }, aIntersectingTriangle, aIntersectingPoint);
}

//	aPosition is x and z, to get y and to see if it is within bounds yo
bool CNavmesh::IsValid(const CU::Vector2f & aPosition, SNavmeshTriangle *& aIntersectingTriangle, CU::Vector3f& aIntersectingPoint)
{
	SNavmeshTriangle* triangle = nullptr;
	CU::Line<float> line;


	CU::Vector2f triangleCenter2D;

	float tempX1 = 0.0f;
	float tempZ1 = 0.0f;
	float tempX2 = 0.0f;
	float tempZ2 = 0.0f;

	for (int i = 0; i < myTriangles.Size(); ++i)
	{
		triangle = &myTriangles[i];
		triangleCenter2D = { triangle->CenterPosition.x, triangle->CenterPosition.z };

		tempX1 = triangle->Edges[0]->FirstVertex->Position.x;
		tempZ1 = triangle->Edges[0]->FirstVertex->Position.z;
		tempX2 = triangle->Edges[0]->SecondVertex->Position.x;
		tempZ2 = triangle->Edges[0]->SecondVertex->Position.z;

		line.SetWith2Points({ tempX1, tempZ1 }, { tempX2, tempZ2 });


		if (line.IsInside(aPosition) == line.IsInside(triangleCenter2D))
		{
			tempX1 = triangle->Edges[1]->FirstVertex->Position.x;
			tempZ1 = triangle->Edges[1]->FirstVertex->Position.z;
			tempX2 = triangle->Edges[1]->SecondVertex->Position.x;
			tempZ2 = triangle->Edges[1]->SecondVertex->Position.z;
			line.SetWith2Points({ tempX1, tempZ1 }, { tempX2, tempZ2 });


			if (line.IsInside(aPosition) == line.IsInside(triangleCenter2D))
			{

				tempX1 = triangle->Edges[2]->FirstVertex->Position.x;
				tempZ1 = triangle->Edges[2]->FirstVertex->Position.z;
				tempX2 = triangle->Edges[2]->SecondVertex->Position.x;
				tempZ2 = triangle->Edges[2]->SecondVertex->Position.z;
				line.SetWith2Points({ tempX1, tempZ1 }, { tempX2, tempZ2 });

				if (line.IsInside(aPosition) == line.IsInside(triangleCenter2D))
				{
					CU::Vector3f v1;
					CU::Vector3f v2;
					CU::Vector3f v3;

					CU::Vector3f origin = { aPosition.x, 5000.0f, aPosition.y};
					CU::Vector3f direction = {0.0f, 1.0f, 0.0f};
					CU::Vector3f intersectionPoint;

					v1 = triangle->Edges[0]->FirstVertex->Position;
					v2 = triangle->Edges[0]->SecondVertex->Position;

					if (triangle->Edges[1]->FirstVertex->Position != v1 &&
						triangle->Edges[1]->FirstVertex->Position != v2)
					{
						v3 = triangle->Edges[1]->FirstVertex->Position;
					}
					else if (triangle->Edges[1]->SecondVertex->Position != v1 &&
						triangle->Edges[1]->SecondVertex->Position != v2)
					{
						v3 = triangle->Edges[1]->SecondVertex->Position;
					}

					CU::Plane<float> plane(v1, v2, v3);

					float d = plane.myNormal.Dot(plane.myPoint);

					if (plane.myNormal.Dot(direction) == 0)
					{
						continue; // No intersection, the line is parallel to the plane
					}

					// Compute the X value for the directed line ray intersecting the plane
					float x = (d - plane.myNormal.Dot(origin)) / plane.myNormal.Dot(direction);

					// output contact point
					intersectionPoint = origin + direction.GetNormalized() * x; //Make sure your ray vector is normalized
					aIntersectingPoint = intersectionPoint;
					aIntersectingTriangle = triangle;
					return true;
				}
			}
		}
	}

	return false;
}

void CNavmesh::CreateNode(CU::GrowingArray<SNavmeshNode>& aNodeList, SNavmeshTriangle & aTriangle, const SNavmeshNode & aEndNode, std::unordered_map<SNavmeshTriangle*, SNavmeshNode*>& triangleNodes)
{
	SNavmeshNode node;
	node.myPosition = aTriangle.CenterPosition;
	node.myH = CalculateHeuristics(node.myPosition, aEndNode.myPosition);
	node.myG = 0.0f;
	node.myTriangle = &aTriangle;

	aNodeList.Add(node);
	triangleNodes[&aTriangle] = &aNodeList.GetLast();
}

void CNavmesh::CreateModel()
{
	myModel = new CModel(); 
	myPositionsModel = new CModel();

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

	myPositionsModelVertices.Init(myTriangles.Size());
	for (int i = 0; i < myTriangles.Size(); ++i)
	{
		SVertexDataCube vertex;
		vertex.position = myTriangles[i].CenterPosition;
		vertex.position.w = 1.0f;

		myPositionsModelVertices.Add(vertex);
	}

	myPositionsModel->Initialize(new CEffect(vertexShader, pixelShader, nullptr, inputLayout, D3D10_PRIMITIVE_TOPOLOGY_POINTLIST), nullptr, myPositionsModelVertices, myIndices);
	myModel->Initialize(effect, nullptr, myModelVertices, myIndices);
}

void CNavmesh::BuildNodeNetwork(CU::GrowingArray<SNavmeshNode>& aNodeList, std::unordered_map<SNavmeshTriangle*, SNavmeshNode*>& triangleNodes)
{
	for (int i = 0; i < aNodeList.Size(); ++i)
	{
		SNavmeshTriangle* nodeTriangle = aNodeList[i].myTriangle;
		CU::VectorOnStack < SNavmeshTriangle*, 3> neighbours;
		
		for (int j = 0; j < 3; ++j)
		{
			if (nodeTriangle->Edges[j]->Triangles[0] != nodeTriangle)
			{
				neighbours.Add(nodeTriangle->Edges[j]->Triangles[0]);
			}
			else if (nodeTriangle->Edges[j]->Triangles[1] != nullptr)
			{
				neighbours.Add(nodeTriangle->Edges[j]->Triangles[1]);
			}
		}

		for (int j = 0; j < neighbours.Size(); ++j)
		{
			aNodeList[i].myNeighbours.Add(triangleNodes[neighbours[j]]);
		}
	}
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

	//CU::Vector3f v1 = myPoints[aFace.myVerteces[0].VertexIndex].Position;
	//CU::Vector3f v2 = myPoints[aFace.myVerteces[1].VertexIndex].Position;
	//CU::Vector3f v3 = myPoints[aFace.myVerteces[2].VertexIndex].Position;

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

	tris.CenterPosition = first.FirstVertex->Position + second.FirstVertex->Position + third.FirstVertex->Position; // Sets the centre of the triangle with median
	tris.CenterPosition /= 3.0f;

	int index = myTriangles.Size();
	myTriangles.Add(tris);
	myTriangles.GetLast().Index = index;

	SNavmeshTriangle* trisp = &myTriangles.GetLast();
	myEdges[edgeIndices[0]].AddTriangle(trisp);
	myEdges[edgeIndices[1]].AddTriangle(trisp);
	myEdges[edgeIndices[2]].AddTriangle(trisp);


}

float CNavmesh::CalculateHeuristics(const CU::Vector3f & aFrom, const CU::Vector3f & aTo)
{
	return (aFrom - aTo).Length();
}
