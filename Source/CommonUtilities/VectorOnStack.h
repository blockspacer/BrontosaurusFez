#pragma once
#include <assert.h>

namespace CU
{
	template <typename Type, int aSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack& aVectorOnStack);

		~VectorOnStack();

		VectorOnStack& operator=(const VectorOnStack& aVectorOnStack);

		inline const Type& operator[](const CountType & aIndex) const;
		inline Type& operator[](const CountType & aIndex);

		inline void Add(const Type& aObject);
		inline void Insert(CountType aIndex, Type& aObject);
		inline void DeleteCyclic(Type& aObject);
		inline void DeleteCyclicAtIndex(CountType aItemNumber);
		inline void RemoveCyclic(const Type& aObject);
		inline void RemoveCyclicAtIndex(CountType aItemNumber);

		inline void Clear();
		inline void DeleteAll();

		inline Type GetLast() const;
		inline CountType Size() const;

	private:
		Type myVectorOnStack[aSize];
		CountType myCurrentSize;

	};

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::VectorOnStack()
	{
		myCurrentSize = 0;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack)
	{
		if (UseSafeModeFlag == false)
		{
			memcpy(myVectorOnStack, aVectorOnStack.myVectorOnStack, sizeof(VectorOnStack));
		}
		else
		{
			for (CountType i = 0; i < Capacity; ++i)
			{
				myVectorOnStack[i] = aVectorOnStack[i];
			}
		}
		myCurrentSize = aVectorOnStack.Size();
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::~VectorOnStack()
	{
		myCurrentSize = 0;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>& VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::operator=(const VectorOnStack& aVectorOnStack)
	{
		if (UseSafeModeFlag == false)
		{
			memcpy(myVectorOnStack, aVectorOnStack.myVectorOnStack, sizeof(VectorOnStack));
		}
		else
		{
			for (CountType i = 0; i < Capacity; ++i)
			{
				myVectorOnStack[i] = aVectorOnStack[i];
			}
		}
		myCurrentSize = aVectorOnStack.Size();

		return *this;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline const Type& VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::operator[](const CountType& aIndex) const
	{
		assert((aIndex >= 0 && aIndex < myCurrentSize) && "Index out of bounds!");
		return myVectorOnStack[aIndex];
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline Type& VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::operator[](const CountType& aIndex)
	{
		assert((aIndex >= 0 && aIndex < myCurrentSize) && "Index out of bounds!");
		return myVectorOnStack[aIndex];
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::Add(const Type& aObject)
	{
		assert((myCurrentSize < Capacity) && "VectorOnStack is full!");
		myVectorOnStack[myCurrentSize] = aObject;
		++myCurrentSize;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::Insert(CountType aIndex, Type& aObject)
	{
		assert((myCurrentSize < Capacity + 1) && "VectorOnStack is full!");

		assert((aIndex >= 0 && aIndex < Capacity) && "Index out of bounds!");

		if (aIndex >= myCurrentSize)
		{
			Add(aObject);
			return;
		}

		for (CountType i = (Size() - 1); i > aIndex; i--)
		{
			myVectorOnStack[i] = myVectorOnStack[i - 1];
		}

		myVectorOnStack[aIndex] = aObject;
		++myCurrentSize;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::DeleteCyclic(Type& aObject)
	{
		for (CountType i = 0; i < myCurrentSize; ++i)
		{
			if (&myVectorOnStack[i] == &aObject)
			{
				delete myVectorOnStack[i];
				myVectorOnStack[i] = nullptr;
				myVectorOnStack[i] = myVectorOnStack[myCurrentSize - 1];
				--myCurrentSize;
				break;
			}
		}
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::DeleteCyclicAtIndex(CountType aItemNumber)
	{
		assert((aItemNumber >= 0 && aItemNumber < myCurrentSize) && "Index out of bounds!");

		delete myVectorOnStack[aItemNumber];
		myVectorOnStack[aItemNumber] = nullptr;

		myVectorOnStack[aItemNumber] = myVectorOnStack[myCurrentSize - 1];
		--myCurrentSize;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& aObject)
	{
		for (CountType i = 0; i < myCurrentSize; ++i)
		{
			if (myVectorOnStack[i] == aObject)
			{
				myVectorOnStack[i] = myVectorOnStack[myCurrentSize - 1];
				--myCurrentSize;
				break;
			}
		}
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(CountType aItemNumber)
	{
		assert((aItemNumber >= 0 && aItemNumber < myCurrentSize) && "Index out of bounds!");

		myVectorOnStack[aItemNumber] = myVectorOnStack[myCurrentSize - 1];
		--myCurrentSize;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::Clear()
	{
		myCurrentSize = 0;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::DeleteAll()
	{
		for (CountType i = 0; i < myCurrentSize; ++i)
		{
			delete myVectorOnStack[i];
			myVectorOnStack[i] = nullptr;
		}
		myCurrentSize = 0;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline CountType VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::Size() const
	{
		return myCurrentSize;
	}

	template <typename Type, int Capacity, typename CountType, bool UseSafeModeFlag>
	inline Type VectorOnStack<Type, Capacity, CountType, UseSafeModeFlag>::GetLast() const
	{
		return myVectorOnStack[myCurrentSize - 1];
	}
} 
