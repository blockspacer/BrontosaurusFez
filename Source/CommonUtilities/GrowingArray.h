#pragma once
#pragma message("Growing Array compiled")
#include <assert.h>
#include <string.h> // memcpy
#include <initializer_list>
#include "QuickSort.h"


#define self (*this)

namespace CU
{
	template<typename ObjectType, typename SizeType = unsigned int, bool USE_SAFE_MODE = true>
	class GrowingArray
	{
	public:
		using iterator = ObjectType*;
		using const_iterator = const ObjectType*;

		GrowingArray();
		GrowingArray(const SizeType aStartCapacity);
		explicit GrowingArray(const SizeType aStartCapacity, const ObjectType& aItemToFillWith);
		GrowingArray(const std::initializer_list<ObjectType>& aInitializerList);
		GrowingArray(const GrowingArray& aGrowingArray);
		GrowingArray(GrowingArray&& aGrowingArray);

		~GrowingArray();

		GrowingArray& operator=(const GrowingArray& aGrowingArray);
		GrowingArray& operator=(GrowingArray&& aGrowingArray);

		void Init(const SizeType aStartCapacity);
		void Init(const SizeType aStartCapacity, const ObjectType& aItemToFillWith);
		void ReInit(const SizeType aStartCapacity);

		inline ObjectType& operator[](const SizeType aIndex);
		inline const ObjectType& operator[](const SizeType aIndex) const;

		inline ObjectType& At(const SizeType aIndex);
		inline const ObjectType& At(const SizeType aIndex) const;

		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		inline void Add(const ObjectType& aObject);
		inline void Add(ObjectType&& aObject);
		inline void Add(const GrowingArray& aArray);
		inline void Insert(const SizeType aIndex, const ObjectType& aObject);
		inline void Remove(const ObjectType& aObject);
		inline void RemoveAtIndex(const SizeType aIndex);
		inline void DeleteCyclic(const ObjectType& aObject);
		inline void DeleteCyclicAtIndex(const SizeType aIndex);
		inline void RemoveCyclic(const ObjectType& aObject);
		inline void RemoveCyclicAtIndex(const SizeType aIndex);
		inline SizeType Find(const ObjectType& aObject);
		inline bool Find(const ObjectType& aObject, SizeType& aReturnIndex);

		inline ObjectType& GetLast();
		inline const ObjectType& GetLast() const;
		inline ObjectType* AsPointer(const SizeType aIndex = 0);
		inline const ObjectType* AsPointer(const SizeType aIndex = 0) const;
		inline void* AsVoidPointer(const SizeType aIndex = 0);
		inline const void* AsVoidPointer(const SizeType aIndex = 0) const;

		inline ObjectType Pop();

		inline void RemoveAll();
		inline void DeleteAll();

		inline void Optimize();
		inline void ShrinkToFit();

		inline void QuickSort(std::function<bool(ObjectType, ObjectType)> aCompareFunction);

		inline void Resize(const SizeType aNewSize);
		inline void Resize(const SizeType aNewSize, const ObjectType& aObject);
		inline void Destroy();

		__forceinline SizeType Size() const;
		__forceinline SizeType Capacity() const;
		__forceinline bool IsInitialized() const;

		static const SizeType FoundNone = static_cast<SizeType>(-1);

		typedef SizeType size_type;

	private:
		inline void Reallocate(const SizeType aNewSize);

