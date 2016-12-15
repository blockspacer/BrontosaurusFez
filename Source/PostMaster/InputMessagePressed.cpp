#include "stdafx.h"
#include "InputMessagePressed.h"
#include "../Components/InputController.h"

InputMessagePressed::InputMessagePressed(const CU::eInputMessage& aInputMessage)
	: myInputMessage(aInputMessage)
{
}


InputMessagePressed::~InputMessagePressed()
{
}

eMessageReturn InputMessagePressed::DoEvent(InputController *aInputController) const
{
	if (aInputController != nullptr)
	{
		aInputController->TakeInputMessage(myInputMessage);
	}

	return eMessageReturn::eContinue;
}
