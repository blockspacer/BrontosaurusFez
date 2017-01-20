#pragma once
#include "Component.h"

class CEnemyObjectiveComponent :public CComponent
{
public:
	CEnemyObjectiveComponent(const std::string& aQuestName);
	~CEnemyObjectiveComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
private:
	std::string myQuestName;
};

