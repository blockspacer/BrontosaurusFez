#pragma once
#include <GrowingArray.h>
#include "ObjLoader.h"
#include "Model.h"
#include "VectorOnStack.h"
#include <unordered_map>


class CPath
{
	friend class CNavmesh;
public:
	struct SWaypoint
	{
		CU::Vector3f myPosition;
		CU::Vector3f myLeftPoint;
		CU::Vector3f myRightPoint;
	};
	CPath() {}
	CPath(const CU::GrowingArray<SWaypoint, unsigned int, false>& aWaypointList)
	{
		myWaypoints = aWaypointList;
	}
	void Smooth();

	CU::GrowingArray<SWaypoint, unsigned int, false> myWaypoints;
private:
};




class CNavmesh
{
public:

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
		SNavmeshPoint* Points[3]; // mebe good?

		CU::Vector3f CenterPosition;

		int Index = -1;
	};
	struct SNavmeshNode
	{
		SNavmeshNode()
		{
			myTriangle = nullptr;
			myParent = nullptr;
		}

		SNavmeshNode* myParent;
		CU::VectorOnStack<SNavmeshNode*, 5, unsigned short, false> myNeighbours;
		
		CU::Vector3f myPosition;

		SNavmeshTriangle* myTriangle;

		float myG = 0.0f;
		float myH = 0.0f;
	};
	class NavmeshLesser
	{
	public:
		bool operator()(const SNavmeshNode* aLeft, const SNavmeshNode* aRight)
		{
			return (aLeft->myG + aLeft->myH) < (aRight->myG + aRight->myH);
			//return (aLeft.myG + aLeft.myH) < (aRight.myG + aRight.myH);
		}
	};

public:
	CNavmesh();
	~CNavmesh();
	void Render();
	void LoadFromFile(const char* aFilePath);
	CPath CalculatePath(const SNavmeshNode & aStartPoint, const SNavmeshNode & aEndPoint);

	SNavmeshTriangle& GetClosestTriangle(const CU::Vector3f & aPosition);

	bool GetPointOnNavmesh(const CU::Vector3f & aOrigin, const CU::Vector3f & aDirection, CNavmesh::SNavmeshTriangle & aIntersectingTriangle, CU::Vector3f & aIntersectionPoint);

	bool IsValid(const CU::Vector3f & aPosition, SNavmeshTriangle *& aIntersectingTriangle, CU::Vector3f & aIntersectingPoint);
	bool IsValid(const CU::Vector2f & aPosition, SNavmeshTriangle *& aIntersectingTriangle, CU::Vector3f & aIntersectingPoint);



	static float TriArea2(const CU::Vector2f& a, const CU::Vector2f& b, const CU::Vector2f& c);
	static bool VEqual(const CU::Vector2f& a, const CU::Vector2f& b);

private:
	void CreateNode(CU::GrowingArray<SNavmeshNode>& aNodeList, SNavmeshTriangle & aTriangle, const SNavmeshNode & aEndNode, std::unordered_map<SNavmeshTriangle*, SNavmeshNode*>& triangleNodes);
	void CreateModel();
	void BuildNodeNetwork(CU::GrowingArray<SNavmeshNode>& aNodeList, std::unordered_map<SNavmeshTriangle*, SNavmeshNode*>& triangleNodes);
	void BuildNavmeshFromOBJ(const ObjLoader::SObjLoaderModel& aModel);
	void BuildEdgesFromFace(const ObjLoader::SObjLoaderFace& aFace);
	float CalculateHeuristics(const CU::Vector3f& aFrom, const CU::Vector3f& aTo);


private:
	CU::Matrix44f myTransformation;

	CU::GrowingArray<SNavmeshEdge> myEdges;
	CU::GrowingArray<SNavmeshPoint> myPoints;
	CU::GrowingArray<SNavmeshTriangle> myTriangles;

	CU::GrowingArray<SNavmeshNode> myNodes;


	CU::GrowingArray<unsigned int> myIndices;
	CU::GrowingArray<SVertexDataCube> myModelVertices;
	CU::GrowingArray<SVertexDataCube> myPositionsModelVertices;

//	IFDEFA BORT
	CModel* myModel;
	CModel* myPositionsModel;

	bool myShouldRender;

};

