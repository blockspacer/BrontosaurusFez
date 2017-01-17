#pragma once
#include <GrowingArray.h>
#include "ObjLoader.h"
#include "Model.h"


class CNavmesh
{
	struct SNavmeshTriangle;
	struct SNavmeshTriangle;

	struct SNavmeshPoint
	{
		SNavmeshPoint()
		{
			Position = CU::Vector3f::Zero;
		}

		SNavmeshPoint(const CU::Vector3f& aRight)
		{
			Position = aRight;
		}
		bool operator ==(const SNavmeshPoint& aRight)
		{
			if (Position == aRight.Position)
			{
				return true;
			}
			return false;
		}

		CU::Vector3f Position;
		int Index = -1;
	};

	struct SNavmeshEdge
	{
		SNavmeshEdge()
		{
			FirstVertex = SecondVertex = nullptr;
			Triangles[0] = nullptr;
			Triangles[1] = nullptr;
		}
		bool AddTriangle(SNavmeshTriangle * aTrianglePtr)
		{
			if (Triangles[0] == nullptr)
			{
				Triangles[0] = aTrianglePtr;
				return true;
			}
			else if (Triangles[1] == nullptr)
			{
				Triangles[1] = aTrianglePtr;
				return true;
			}
			return false;
		}
		bool operator ==(const SNavmeshEdge& aRight)
		{
			if ((FirstVertex == aRight.FirstVertex && SecondVertex == aRight.SecondVertex) ||
				(FirstVertex == aRight.SecondVertex && SecondVertex == aRight.FirstVertex))
			{
				return true;
			}
			return false;
		}

		SNavmeshPoint* FirstVertex;
		SNavmeshPoint* SecondVertex;
		SNavmeshTriangle* Triangles[2];
		int Index = -1;
	};


	struct SNavmeshTriangle
	{
		SNavmeshEdge* Edges[3];


		int Index = -1;
	};

public:
	CNavmesh();
	~CNavmesh();
	void Render();
	void LoadFromFile(const char* aFilePath);
private:

	void CreateModel();

	void BuildNavmeshFromOBJ(const ObjLoader::SObjLoaderModel& aModel);
	void BuildEdgesFromFace(const ObjLoader::SObjLoaderFace& aFace);


	CU::GrowingArray<SNavmeshEdge> myEdges;
	CU::GrowingArray<SNavmeshPoint> myPoints;
	CU::GrowingArray<SNavmeshTriangle> myTriangles;

	CU::GrowingArray<unsigned int> myIndices;
	CU::GrowingArray<SVertexDataCube> myModelVertices;
	CModel* myModel;
};

