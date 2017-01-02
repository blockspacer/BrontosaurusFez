#pragma once
#include "Component.h"

class ICollider;

class CCollisionComponent : public CComponent
{
public:
	CCollisionComponent(ICollider* aCollider);
	CCollisionComponent(const CCollisionComponent& aCopy) = delete;
	~CCollisionComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;

private:
	void OnCollisionEnter(ICollider* aCollider);
	void OnCollisionUpdate(ICollider* aCollider);
	void OnCollisionExit(ICollider* aCollider);
	
	ICollider* myCollider;
};
