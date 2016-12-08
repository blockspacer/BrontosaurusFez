#pragma once
#include "../CommonUtilities/vector2.h"
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/vector4.h"
//#include <vector2.h>
//#include <vector4.h>
//#include <matrix44.h>

//Legacy ?
//Size of a vertex can be found in the FBX loader model, along with all data and number of vertices. 
struct SVertexDataCube
{
	CU::Vector4f position;
};

struct SCameraViewProjection
{
	CU::Matrix44f myCameraSpaceInverse;
	CU::Matrix44f myProjectionSpace;
}; 

struct SToWorldSpace
{
	CU::Matrix44f myWorldSpace;
	CU::Matrix44f myWorldSpaceLastFrame;
};


struct SLODDistances
{
	SLODDistances() { LOD_DistStart2 = LOD_DistEnd2 = 0.f; }
	float LOD_DistStart2;
	float LOD_DistEnd2;
};


struct SOncePerFrameBuffer
{
	SCameraViewProjection myCameraMatrices;

	float deltaTime;
	float time;

	float fogStart;
	float fogEnd;
};