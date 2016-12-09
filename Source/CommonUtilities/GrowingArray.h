#pragma once
#include <assert.h>
#include <string.h> // memcpy
#include "DL_Debug.h"

namespace CU
{
	template<typename ObjectType, typename SizeType = unsigned int>
	class GrowingArray
	{
	public:
		GrowingArray();
		GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag = true);
		GrowingArray(std::initializer_list<ObjectType> aInitializerList);
		GrowingArray(const GrowingArray& aGrowingArray);
		GrowingArray(GrowingArray&& aGrowingArray);
		GrowingArray(SizeType aNrOfItems, const ObjectType& aItemToFillWith, bool aUseSafeModeFlag = true);

		~GrowingArray();

		GrowingArray &operator=(const GrowingArray& aGrowingArray);
		GrowingArray &operator=(GrowingArray&& aGrowingArray);

		void Init(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag = true);
		void Init(SizeType aNrOfItems, const ObjectType& aItemToFillWith, bool aUseSafeModeFlag = true);
		void ReInit(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag = true);

		inline ObjectType &operator[](const SizeType& aIndex);
		inline const ObjectType &operator[](const SizeType& aIndex) const;

		inline ObjectType &At(const SizeType& aIndex);
		inline const ObjectType &At(const SizeType& aIndex) const;

		inline void Add(const ObjectType& aObject);
		inline void Add(ObjectType&& aObject);
		inline void Add(const GrowingArray& anArray);
		inline void Insert(SizeType aIndex, ObjectType& aObject);
		inline void Remove(ObjectType& aObject);
		inline void RemoveAtIndex(SizeType aItemNumber);
		inline void DeleteCyclic(ObjectType& aObject);
		inline void DeleteCyclicAtIndex(SizeType aItemNumber);
		inline void RemoveCyclic(const ObjectType& aObject);
		inline void RemoveCyclicAtIndex(SizeType aItemNumber);
		inline SizeType Find(const ObjectType& aObject);
		inline bool Find(const ObjectType& aObject, SizeType & aReturnIndex);

		inline ObjectType &GetLast();
		inline const ObjectType &GetLast() const;

		static const SizeType FoundNone = static_cast<SizeType>(-1);
			   
		inline void RemoveAll();
		inline void DeleteAll();

		void Optimize();
		__forceinline SizeType Size() const;
		__forceinline SizeType Capacity() const;
		__forceinline bool IsInitialized() const;

		inline void Resize(SizeType aNewSize);
		inline void Resize(SizeType aNewSize, const ObjectType& aObject);
		inline void Reallocate(SizeType aNewSize);
		inline void Destroy();

		typedef SizeType size_type;

	private:

