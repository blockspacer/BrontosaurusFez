#include "stdafx.h"
#include "SecretlySetMousePos.h"
#include "GUI\GUICursor.h"
#include <CommonUtilities\Camera.h>

const CU::Camera* CSecretlySetMousePos::ourPlayerCamera = nullptr;

CSecretlySetMousePos::CSecretlySetMousePos(const CU::Vector3f& aMouse3DPos)
	: myMousePos3D(aMouse3DPos)
{
}

CSecretlySetMousePos::~CSecretlySetMousePos()
{
}

eMessageReturn CSecretlySetMousePos::DoEvent(GUICursor* aGUICursor) const
{
	if (aGUICursor && ourPlayerCamera)
	{
		CU::Vector4f mouseViewPos(myMousePos3D * ourPlayerCamera->GetInverse(), 1.f);
		CU::Vector4f mouseScreenPos = mouseViewPos * ourPlayerCamera->GetProjection();
		mouseScreenPos /= mouseScreenPos.w;
		CU::Vector2f projectedPosition(mouseScreenPos.x, mouseScreenPos.y);
		projectedPosition.y = -projectedPosition.y;
		projectedPosition += projectedPosition.One;
		projectedPosition *= 0.5f;

		aGUICursor->SetPosition(projectedPosition);
	}

	return eMessageReturn::eContinue;
}

void CSecretlySetMousePos::SetCamera(const CU::Camera& aCamera)
{
	ourPlayerCamera = &aCamera;
}
