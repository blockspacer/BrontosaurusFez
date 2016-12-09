#include "stdafx.h"
#include "Frustum.h"
#include "vector3.h"
#include "matrix33.h"
#include "AABB.h"

namespace
{
	float DistancePoints2(const CU::Vector3f aFirstVector, const CU::Vector3f aSecondVector)
	{
		return ((aFirstVector.x - aSecondVector.x) * (aFirstVector.x - aSecondVector.x)) + 
			((aFirstVector.y - aSecondVector.y) * (aFirstVector.y - aSecondVector.y)) + 
			((aFirstVector.z - aSecondVector.z) * (aFirstVector.z - aSecondVector.z));
	}
}




namespace CU
{

	CFrustum::CFrustum()
	{

	}


	CFrustum::~CFrustum()
	{
	}
	
	void CFrustum::SetFrustum(float aFar, float aNear, float aFOV, float aWidth, float aHeight)
	{
		if (myPlanes.Count() > 0)
		{
			myPlanes.RemovePlanes();
		}


		//float AspectRatioX = aWidth / aHeight;
		float AspectRatioY = aHeight / aWidth;
		float FovX = (aFOV + 15) * (static_cast<float>(M_PI) / 180.0f);
		float TanFovX = std::tan(FovX / 2.0f);
		float FovY = 2.0f * std::atan(TanFovX * AspectRatioY);


		Plane<float> farPlane = Plane<float>(CU::Point3f(0.f, 0.f, aFar), CU::Vector3f(0.f, 0.f, 1.0f));
		Plane<float> nearPlane = CU::Plane<float>(CU::Point3f(0.f, 0.f, aNear), Vector3f(0.f, 0.f, -1.0f));


		Plane<float> leftPlane = Plane<float>(Point3f(0.f, 0.f, 0.f),
			Point3f(1.f, 0.f, 0.f) * Matrix33f::CreateRotateAroundY(FovX));

		Plane<float> rightPlane = Plane<float>(Point3f(0.f, 0.f, 0.f),
			Point3f(-1.f, 0.f, 0.f) * Matrix33f::CreateRotateAroundY(-FovX));

		Plane<float> topPlane = Plane<float>(Point3f(0.f, 0.f, 0.f),
			Point3f(0.f, 1.f, 0.f) * Matrix33f::CreateRotateAroundX(-FovY));

		CU::Plane<float> bottomPlane = Plane<float>(Point3f(0.f, 0.f, 0.f),
			Point3f(0.f, -1.f, 0.f) * Matrix33f::CreateRotateAroundX(FovY));


		myPlanes.AddPlane(farPlane);
		myPlanes.AddPlane(nearPlane);
		myPlanes.AddPlane(leftPlane);
		myPlanes.AddPlane(rightPlane);
		myPlanes.AddPlane(topPlane);
		myPlanes.AddPlane(bottomPlane);


	}
	bool CFrustum::IsInside(const CU::AABB & aAABB)
	{
	//	// assumes the AABB is in the same space as the Frustum
		bool intersects =
			myPlanes.Inside(aAABB.myCenterPos, aAABB.myRadius);// |
			//myPlanes.Inside({ aAABB.myMinPos.x, aAABB.myMinPos.y, aAABB.myMinPos.z }) |
			//myPlanes.Inside({ aAABB.myMaxPos.x, aAABB.myMinPos.y, aAABB.myMinPos.z }) |
			//myPlanes.Inside({ aAABB.myMinPos.x, aAABB.myMaxPos.y, aAABB.myMinPos.z }) |
			//myPlanes.Inside({ aAABB.myMinPos.x, aAABB.myMinPos.y, aAABB.myMaxPos.z }) |
			//myPlanes.Inside({ aAABB.myMaxPos.x, aAABB.myMaxPos.y, aAABB.myMaxPos.z }) |
			//myPlanes.Inside({ aAABB.myMinPos.x, aAABB.myMaxPos.y, aAABB.myMaxPos.z }) |
			//myPlanes.Inside({ aAABB.myMaxPos.x, aAABB.myMinPos.y, aAABB.myMaxPos.z }) |
			//myPlanes.Inside({ aAABB.myMaxPos.x, aAABB.myMaxPos.y, aAABB.myMinPos.z });


		return intersects;
	}
}