#pragma once

template <typename ValueType>
class ValueObserver
{
public:
	virtual void ValueChanged(const ValueType aValue) = 0;
	virtual void SetMaxValue(const ValueType aValue) { aValue; }
};
