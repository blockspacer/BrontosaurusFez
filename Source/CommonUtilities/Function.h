#pragma once

template<typename ReturnType, typename... Arguments>
class CFunction
{
public:
	inline CFunction();
	inline CFunction(const CFunction& aCopy);
	inline ~CFunction();

	inline ReturnType operator()(Arguments aArguments);

	inline CFunction& operator=(const CFunction& aCopy);

	inline bool operator==(const CFunction& aOther) const;
	inline bool operator==(const std::nullptr_t aPointer) const;

private:
	ReturnType (*myFunction)(Arguments);
};

template<typename ReturnType, typename ...Arguments>
inline CFunction<ReturnType, ...Arguments>::CFunction()
{
	myFunction = nullptr;
}

template<typename ReturnType, typename ...Arguments>
inline CFunction<ReturnType, ...Arguments>::~CFunction()
{
}

template<typename ReturnType, typename ...Arguments>
inline ReturnType CFunction<ReturnType, ...Arguments>::operator()(Arguments aArguments)
{
	return myFunction(aArguments);
}

template<typename ReturnType, typename ...Arguments>
inline CFunction & CFunction<ReturnType, ...Arguments>::operator=(const CFunction & aCopy)
{
	myFunction = aCopy.myFunction;
}

template<typename ReturnType, typename ...Arguments>
inline bool CFunction<ReturnType, ...Arguments>::operator==(const CFunction& aOther) const
{
	return myFunction == aOther.myFunction;
}

template<typename ReturnType, typename ...Arguments>
inline bool CFunction<ReturnType, ...Arguments>::operator==(const std::nullptr_t aPointer) const
{
	return myFunction == aPointer;
}
