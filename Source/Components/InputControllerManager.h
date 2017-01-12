#pragma once

namespace CU
{
	class Time;
}
class CScene;
class InputController;
class InputControllerManager
{
public:
	static InputControllerManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	CU::GrowingArray<InputController*>& GetComponents();

	void SetScene(CScene* aScene);
	InputController* CreateAndRegisterComponent();
	void Update(const CU::Time& aDeltaTime);
	~InputControllerManager();

private:
	InputControllerManager();

	CU::GrowingArray<InputController*> myComponents;
	CScene* myScene;

	static InputControllerManager* ourInstance;
};

