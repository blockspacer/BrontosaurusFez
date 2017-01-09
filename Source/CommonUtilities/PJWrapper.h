#pragma once
#include "picojson.h"

namespace CU
{
	class CPJWrapper;
	typedef std::map<std::string, CPJWrapper> JsonObject;

	enum class eJsonType
	{
		JSON_NULL,
		BOOL,
		NUMBER,
		STRING,
		ARRAY,
		OBJECT,

	};

	class CPJWrapper
	{
	public:
		CPJWrapper();
		CPJWrapper(picojson::value aValue);
		~CPJWrapper();

		std::string Parse(std::string afilePath);

		eJsonType GetType();
		CU::eJsonType GetType() const;
		bool IsNull() const;

		bool IsBool() const;
		bool GetBool();

		bool IsNumber() const;
		double GetNumber();
		double GetNumber() const;
		bool IsString() const;
		std::string GetString();
		std::string GetString() const;
		bool IsArray() const;
		CPJWrapper operator[](const int anIndex);
		CPJWrapper operator[](const int anIndex) const;
		size_t Size();
		size_t Size() const;
		bool IsObject() const;
		JsonObject GetJsonObject();
		JsonObject GetJsonObject()const;

	private:
		picojson::value myValue;
	};

}