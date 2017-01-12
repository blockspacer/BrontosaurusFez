#pragma once

namespace picojson
{
	class value;
}

namespace CU
{
	enum class eJsoneValueType
	{
		JSON_NULL,
		BOOL,
		NUMBER,
		STRING,
		ARRAY,
		OBJECT
	};

	class CJsonValue
	{
	public:
		CJsonValue();
		CJsonValue(const CJsonValue& aValue);
		CJsonValue(const picojson::value& aValue);
		CJsonValue(const std::string& aFilePath);
		~CJsonValue();

		CJsonValue& operator=(const CJsonValue& aValue);

		std::string Parse(const std::string& aFilePath);

		eJsoneValueType GetType() const;
		int Size() const;

		bool IsNull() const;
		bool IsBool() const;
		bool IsNumber() const;
		bool IsString() const;
		bool IsArray() const;
		bool IsObject() const;
		bool HasKey(const std::string& aKey) const;

		bool GetBool() const;

		double GetNumber() const;
		float GetFloat() const;
		int GetInt() const;
		unsigned int GetUInt() const;

		const std::string& GetString() const;

		CJsonValue operator[](const int aIndex) const;
		CJsonValue operator[](const std::string& aKey) const;

		CJsonValue at(const int aIndex) const;
		CJsonValue at(const std::string& aKey) const;

	private:
		CJsonValue(const picojson::value* aValuePointer);

		const picojson::value* myValue;
		bool myIsBorrowed;
	};
}
