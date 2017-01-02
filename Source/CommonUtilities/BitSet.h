#pragma once

#ifndef assert
#include <assert.h>
#endif // !assert

#define INDEX_TO_BIT(A_INDEX) (static_cast<Type>(1) << A_INDEX)

namespace CU
{
	template<bool CONDITION, typename IF_TRUE, typename IF_FALSE>
	struct COMPILE_TIME_IF
	{
		using IfType = IF_FALSE;
	};

	template<typename IF_TRUE, typename IF_FALSE>
	struct COMPILE_TIME_IF<true, IF_TRUE, IF_FALSE>
	{
		using IfType = IF_TRUE;
	};

	template<unsigned int BIT_COUNT>
	class CBitSet
	{
	public:
		using Type = typename COMPILE_TIME_IF<(BIT_COUNT <= 32), unsigned int, unsigned long long>::IfType;

		class CReference
		{
		public:
			friend class CBitSet<BIT_COUNT>;
			~CReference();

			CReference& operator=(const CReference& aValue);
			CReference& operator=(const bool aValue);
			operator bool() const;

		private:
			CReference(CBitSet& aBitSet, const Type aIndex);

			CBitSet& myBitSet;
			Type myIndex;
		};

		CBitSet();
		CBitSet(const Type aNumber);
		~CBitSet();

		inline CReference operator[](const Type aIndex);
		inline bool operator[](const Type aIndex) const;

		inline void Set(const Type aIndex, const bool aValue = true);
		inline void Flip(const Type aIndex);

		inline bool Get(const Type aIndex) const;
		inline bool Any() const;
		inline bool All() const;

	private:
		Type myBits;
	};

	template<unsigned int BIT_COUNT>
	CBitSet<BIT_COUNT>::CBitSet()
		: myBits(0u)
	{
		static_assert(BIT_COUNT <= 64, "bitset not implemented to hold larger than unsigned long long");
	}

	template<unsigned int BIT_COUNT>
	CBitSet<BIT_COUNT>::CBitSet(const Type aNumber)
		: myBits(aNumber)
	{
		static_assert(BIT_COUNT <= 64, "bitset not implemented to hold larger than unsigned long long");
	}

	template<unsigned int BIT_COUNT>
	CBitSet<BIT_COUNT>::~CBitSet()
	{
	}

	template<unsigned int BIT_COUNT>
	inline typename CBitSet<BIT_COUNT>::CReference CBitSet<BIT_COUNT>::operator[](const Type aIndex)
	{
		assert(aIndex < BIT_COUNT && "bitset index out of range");
		return CReference(*this, aIndex);
	}

	template<unsigned int BIT_COUNT>
	inline bool CBitSet<BIT_COUNT>::operator[](const Type aIndex) const
	{
		assert(aIndex < BIT_COUNT && "bitset index out of range");
		return Get(aIndex);
	}

	template<unsigned int BIT_COUNT>
	inline void CBitSet<BIT_COUNT>::Set(const Type aIndex, const bool aValue)
	{
		assert(aIndex < BIT_COUNT && "bitset index out of range");
		aValue ? (myBits |= INDEX_TO_BIT(aIndex)) : (myBits ^= INDEX_TO_BIT(aIndex));
	}

	template<unsigned int BIT_COUNT>
	inline void CBitSet<BIT_COUNT>::Flip(const Type aIndex)
	{
		assert(aIndex < BIT_COUNT && "bitset index out of range");
		myBits ^= INDEX_TO_BIT(aIndex);
	}

	template<unsigned int BIT_COUNT>
	inline bool CBitSet<BIT_COUNT>::Get(const Type aIndex) const
	{
		assert(aIndex < BIT_COUNT && "bitset index out of range");
		return (myBits & INDEX_TO_BIT(aIndex)) > 0u;
	}

	template<unsigned int BIT_COUNT>
	inline bool CBitSet<BIT_COUNT>::Any() const
	{
		return myBits > 0u;
	}

	template<unsigned int BIT_COUNT>
	inline bool CBitSet<BIT_COUNT>::All() const
	{
		for (unsigned int i = 0; i < BIT_COUNT; ++i)
		{
			if (Get(i) == false)
			{
				return false;
			}
		}

		return true;
	}

	template<unsigned int BIT_COUNT>
	inline typename CBitSet<BIT_COUNT>::CReference& CBitSet<BIT_COUNT>::CReference::operator=(const CReference& aValue)
	{
		*this = static_cast<bool>(aValue);
		return *this;
	}

	template<unsigned int BIT_COUNT>
	inline typename CBitSet<BIT_COUNT>::CReference& CBitSet<BIT_COUNT>::CReference::operator=(const bool aValue)
	{
		myBitSet.Set(myIndex, aValue);
		return *this;
	}

	template<unsigned int BIT_COUNT>
	inline CBitSet<BIT_COUNT>::CReference::operator bool() const
	{
		return myBitSet.Get(myIndex);
	}

	template<unsigned int BIT_COUNT>
	CBitSet<BIT_COUNT>::CReference::CReference(CBitSet& aBitSet, const Type aIndex)
		: myBitSet(aBitSet)
		, myIndex(aIndex)
	{
		assert(aIndex < BIT_COUNT && "bitset index out of range");
	}

	template<unsigned int BIT_COUNT>
	CBitSet<BIT_COUNT>::CReference::~CReference()
	{
	}
}

#undef INDEX_TO_BIT