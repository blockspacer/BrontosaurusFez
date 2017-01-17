#include "stdafx.h"
#include "JsonValue.h"
#include "picojson.h"
#include <fstream>

#define JSON_ERROR(ERROR_MESSAGE) assert(ERROR_MESSAGE && false)

namespace CU
{
	CJsonValue::CJsonValue()
		: myValue(nullptr)
		, myIsBorrowed(false)
	{
		myValue = new picojson::value();
	}

	CJsonValue::CJsonValue(const CJsonValue& aValue)
		: myValue(nullptr)
		, myIsBorrowed(false)
	{
		if (aValue.myValue != nullptr)
		{
			myValue = new picojson::value(*aValue.myValue);
		}
	}

	CJsonValue::CJsonValue(const picojson::value& aValue)
		: myValue(nullptr)
		, myIsBorrowed(false)
	{
		myValue = new picojson::value(aValue);
	}

	CJsonValue::CJsonValue(const std::string& aFilePath)
		: myValue(nullptr)
		, myIsBorrowed(false)
	{
		Parse(aFilePath);
	}

	CJsonValue::~CJsonValue()
	{
		if (myIsBorrowed == false)
		{
			SAFE_DELETE(myValue);
		}
	}

	CJsonValue& CJsonValue::operator=(const CJsonValue& aValue)
	{
		myValue = aValue.myValue;
		myIsBorrowed = true;

		return *this;
	}

	std::string CJsonValue::Parse(const std::string& aFilePath)
	{
		if (myValue != nullptr && myIsBorrowed == false)
		{
			delete myValue;
		}

		myIsBorrowed = false;

		std::ifstream jsonFile(aFilePath);
		if (jsonFile.good() == false)
		{
			DL_ASSERT("Could not find json file %s", aFilePath.c_str());
			return "Error loading filePath " + aFilePath;
		}

		picojson::value* newValue = new picojson::value();
		std::string errorMessage = picojson::parse(*newValue, jsonFile);
		myValue = newValue;

		return errorMessage;
	}

	eJsoneValueType CJsonValue::GetType() const
	{
		if (IsObject() == true)
		{
			return eJsoneValueType::OBJECT;
		}

		if (IsArray() == true)
		{
			return eJsoneValueType::ARRAY;
		}

		if (IsString() == true)
		{
			return eJsoneValueType::STRING;
		}

		if (IsNumber() == true)
		{
			return eJsoneValueType::NUMBER;
		}

		if (IsBool() == true)
		{
			return eJsoneValueType::BOOL;
		}

		return eJsoneValueType::JSON_NULL;
	}

	int CJsonValue::Size() const
	{
		if (myValue == nullptr)
		{
			JSON_ERROR("json value is null");
			return 0;
		}

		if (IsArray() == true)
		{
			return static_cast<int>(myValue->get<picojson::array>().size());
		}

		if (IsObject() == true)
		{
			return static_cast<int>(myValue->get<picojson::array>().size());
		}

		if (IsString() == true)
		{
			return static_cast<int>(myValue->get<std::string>().size());
		}

		DL_ASSERT("trying to get size from simple json value");
		return 0;
	}

	int CJsonValue::Count(const std::string & aKey)
	{
		//maybe not work work in progress, feel free to remove or redo.
		if (myValue == nullptr)
		{
			JSON_ERROR("json value is null");
			return 0;
		}
		if (IsArray() == false)
		{
			eJsoneValueType type = GetType();
			JSON_ERROR("json value is not an array");
			return 0;
		}

		const picojson::object& tempObject = myValue->get<picojson::object>();
		auto it = tempObject.count(aKey);
		if (it <= 0)
		{
			JSON_ERROR("json object invalid key");
			return 0;
		}

		return it;
	}

	bool CJsonValue::IsNull() const
	{
		return myValue == nullptr || myValue->is<picojson::null>();
	}

	bool CJsonValue::IsBool() const
	{
		return myValue != nullptr && myValue->is<bool>();
	}
	bool CJsonValue::IsNumber() const
	{
		return myValue != nullptr && myValue->is<double>();
	}

	bool CJsonValue::IsString() const
	{
		return myValue != nullptr && myValue->is<std::string>();
	}

	bool CJsonValue::IsArray() const
	{
		return myValue != nullptr && myValue->is<picojson::array>();
	}

	bool CJsonValue::IsObject() const
	{
		return myValue != nullptr && myValue->is<picojson::object>();
	}

	bool CJsonValue::HasKey(const std::string& aKey) const
	{
		if (myValue == nullptr)
		{
			JSON_ERROR("json value is null");
			return false;
		}
		if (IsObject() == false)
		{
			JSON_ERROR("json value is not an object");
			return false;
		}

		const picojson::object& tempObject = myValue->get<picojson::object>();
		auto it = tempObject.find(aKey);

		return it != tempObject.end();
	}

	bool CJsonValue::GetBool() const
	{
		if (myValue == nullptr)
		{
			JSON_ERROR("json value is null");
			return false;
		}

		return myValue->get<bool>();
	}

	double CJsonValue::GetNumber() const
	{
		if (myValue == nullptr)
		{
			JSON_ERROR("json value is null");
			return 0.0;
		}

		return myValue->get<double>();
	}

	float CJsonValue::GetFloat() const
	{
		return static_cast<float>(GetNumber());
	}

	int CJsonValue::GetInt() const
	{
		return static_cast<int>(GetNumber());
	}

	unsigned int CJsonValue::GetUInt() const
	{
		return static_cast<unsigned int>(GetNumber());
	}

	const std::string& CJsonValue::GetString() const
	{
		if (myValue == nullptr)
		{
			JSON_ERROR("json value is null");
			const static std::string nullString("");
			return nullString;
		}

		return myValue->get<std::string>();
	}

	CJsonValue CJsonValue::operator[](const int anIndex) const
	{
		if (myValue == nullptr)
		{
			JSON_ERROR("json value is null");
			return CJsonValue();
		}

		size_t sIndex = static_cast<size_t>(anIndex);

		const picojson::array& tempArray = myValue->get<picojson::array>();
		if (sIndex < 0 || sIndex >= tempArray.size())
		{
			JSON_ERROR("json array subscript out of range");
			return CJsonValue();
		}

		return CJsonValue(&tempArray[sIndex]);
	}

	CJsonValue CJsonValue::operator[](const std::string& aKey) const
	{
		if (myValue == nullptr)
		{
			JSON_ERROR("json value is null");
			return CJsonValue();
		}
		if (IsObject() == false)
		{
			eJsoneValueType type = GetType();
			JSON_ERROR("json value is not an object");
			return CJsonValue();
		}

		const picojson::object& tempObject = myValue->get<picojson::object>();
		auto it = tempObject.find(aKey);
		if (it == tempObject.end())
		{
			JSON_ERROR("json object invalid key");
			return CJsonValue();
		}

		return CJsonValue(&it->second);
	}

#define self (*this)
	CJsonValue CJsonValue::at(const int aIndex) const
	{
		return self[aIndex];
	}

	CJsonValue CJsonValue::at(const std::string& aKey) const
	{
		return self[aKey];
	}
#undef self

	CJsonValue::CJsonValue(const picojson::value* aValuePointer)
		: myValue(aValuePointer)
		, myIsBorrowed(true)
	{
	}
}