		ObjectType* myArray;
		SizeType mySize;
		SizeType myCapacity;
	};

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::GrowingArray()
	{
		mySize = 0;
		myCapacity = 0;
		myArray = nullptr;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::GrowingArray(const SizeType aStartCapacity) : GrowingArray()
	{
		Init(aStartCapacity);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::GrowingArray(const SizeType aStartCapacity, const ObjectType& aItemToFillWith) : GrowingArray()
	{
		Init(aStartCapacity, aItemToFillWith);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::GrowingArray(const std::initializer_list<ObjectType>& aInitializerList) : GrowingArray()
	{
		Init(static_cast<SizeType>(aInitializerList.size()));

		for (const ObjectType& initilizer : aInitializerList)
		{
			Add(initilizer);
		}
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::GrowingArray(const GrowingArray& aGrowingArray) : GrowingArray()
	{
		self = aGrowingArray;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::GrowingArray(GrowingArray&& aGrowingArray) : GrowingArray()
	{
		self = std::move(aGrowingArray);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::~GrowingArray()
	{
		Destroy();				//OM FEL, SKÄLL PÅ KEVIN! och Carl
	}


	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>& GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::operator=(const GrowingArray& aGrowingArray)
	{
		if (IsInitialized() == true)
		{
			Destroy();
		}

		if (aGrowingArray.IsInitialized() == true)
		{
			Init(aGrowingArray.myCapacity);

			mySize = aGrowingArray.mySize;

			if (USE_SAFE_MODE)
			{
				for (SizeType i = 0; i < aGrowingArray.Size(); ++i)
				{
					myArray[i] = aGrowingArray[i];
				}
			}
			else
			{
				if (Size() > 0)
				{
					memcpy(myArray, aGrowingArray.myArray, sizeof(ObjectType) * mySize);
				}
			}
		}

		return self;
	}


	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>& GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::operator=(GrowingArray&& aGrowingArray)
	{
		if (IsInitialized() == true)
		{
			delete[] myArray;
		}

		myArray = aGrowingArray.myArray;
		aGrowingArray.myArray = nullptr;
		mySize = aGrowingArray.mySize;
		aGrowingArray.mySize = 0;
		myCapacity = aGrowingArray.myCapacity;
		aGrowingArray.myCapacity = 0;

		return self;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Init(const SizeType aStartCapacity)
	{
		assert(IsInitialized() == false && "Growing array must not be initialized twice, consider ReInit");
		assert(aStartCapacity > 0 && "Growing array must not be inited with zero capacity");

		myArray = new ObjectType[aStartCapacity];
		myCapacity = aStartCapacity;
		mySize = 0;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Init(const SizeType aStartCapacity, const ObjectType& aItemToFillWith)
	{
		assert(IsInitialized() == false && "Growing array must not be initialized twice, consider ReInit");
		assert(aStartCapacity > 0 && "Growing array must not be inited with zero capacity");

		Init(aStartCapacity);

		for (SizeType i = 0; i < aStartCapacity; ++i)
		{
			Add(aItemToFillWith);
		}
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::ReInit(const SizeType aStartCapacity)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert(aStartCapacity > 0 && "Growing array must not be inited with zero capacity");

		Destroy();
		Init(aStartCapacity);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline ObjectType& GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::operator[](const SizeType aIndex)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return myArray[aIndex];
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline const ObjectType& GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::operator[](const SizeType aIndex) const
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return myArray[aIndex];
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline ObjectType & GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::At(const SizeType aIndex)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return myArray[aIndex];
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline const ObjectType & GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::At(const SizeType aIndex) const
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return myArray[aIndex];
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline typename GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::iterator GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::begin()
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		return myArray;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline typename GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::const_iterator GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::begin() const
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		return myArray;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline typename GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::iterator GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::end()
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		return (myArray + mySize);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline typename GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::const_iterator GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::end() const
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		return (myArray + mySize);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Add(const ObjectType& aObject)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		if (mySize >= myCapacity)
		{
			Reallocate(myCapacity * 2);
		}

		myArray[mySize] = aObject;
		++mySize;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Add(ObjectType&& aObject)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		if (mySize >= myCapacity)
		{
			Reallocate(myCapacity * 2);
		}

		myArray[mySize] = std::move(aObject);
		++mySize;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Add(const GrowingArray& aArray)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		for (ComponentId i = 0; i < anArray.Size(); ++i)
		{
			Add(anArray[i]);
		}
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Insert(const SizeType aIndex, const ObjectType& aObject)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		if (mySize >= myCapacity)
		{
			Reallocate(myCapacity * 2);
		}

		if (USE_SAFE_MODE)
		{
			for (SizeType i = mySize; i > aIndex; i--)
			{
				myArray[i] = myArray[i - 1];
			}
		}
		else
		{
			memmove(myArray + aIndex + 1, myArray + aIndex, sizeof(ObjectType) * (mySize - aIndex)); //if wrong, it's here
		}

		myArray[aIndex] = aObject;
		++mySize;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Remove(const ObjectType& aObject)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		SizeType index = Find(aObject);
		if (index != FoundNone)
		{
			RemoveAtIndex(index);
		}
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::RemoveAtIndex(const SizeType aIndex)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");


		if (mySize > 1)
		{
			if (USE_SAFE_MODE)
			{
				for (SizeType i = aIndex; i < myCapacity; ++i)
				{
					myArray[i] = myArray[i + 1];
				}
			}
			else
			{
				memmove(myArray + aIndex, myArray + aIndex + 1, sizeof(ObjectType) * (mySize - aIndex)); //if wrong, it's here
			}

			--mySize;
		}
		else
		{
			RemoveAll();
		}
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::DeleteCyclic(const ObjectType& aObject)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		SizeType index = Find(aObject);
		if (index != FoundNone)
		{
			DeleteCyclicAtIndex(index);
		}
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::DeleteCyclicAtIndex(const SizeType aIndex)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds!");

		if (myArray[aIndex] != nullptr)
		{
			delete myArray[aIndex];
			myArray[aIndex] = nullptr;
		}

		myArray[aIndex] = myArray[mySize - 1];
		--mySize;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::RemoveCyclic(const ObjectType& aObject)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		SizeType index = Find(aObject);
		if (index != FoundNone)
		{
			RemoveCyclicAtIndex(index);
		}
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::RemoveCyclicAtIndex(const SizeType aIndex)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds!");

		myArray[aIndex] = myArray[mySize - 1];
		myArray[mySize - 1] = ObjectType(); //trying this to fix sharedptr error, mvh carl
		--mySize;
	}


	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline SizeType GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Find(const ObjectType& aObject)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		for (SizeType i = 0; i < mySize; ++i)
		{
			if (myArray[i] == aObject)
			{
				return i;
			}
		}

		return FoundNone;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	bool GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Find(const ObjectType& aObject, SizeType& aReturnIndex)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		for (SizeType i = 0; i < mySize; ++i)
		{
			if (myArray[i] == aObject)
			{
				aReturnIndex = i;
				return true;
			}
		}

		return false;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline ObjectType& GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::GetLast()
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		return myArray[mySize - 1];
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline const ObjectType& GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::GetLast() const
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		return myArray[mySize - 1];
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline ObjectType * GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::AsPointer(const SizeType aIndex)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return (myArray + aIndex);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline const ObjectType * GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::AsPointer(const SizeType aIndex) const
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return (myArray + aIndex);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void * GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::AsVoidPointer(const SizeType aIndex)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return static_cast<void*>(myArray + aIndex);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline const void * GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::AsVoidPointer(const SizeType aIndex) const
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return static_cast<void*>(myArray + aIndex);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline ObjectType GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Pop()
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		assert(Size() > 0 && "GrowingArray is empty.");
		return myArray[--mySize];
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::RemoveAll()
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		mySize = 0;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::DeleteAll()
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		for (SizeType i = 0; i < mySize; ++i)
		{
			if (myArray[i] != nullptr)
			{
				delete myArray[i];
				myArray[i] = nullptr;
			}
		}
		mySize = 0;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Optimize()
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		Reallocate(mySize);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::ShrinkToFit()
	{
		Optimize();
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void CU::GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::QuickSort(std::function<bool(ObjectType, ObjectType)> aCompareFunction)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		CU::QuickSort(*this, 0, mySize, aCompareFunction);
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	__forceinline SizeType GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Size() const
	{
		return mySize;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	__forceinline SizeType GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Capacity() const
	{
		return myCapacity;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline bool GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::IsInitialized() const
	{
		return myArray != nullptr;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Resize(SizeType aNewSize)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		if (aNewSize > myCapacity)
		{
			Reallocate(aNewSize);
		}

		mySize = aNewSize;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Resize(SizeType aNewSize, const ObjectType& aObject)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");

		Resize(aNewSize);

		for (SizeType i = 0; i < Size(); ++i)
		{
			myArray[i] = aObject;
		}
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Reallocate(const SizeType aNewSize)
	{
		assert(IsInitialized() == true && "GrowingArray not yet initialized.");
		ObjectType *newArray = new ObjectType[aNewSize];

		for (SizeType i = 0; i < mySize; ++i)
		{
			newArray[i] = myArray[i];
		}

		delete[] myArray;
		myArray = newArray;
		myCapacity = aNewSize;
	}

	template<typename ObjectType, typename SizeType, bool USE_SAFE_MODE>
	inline void GrowingArray<ObjectType, SizeType, USE_SAFE_MODE>::Destroy()
	{
		delete[] myArray;
		myArray = nullptr;
		myCapacity = 0;
		mySize = 0;
	}
}
