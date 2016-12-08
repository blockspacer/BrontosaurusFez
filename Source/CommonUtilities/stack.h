#pragma once
#include "GrowingArray.h"
#define STACK_TEMPLATE_ARG template<typename TYPE>

namespace CU
{
	template<typename TYPE, typename SIZE_T = int>
	class Stack
	{
	public:
		Stack();
		
		inline void Push(const TYPE& aValue);
		inline TYPE Pop();
		
		inline TYPE& Top();
		inline TYPE& At(const SIZE_T aIndex);
		
		inline SIZE_T Size();

		inline void Clear();
	private:
		CU::GrowingArray<TYPE, SIZE_T> myStack;
	
	};
	
	template<typename TYPE, typename SIZE_T>
	Stack<TYPE, SIZE_T>::Stack()
	{
		myStack.Init(10);
	}
	
	template<typename TYPE, typename SIZE_T>
	void Stack<TYPE, SIZE_T>::Push(const TYPE &aValue)
	{
		myStack.Add(aValue);
	}
	
	template<typename TYPE, typename SIZE_T>
	TYPE Stack<TYPE, SIZE_T>::Pop()
	{
		assert(!(myStack.Size() <= 0 && "Stack is empty"));
		TYPE temp = myStack[myStack.Size() - 1];
		myStack.RemoveCyclicAtIndex(myStack.Size() - 1);
		return temp;
	}
	
	template<typename TYPE, typename SIZE_T>
	TYPE& Stack<TYPE, SIZE_T>::Top()
	{
		assert(!(myStack.Size() <= 0 && "Stack is empty"));
		return myStack[myStack.Size() - 1];
	}

	template<typename TYPE, typename SIZE_T>
	inline TYPE& Stack<TYPE, SIZE_T>::At(const SIZE_T aIndex)
	{
		assert(!(myStack.Size() <= 0 && "Stack is empty"));
		assert(aIndex < myStack.Size() && "Index out of range");

		if (aIndex >= 0)
		{
			return myStack[aIndex];
		}
		else
		{
			return myStack[myStack.Size() + aIndex];
		}
	}
	
	template<typename TYPE, typename SIZE_T>
	SIZE_T Stack<TYPE, SIZE_T>::Size()
	{
		return myStack.Size();
	}

	template <typename TYPE, typename SIZE_T>
	void Stack<TYPE, SIZE_T>::Clear()
	{
		myStack.RemoveAll();
	}

}