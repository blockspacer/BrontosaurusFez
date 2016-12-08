#pragma once

#include "../CommonUtilities/GrowingArray.h"
#include "../PostMaster/Subscriber.h"

namespace CU
{
	class InputWrapper;
	class XInputWrapper;

	template <typename T>
	class Vector2;
	using Vector2f = Vector2<float>;

	enum class eKeys;
}


class CInputManager : public Subscriber
{
public:
	CInputManager();
	~CInputManager();

	void Update();
	eMessageReturn Recieve(const Message & aMessage) override;
	void SetMousePosition(const CU::Vector2f& aMousePosition);
	void LockUnlockMouse(const bool aHasFocus);
private:
	void UpdateMouse();
	void UpdateKeyboard();
	void UpdateGamePad();

	CU::InputWrapper* myDInputWrapper;
	CU::XInputWrapper* myXInputWrapper;
	CU::GrowingArray<CU::eKeys> myKeyDowns;

	bool myHasFocus;
};
