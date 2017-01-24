#include "stdafx.h"
#include "IController.h"
#include "AIControllerComponent.h"

IController::IController()
{
	myWeight = 1.0f;
	myControllerType = eControllerType::eDefault;
	myType = eComponentType::eControllerInterface;
}

IController::~IController()
{
}

void IController::AddParent(CAIControllerComponent * aController)
{
	myController = aController;
}

const eControllerType IController::GetType() const
{
	return myControllerType;
}

CAIControllerComponent * IController::GetControllerComponent()
{
	return myController;
}

const CU::Vector2f IController::Update(const CU::Time& aDeltaTime)
{
	return CU::Vector2f();
}
