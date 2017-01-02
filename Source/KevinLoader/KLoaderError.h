#pragma once

namespace KLoader
{
	enum class eError
	{
		NO_LOADER_ERROR = 0,
		JSON_ERROR,
		MISSING_OBJECTS,
		MISSING_OBJECT_LOAD_FUNC,
		MISSING_COMPONENTS,
	};
}