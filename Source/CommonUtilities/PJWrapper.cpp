#include "stdafx.h"
#include "PJWrapper.h"
#include <fstream>

CU::CPJWrapper::CPJWrapper()
{
}


CU::CPJWrapper::~CPJWrapper()
{
}

std::string CU::CPJWrapper::Parse(const std::string& afilePath)
{
	std::ifstream file(afilePath);

	std::string error = picojson::parse(myValue, file);
	file.close();

	return error;
}

CU::eJsonType CU::CPJWrapper::GetType()
{
	/*if (IsNull())
	{
		return eJsonType::JSON_NULL;
	}*/

	if (IsBool())
	{
		return eJsonType::BOOL;
	}

	if (IsNumber())
	{
		return eJsonType::NUMBER;
	}

	if (IsArray())
	{
		return  eJsonType::ARRAY;
	}

	if (IsObject())
	{
		return eJsonType::OBJECT;
	}

	if (IsString())
	{
		return eJsonType::STRING;
	}

	return eJsonType::JSON_NULL;
}

CU::eJsonType CU::CPJWrapper::GetType() const
{
	/*if (IsNull())
	{
	return eJsonType::JSON_NULL;
	}*/

	if (IsBool())
	{
		return eJsonType::BOOL;
	}

	if (IsNumber())
	{
		return eJsonType::NUMBER;
	}

	if (IsString())
	{
		return eJsonType::STRING;
	}

	if (IsArray())
	{
		return  eJsonType::ARRAY;
	}

	if (IsObject())
	{
		return eJsonType::OBJECT;
	}

	return eJsonType::JSON_NULL;
}

bool CU::CPJWrapper::IsNull() const
{
	return myValue.is<picojson::null>();
}

bool CU::CPJWrapper::IsBool() const
{
	return myValue.is<bool>();
}

bool CU::CPJWrapper::GetBool()
{
	if (IsBool() == false)
	{
		DL_ASSERT("Trying to get a bool from a json value that does not contain a bool");
	}

	return myValue.get<bool>();
}

bool CU::CPJWrapper::IsNumber() const
{
	return myValue.is<double>();
}

double CU::CPJWrapper::GetNumber()
{
	if (IsNumber() == false)
	{
		DL_ASSERT("Trying to get a Number from a json value that does not contain a Number");
	}

	return myValue.get<double>();
}

double CU::CPJWrapper::GetNumber() const
{
	if (IsNumber() == false)
	{
		DL_ASSERT("Trying to get a Number from a json value that does not contain a Number");
	}

	return myValue.get<double>();
}

float CU::CPJWrapper::GetFloat() const
{
	return static_cast<float>(GetNumber());
}

int CU::CPJWrapper::GetInt() const
{
	return static_cast<int>(GetNumber());
}

unsigned int CU::CPJWrapper::GetUInt() const
{
	return static_cast<unsigned int>(GetNumber());
}

bool CU::CPJWrapper::IsString() const
{
	return myValue.is<std::string>();
}

std::string& CU::CPJWrapper::GetString()
{
	if (IsString() == false)
	{
		DL_ASSERT("Trying to get a string from a json value that does not contain a string");
	}

	return  myValue.get<std::string>();
}

const std::string& CU::CPJWrapper::GetString() const
{
	if (IsString() == false)
	{
		DL_ASSERT("Trying to get a string from a json value that does not contain a string");
	}

	return  myValue.get<std::string>();
}

bool CU::CPJWrapper::IsArray() const
{
	return myValue.is<picojson::array>();
}

CU::CPJWrapper CU::CPJWrapper::operator[](const int anIndex)
{
	if (IsArray() == false)
	{
		DL_ASSERT("pico json value is not an array");
	}

	const picojson::array& tempArray = myValue.get<picojson::array>();

	if (anIndex >= tempArray.size() || anIndex < 0)
	{
		DL_ASSERT("Out of range in json array");
	}

	return tempArray[anIndex];
}

CU::CPJWrapper CU::CPJWrapper::operator[](const int anIndex) const
{
	if (IsArray() == false)
	{
		DL_ASSERT("pico json value is not an array");
	}

	const picojson::array& tempArray = myValue.get<picojson::array>();

	if (anIndex >= tempArray.size() || anIndex < 0)
	{
		DL_ASSERT("Out of range in json array");
	}

	return tempArray[anIndex];
}

size_t CU::CPJWrapper::Size()
{
	if (IsArray() == false && IsObject() == false && IsString() == false)
	{
		DL_ASSERT("trying to get size from a json value that is neither an Object, an Array or a String");
	}

	switch (GetType())
	{
	case eJsonType::ARRAY: 
		return myValue.get<picojson::array>().size();
	case eJsonType::OBJECT:
		return myValue.get<picojson::object>().size();
	case eJsonType::STRING:
		return GetString().size();
	case eJsonType::JSON_NULL:
	case eJsonType::BOOL: 
	case eJsonType::NUMBER: 
	default: 
		DL_ASSERT("Something Is wrong here this should be unreachable");
		break;
	}
	return 0;
}

size_t CU::CPJWrapper::Size() const
{
	if (IsArray() == false && IsObject() == false && IsString() == false)
	{
		DL_ASSERT("trying to get size from a json value that is neither an Object, an Array or a String");
	}

	switch (GetType())
	{
	case eJsonType::ARRAY:
		return myValue.get<picojson::array>().size();
	case eJsonType::OBJECT:
		return myValue.get<picojson::object>().size();
	case eJsonType::STRING:
		return GetString().size();
	case eJsonType::JSON_NULL:
	case eJsonType::BOOL:
	case eJsonType::NUMBER:
	default:
		DL_ASSERT("Something Is wrong here this should be unreachable");
		break;
	}
	return 0;
}

bool CU::CPJWrapper::IsObject() const
{
	return myValue.is<picojson::object>();
}

CU::JsonObject CU::CPJWrapper::GetJsonObject()
{
	if (IsObject() == false)
	{
		DL_ASSERT("trying to get jason value as an Object when it's not an Object");
	}

	const picojson::object& object = myValue.get<picojson::object>();

	JsonObject resultObject;
	for (auto pair : object)
	{
		resultObject[pair.first] = CPJWrapper(pair.second);
	}

	return resultObject;
}

CU::JsonObject CU::CPJWrapper::GetJsonObject() const
{
	if (IsObject() == false)
	{
		DL_ASSERT("trying to get jason value as an Object when it's not an Object");
	}

	const picojson::object& object = myValue.get<picojson::object>();

	JsonObject resultObject;
	for (auto pair : object)
	{
		resultObject[pair.first] = CPJWrapper(pair.second);
	}

	return resultObject;
}

CU::CPJWrapper::CPJWrapper(const picojson::value& aValue)
{
	myValue = aValue;
}
