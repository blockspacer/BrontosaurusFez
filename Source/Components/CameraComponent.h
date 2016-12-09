#pragma once
#include "Component.h"
//#include <matrix44.h>

namespace CU
{
	template <typename TYPE>
	class Matrix44;
	using Matrix44f = Matrix44<float>;
}

class CCameraComponent : public CComponent
{
public:
	friend class CCameraManager;
	CU::Matrix44f GetToWorldTransformation();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)override;

	void Destroy();

private:
	CCameraComponent();
	~CCameraComponent();
private:

};

