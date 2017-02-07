#pragma once
#include "Event.h"

namespace CU
{
	template <typename T>
	class Vector3;
	using Vector3f = Vector3<float>;

	class Camera;
}

class CSecretlySetMousePos : public Event
{
public:
	CSecretlySetMousePos(const CU::Vector3f& aMouse3DPos);
	~CSecretlySetMousePos();

	eMessageReturn DoEvent(GUICursor* aGUICursor) const override;

	static void SetCamera(const CU::Camera& aCamera);

private:
	const CU::Vector3f& myMousePos3D;
	
	static const CU::Camera* ourPlayerCamera;
};

