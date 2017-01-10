#pragma once
#include "Component.h"
#include "../CommonUtilities/vector2.h"

class CAIControllerComponent;

enum class eControllerType
{
	eDefault,
	eSeek,
	eBlend,
	eArrive,
	eFlee,
	eWander,
};

class IController : public CComponent
{
public:
	IController();
	~IController();

	const eControllerType GetType() const;

	void AddParent(CAIControllerComponent* aController);

	virtual const CU::Vector2f Update(const CU::Time& aDeltaTime) = 0;
protected:
	CAIControllerComponent* GetControllerComponent();

	float myWeight;
	eControllerType myControllerType;
	CAIControllerComponent* myController;
};