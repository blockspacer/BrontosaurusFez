#pragma once
#include "Vector3.h"
#include "matrix44.h"
#include "Frustum.h"

namespace CU
{
	class Camera
	{
	public:
		Camera();
		Camera(const float aFov, const float aProjectionWidth, const float aProjectionHeight, const float aNear, const float aFar, const Vector3f & aPosition);
		// Initializes the camera with screen height, screen width and it's world position 
		// In the case of TGA2D both aProjectionWidth and aProjectionHeight should probably be 1. 
		// maybe it's the same in BSR? we'll see about that
		
		~Camera();

		//Init yo
		void Init(const float aFov, const float aProjectionWidth, const float aProjectionHeight, const float aNear, const float aFar, const Vector3f & aPosition);
		void ReInit(const float aFov, const float aProjectionWidth, const float aProjectionHeight, const float aNear, const float aFar);

		

		// Returns the inverse of the camera orientation 
		Matrix44f GetInverse() const;

		// Returns the projection matrix 
		const Matrix44f& GetProjection() const;

		const Vector3f GetPosition() const;

		void Translate(Vector3f aVector);
		void Roll(const float aAngle);
		void Pitch(const float aAngle);
		void Jaw(const float aAngle);

		void TranslateForward(const float aAmount);
		void TranslateSideways(const float aAmount);

		void SetTransformation(const Matrix44f & aTransformation);
		const Matrix44f& GetTransformation() const;
		inline float GetDistFarNear();
		inline float GetFar();
		inline float GetNear();
		inline const CU::CFrustum& GetFrustum();
		
		bool IsInside(const CU::AABB& aAABB);

	private:
		// Describes the orientation of the camera in regard to world space (or 
		// rather its parent space, but it's probably world) 
		Matrix44f myTransformation;

		// Describes the transformation from camera space to screen space 
		Matrix44f myProjection;


		//supercool frustum that will be glorious and amazing
		CFrustum myFrustum;

		float mySpeed;

		float myDistFarNear;
		float myNear;
		float myFar;
	};

	inline const CU::CFrustum & CU::Camera::GetFrustum()
	{
		return myFrustum;
	}


	float Camera::GetDistFarNear()
	{
		return myDistFarNear;
	}

	float Camera::GetFar()
	{
		return myFar;
	}

	float Camera::GetNear()
	{
		return myNear;
	}

} 