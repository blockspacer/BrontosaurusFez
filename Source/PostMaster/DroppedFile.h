#pragma once
#include "Event.h"

class DroppedFile :	public Event
{
public:
	DroppedFile(const std::string& aFilePath);
	~DroppedFile();

private:
	std::string myFilePath;
};
