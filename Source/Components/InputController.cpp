#include "stdafx.h"
#include "InputController.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Event.h"
#include "../CommonUtilities/EInputMessage.h"
#include "../CommonUtilities/EMouseButtons.h"
#include "GameObject.h"
#include "../BrontosaurusEngine/Engine.h"
#include "CameraManager.h"
#include "../CommonUtilities/Camera.h"

#define to_str(apa) #apa
#define combine(apa, apa2) to_str(apa) ## to_str(apa2)

InputController::InputController(const CU::Camera& aPlayerCamera)
	: myPlayerCamera(aPlayerCamera)
{
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eMouseDownMessage);
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eInputMessagePressed);
	mySkillInputMessageActivators.Init(5);
}


InputController::~InputController()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseDownMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eInputMessagePressed);
}

void InputController::Update(float aDeltaTime)
{
}

eMessageReturn InputController::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

void InputController::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
}

void InputController::Destroy()
{
	//float pointX, pointY;
	//D3DXMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	//D3DXVECTOR3 direction, origin, rayOrigin, rayDirection;
	//bool intersect, result;


	//// Move the mouse cursor coordinates into the -1 to +1 range.
	//pointX = ((2.0f * (float)mouseX) / (float)m_screenWidth) - 1.0f;
	//pointY = (((2.0f * (float)mouseY) / (float)m_screenHeight) - 1.0f) * -1.0f;

	//// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	//m_D3D->GetProjectionMatrix(projectionMatrix);
	//pointX = pointX / projectionMatrix._11;
	//pointY = pointY / projectionMatrix._22;

	//// Get the inverse of the view matrix.
	//m_Camera->GetViewMatrix(viewMatrix);
	//D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	//// Calculate the direction of the picking ray in view space.
	//direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	//direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	//direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	//// Get the origin of the picking ray which is the position of the camera.
	//origin = m_Camera->GetPosition();

	//// Get the world matrix and translate to the location of the sphere.
	//m_D3D->GetWorldMatrix(worldMatrix);
	//D3DXMatrixTranslation(&translateMatrix, -5.0f, 1.0f, 5.0f);
	//D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	//// Now get the inverse of the translated world matrix.
	//D3DXMatrixInverse(&inverseWorldMatrix, NULL, &worldMatrix);

	//// Now transform the ray origin and the ray direction from view space to world space.
	//D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
	//D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

	//// Normalize the ray direction.
	//D3DXVec3Normalize(&rayDirection, &rayDirection);

	//// Now perform the ray-sphere intersection test.
	//intersect = RaySphereIntersect(rayOrigin, rayDirection, 1.0f);

	//if (intersect == true)
	//{
	//	// If it does intersect then set the intersection to "yes" in the text string that is displayed to the screen.
	//	result = m_Text->SetIntersection(true, m_D3D->GetDeviceContext());
	//}
	//else
	//{
	//	// If not then set the intersection to "No".
	//	result = m_Text->SetIntersection(false, m_D3D->GetDeviceContext());
	//}

	//return;
}

eMessageReturn InputController::MouseClicked(const CU::eMouseButtons aMouseButton, const CU::Vector2f & aMousePosition)
{
	if(aMouseButton == CU::eMouseButtons::LBUTTON)
	{
		//convert pixel mouse position to world ground position
		CU::Vector2f windowSize(WINDOW_SIZE);
		CU::Vector2f mousePosZeroToOne = aMousePosition / windowSize;
		CU::Vector2f mousePosNormalizedSpace = mousePosZeroToOne * 2.f - CU::Vector2f::One;
		CU::Vector4f mousePosNormalizedHomogeneousSpace(mousePosNormalizedSpace, CU::Vector2f::Zero);
		CU::Vector4f screenToCameraSpaceRay = mousePosNormalizedHomogeneousSpace * myPlayerCamera.GetProjectionInverse();

		CU::Vector4f direction;
		direction.x = (screenToCameraSpaceRay.x * myPlayerCamera.GetTransformation().m11) + (screenToCameraSpaceRay.y * myPlayerCamera.GetTransformation().m21) + myPlayerCamera.GetTransformation().m31;
		direction.y = (screenToCameraSpaceRay.x * myPlayerCamera.GetTransformation().m12) + (screenToCameraSpaceRay.y * myPlayerCamera.GetTransformation().m22) + myPlayerCamera.GetTransformation().m32;
		direction.z = (screenToCameraSpaceRay.x * myPlayerCamera.GetTransformation().m13) + (screenToCameraSpaceRay.y * myPlayerCamera.GetTransformation().m23) + myPlayerCamera.GetTransformation().m33;
		direction.w = 0.f;

		CU::Vector4f targetPosition3D;
		const CU::Vector4f groundNormal(0.f, 1.f, 0.f, 0.f);
		float denominator = direction.Dot(groundNormal);
		if (std::fabs(denominator) > 0.0001f)
		{
			float t = (GetParent()->GetToWorldTransform().GetPosition() - myPlayerCamera.GetPosition()).Dot(groundNormal.myVector3) / denominator;
			if (std::fabs(t) > 0.0001f)
			{
				targetPosition3D = myPlayerCamera.GetPosition() + CU::Vector3f(direction * t);
			}
		}

		CU::Vector2f targetPosition(targetPosition3D.x, -targetPosition3D.z);
		
		eComponentMessageType type = eComponentMessageType::eSetNavigationTarget;
		SComponentMessageData data;
		data.myVector2f = targetPosition;
		GetParent()->NotifyComponents(type, data);
	}

	return eMessageReturn::eContinue;
}

eMessageReturn InputController::TakeInputMessage(const CU::eInputMessage aInputMessage)
{
	for(unsigned short i = 0; i < mySkillInputMessageActivators.Size(); i++)
	{
		if(mySkillInputMessageActivators[i] == aInputMessage)
		{
			eComponentMessageType type = eComponentMessageType::eUseSkill;
			SComponentMessageData data;
			data.myInt = i;
			GetParent()->NotifyComponents(type, data);
		}
	}
	return eMessageReturn::eContinue;
}
