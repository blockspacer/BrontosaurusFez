#pragma once
#include "Component.h"

class CActivationObjectiveComponent :public CComponent
{
public:
	CActivationObjectiveComponent(const std::string anObjectiveName);
	~CActivationObjectiveComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
private:
	std::string myObjectiveName;
};

