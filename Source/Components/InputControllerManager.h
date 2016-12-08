#pragma once
#include "../CommonUtilities/GrowingArray.h"
class InputController;
class InputControllerManager
{
public:
	static InputControllerManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	void RegisterComponent(InputController* aComponent);
	void Update(const CU::Time& aDeltaTime);
	~InputControllerManager();

private:
	InputControllerManager();

	CU::GrowingArray<InputController*> myComponents;
	
	static InputControllerManager* ourInstance;
};

