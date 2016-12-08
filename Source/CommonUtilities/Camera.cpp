#include "stdafx.h"
#include "Camera.h"
#include "AABB.h"


namespace CU
{
	Camera::Camera()
	{
	}

	Camera::Camera(
		const float aFov,
		const float aProjectionWidth,
		const float aProjectionHeight,
		const float aNear,
		const float aFar,
		const Vector3f &aPosition)
	{
		Init(aFov, aProjectionWidth, aProjectionHeight, aNear, aFar, aPosition);
	}

	Camera::~Camera()
	{
	}

	void Camera::Init(
		const float aFov,
		const float aProjectionWidth,
		const float aProjectionHeight,
		const float aNear,
		const float aFar,
		const Vector3f &aPosition)
	{

		myProjection = myProjection.CreateProjectionMatrixLH(1, aFar, aProjectionWidth , aProjectionHeight , (aFov) *(3.14159265f / 180.f));
		myFrustum.SetFrustum(aFar, aNear, aFov, aProjectionWidth, aProjectionHeight);
		myTransformation = Matrix44f::Identity;
		myTransformation.SetPosition(aPosition);
		aNear;
		mySpeed = 0.1f;
		myDistFarNear = aFar - aNear;
		myFar = aFar;
		myNear = aNear;


	}

	void Camera::ReInit(
		const float aFov,
		const float aProjectionWidth,
		const float aProjectionHeight,
		const float aNear,
		const float aFar)
	{
		myProjection = myProjection.CreateProjectionMatrixLH(aNear, aFar, aProjectionWidth , aProjectionHeight, aFov * (3.14159265f / 180.f));
		myFrustum.SetFrustum(aFar, aNear, aFov, aProjectionWidth, aProjectionHeight);
		myDistFarNear = aFar - aNear;
		myFar = aFar;
		myNear = aNear;
	}

	Matrix44f Camera::GetInverse() const
	{
		Matrix44f tempMatrix = myTransformation;
		Vector4f translation(tempMatrix.GetPosition());
		tempMatrix.SetPosition(Vector3f(0.0f, 0.0f, 0.0f));
		translation *= -1;
		translation.w = 1;
		tempMatrix.Transpose();
		translation = translation * tempMatrix;
		tempMatrix.SetPosition(Vector3f(translation.x, translation.y, translation.z));
		return tempMatrix;
	}

	const Matrix44f& Camera::GetProjection() const
	{
		return myProjection;
	}

	const Vector3f Camera::GetPosition() const
	{
		return myTransformation.GetPosition();
	}

	void Camera::Translate(Vector3f aVector)
	{
		Matrix44f translationMatrix;
		translationMatrix.SetPosition(aVector);

		myTransformation = translationMatrix * myTransformation;
	}


	void Camera::Roll(const float aAngle)
	{
		myTransformation = myTransformation.CreateRotateAroundZ(aAngle) * myTransformation;
	}
	void Camera::Pitch(const float aAngle)
	{
		myTransformation = myTransformation.CreateRotateAroundX(aAngle) * myTransformation;

	}
	void Camera::Jaw(const float aAngle)
	{
		myTransformation = myTransformation.CreateRotateAroundY(aAngle) * myTransformation;
	}

	void Camera::TranslateForward(const float aAmount)
	{
		Vector3f position = myTransformation.GetPosition();
		Vector3f forwardVector(myTransformation.m31, myTransformation.m32, myTransformation.m33);
		position += forwardVector * aAmount;

		myTransformation.SetPosition(position);
	}

	void Camera::TranslateSideways(const float aAmount)
	{
		Vector3f position = myTransformation.GetPosition();
		Vector3f forwardVector(myTransformation.m11, myTransformation.m12, myTransformation.m13);
		position += forwardVector * aAmount;

		myTransformation.SetPosition(position);
	}
	void Camera::SetTransformation(const Matrix44f & aTransformation)
	{
		myTransformation = aTransformation;
	}
	const Matrix44f& Camera::GetTransformation() const
	{
		return myTransformation;
	}


	bool Camera::IsInside(const CU::AABB& aAABB)
	{
		CU::Matrix44f inverse = GetInverse();

		CU::AABB box = aAABB; 
		box.myCenterPos = aAABB.myCenterPos * inverse;
		box.myMinPos = aAABB.myMinPos * inverse;
		box.myMaxPos = aAABB.myMaxPos * inverse;

		return myFrustum.IsInside(box);


	}
}