		ObjectType *myArray;
		SizeType mySize;
		SizeType myCapacity;
		bool mySafeModeFlag;
		bool myIsInit;
	};

	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::GrowingArray()
	{
		mySize = 0;
		myCapacity = 0;
		myArray = nullptr;
		mySafeModeFlag = true;
		myIsInit = false;
	}

	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::GrowingArray(SizeType aNrOfRecommendedItems, bool aUseSafeModeFlag) : GrowingArray()
	{
		mySize = 0;
		Init(aNrOfRecommendedItems, aUseSafeModeFlag);
	}

	template <typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::GrowingArray(std::initializer_list<ObjectType> aInitializerList) : GrowingArray()
	{
		Init(static_cast<SizeType>(aInitializerList.size()));

		for (ObjectType initilizer : aInitializerList)
		{
			Add(initilizer);
		}
	}

	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::GrowingArray(const GrowingArray& aGrowingArray) : GrowingArray()
	{
		if (aGrowingArray.myIsInit == true)
		{
			Init(aGrowingArray.myCapacity, aGrowingArray.mySafeModeFlag);

			mySize = aGrowingArray.mySize;

			if (mySafeModeFlag == true)
			{
				for (SizeType i = 0; i < aGrowingArray.Size(); ++i)
				{
					myArray[i] = aGrowingArray[i];
				}
			}
			else
			{
				/*delete[] myArray;
				myArray = nullptr;
				myArray = new ObjectType[mySize];*/
				memcpy(myArray, aGrowingArray.myArray, sizeof(*myArray) * aGrowingArray.mySize);
			}
			myIsInit = true;
		}
	}

	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::GrowingArray(GrowingArray&& aGrowingArray) : GrowingArray()
	{
		myArray = aGrowingArray.myArray;
		mySize = aGrowingArray.mySize;
		myCapacity = aGrowingArray.myCapacity;
		mySafeModeFlag = aGrowingArray.mySafeModeFlag;
		myIsInit = true;

		aGrowingArray.myArray = nullptr;
	}

	template <typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::GrowingArray(SizeType aNrOfItems, const ObjectType& aItemToFillWith, bool aUseSafeModeFlag) : GrowingArray()
	{
		Init(aNrOfItems , aUseSafeModeFlag);

		for (SizeType i = 0; i < aNrOfItems; ++i)
		{
			Add(aItemToFillWith);
		}
	}

	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>::~GrowingArray()
	{
		if (myIsInit == true)
		{
			delete[] myArray;				//OM FEL, SKÄLL PÅ KEVIN!
		}
	}


	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>& GrowingArray<ObjectType, SizeType>::operator=(const GrowingArray& aGrowingArray)
	{
		if (myIsInit == true)
		{
			Destroy();
		}

		Init(aGrowingArray.myCapacity, aGrowingArray.mySafeModeFlag);

		mySize = aGrowingArray.mySize;

		if (mySafeModeFlag == true)
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
				memcpy(myArray, aGrowingArray.myArray, sizeof(*myArray) * mySize);
			}
		}

		return *this;
	}


	template<typename ObjectType, typename SizeType>
	GrowingArray<ObjectType, SizeType>& GrowingArray<ObjectType, SizeType>::operator=(GrowingArray&& aGrowingArray)
	{
		if (myArray != nullptr)
		{
			delete[] myArray;
		}

		myArray = aGrowingArray.myArray;
		mySize = aGrowingArray.mySize;
		myCapacity = aGrowingArray.myCapacity;
		mySafeModeFlag = aGrowingArray.mySafeModeFlag;
		myIsInit = true;

		aGrowingArray.myArray = nullptr;
		aGrowingArray.myIsInit = false;
		return *this;
	}

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Init(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag)
	{
		assert(myIsInit == false && "Growing array must not be initialized twice, consider ReInit");
		assert(aNrOfRecomendedItems > 0 && "Growing array must not be inited with zero recomended items that will cause a heap corruption which is super uber bad!!!!");

		if (aNrOfRecomendedItems == 1)
		{
			int br = 0;
			br++;
		}
		myArray = new ObjectType[aNrOfRecomendedItems];
		myCapacity = aNrOfRecomendedItems;
		mySize = 0;
		mySafeModeFlag = aUseSafeModeFlag;
		myIsInit = true;
	}

	template <typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Init(SizeType aNrOfItems, const ObjectType& aItemToFillWith, bool aUseSafeModeFlag)
	{
		if (myIsInit == true)
		{
			DL_ASSERT("Can't use that init on a already inited growing array");
		}

		Init(aNrOfItems, aUseSafeModeFlag);

		for (int i = 0; i < aNrOfItems; ++i)
		{
			Add(aItemToFillWith);
		}
	}

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::ReInit(SizeType aNrOfRecomendedItems, bool aUseSafeModeFlag)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");

		Destroy();
		Init(aNrOfRecomendedItems, aUseSafeModeFlag);
	}

	template<typename ObjectType, typename SizeType>
	inline ObjectType& GrowingArray<ObjectType, SizeType>::operator[](const SizeType& aIndex)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return myArray[aIndex];
	}

	template<typename ObjectType, typename SizeType>
	inline const ObjectType& GrowingArray<ObjectType, SizeType>::operator[](const SizeType& aIndex) const
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return myArray[aIndex];
	}

	template<typename ObjectType, typename SizeType>
	inline ObjectType& GrowingArray<ObjectType, SizeType>::At(const SizeType& aIndex)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return myArray[aIndex];
	}

	template<typename ObjectType, typename SizeType>
	inline const ObjectType& GrowingArray<ObjectType, SizeType>::At(const SizeType& aIndex) const
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		assert((aIndex >= 0 && aIndex < mySize) && "Index out of bounds");
		return myArray[aIndex];
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Add(const ObjectType& aObject)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		
		if (mySize >= myCapacity)
		{
			Reallocate(myCapacity * 2);
		}

		myArray[mySize] = aObject;
		++mySize;
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Add(ObjectType&& aObject)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");

		if (mySize >= myCapacity)
		{
			Reallocate(myCapacity * 2);
		}

		myArray[mySize] = std::move(aObject);
		++mySize;
	}

	template <typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Add(const GrowingArray& anArray)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");

		for (ComponentId i = 0; i < anArray.Size(); ++i)
		{
			Add(anArray[i]);
		}
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Insert(SizeType aIndex, ObjectType& aObject)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");

		if (mySize >= myCapacity)
		{
			Reallocate(myCapacity * 2);
		}

		++mySize;

		for (SizeType i = mySize - 1; i > aIndex; i--)
		{
			myArray[i] = myArray[i - 1];
		}
		myArray[aIndex] = aObject;
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Remove(ObjectType& aObject)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");

		SizeType index = Find(aObject);
		if (index != FoundNone)
		{
			RemoveAtIndex(index);
		}
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::RemoveAtIndex(SizeType aItemNumber)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		assert((aItemNumber >= 0 && aItemNumber < mySize) && "Index out of bounds");

		if (mySize > 1)
		{
			--mySize;
			for (SizeType i = aItemNumber; i < myCapacity; ++i)
			{
				myArray[i] = myArray[i + 1];
			}
		}
		else
		{
			RemoveAll();
		}
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::DeleteCyclic(ObjectType& aObject)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");

		for (SizeType i = 0; i < mySize; ++i)
		{
			if (myArray[i] == aObject)
			{
				delete myArray[i];
				myArray[i] = nullptr;
				myArray[i] = myArray[mySize - 1];
				--mySize;
				break;
			}
		}
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::DeleteCyclicAtIndex(SizeType aItemNumber)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		assert((aItemNumber >= 0 && aItemNumber < mySize) && "Index out of bounds!");

		if (myArray[aItemNumber] != nullptr)
		{
			delete myArray[aItemNumber];
			myArray[aItemNumber] = nullptr;
		}

		myArray[aItemNumber] = myArray[mySize - 1];
		--mySize;
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::RemoveCyclic(const ObjectType& aObject)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		for (SizeType i = 0; i < mySize; ++i)
		{
			if (myArray[i] == aObject)
			{
				myArray[i] = myArray[mySize - 1];
				--mySize;
				break;
			}
		}

	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::RemoveCyclicAtIndex(SizeType aItemNumber)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		assert((aItemNumber >= 0 && aItemNumber < mySize) && "Index out of bounds!");

		myArray[aItemNumber] = myArray[mySize - 1];
		myArray[mySize - 1] = ObjectType(); //trying this to fix sharedptr error, mvh carl
		--mySize;
	}


	template<typename ObjectType, typename SizeType>
	inline SizeType GrowingArray<ObjectType, SizeType>::Find(const ObjectType& aObject)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		for (SizeType i = 0; i < mySize; ++i)
		{
			if (myArray[i] == aObject)
			{
				return i;
			}
		}
		return FoundNone;
	}

	template <typename ObjectType, typename SizeType>
	bool GrowingArray<ObjectType, SizeType>::Find(const ObjectType& aObject, SizeType& aReturnIndex)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
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

	template<typename ObjectType, typename SizeType>
	inline ObjectType& GrowingArray<ObjectType, SizeType>::GetLast()
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		return myArray[mySize - 1];
	}

	template<typename ObjectType, typename SizeType>
	inline const ObjectType& GrowingArray<ObjectType, SizeType>::GetLast() const
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		return myArray[mySize - 1];
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::RemoveAll()
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		mySize = 0;
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::DeleteAll()
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
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

	template<typename ObjectType, typename SizeType>
	void GrowingArray<ObjectType, SizeType>::Optimize()
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		ObjectType *tempArray = new ObjectType[mySize];
		for (SizeType i = 0; i < mySize; ++i)
		{
			tempArray[i] = myArray[i];
		}

		if (myArray != nullptr)
		{
			delete myArray;
			myArray = nullptr;
		}

		myCapacity = mySize;
		myArray = new ObjectType[mySize];

		for (SizeType i = 0; i < mySize; ++i)
		{
			myArray[i] = tempArray[i];
		} 
			delete tempArray;
			tempArray = nullptr;
	}

	template<typename ObjectType, typename SizeType>
	__forceinline SizeType GrowingArray<ObjectType, SizeType>::Size() const
	{
		//assert(myIsInit == true && "GrowingArray not yet initialized.");
		return mySize;
		
	}

	template<typename ObjectType, typename SizeType>
	__forceinline SizeType GrowingArray<ObjectType, SizeType>::Capacity() const
	{
		//assert(myIsInit == true && "GrowingArray not yet initialized.");
		return myCapacity;
	}

	template<typename ObjectType, typename SizeType>
	inline bool GrowingArray<ObjectType, SizeType>::IsInitialized() const
	{
		return myIsInit;
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Resize(SizeType aNewSize)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");

		if (aNewSize > myCapacity)
		{
			Reallocate(aNewSize);
		}

		mySize = aNewSize;
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Resize(SizeType aNewSize, const ObjectType& aObject)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");

		Resize(aNewSize);

		for (int i = 0; i < Size(); ++i)
		{
			myArray[i] = aObject;
		}
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Reallocate(SizeType aNewSize)
	{
		assert(myIsInit == true && "GrowingArray not yet initialized.");
		ObjectType *newArray = new ObjectType[aNewSize];
		for (SizeType i = 0; i < mySize; ++i)
		{
			newArray[i] = myArray[i];
		}
		
		delete[] myArray;
		myArray = nullptr;
		myArray = newArray;
		myCapacity = aNewSize;
	}

	template<typename ObjectType, typename SizeType>
	inline void GrowingArray<ObjectType, SizeType>::Destroy()
	{
		delete[] myArray;
		myArray = nullptr;
		myCapacity = 0;
		mySize = 0;
		myIsInit = false;
	}
} 