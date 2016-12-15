#pragma once

//#include <GrowingArray.h>
//#include <StaticArray.h>
#include <mutex>
#include <cstdint>


#define BUFFER_SIZE 512


struct SRenderMessage;

class CSynchronizer
{
public:
	using size_type = std::uint32_t;
	enum eBufferIndex : char
	{
		eFirst,
		eSecond,
		eThird,
		eSize
	};

public:
	CSynchronizer();
	~CSynchronizer();

	inline SRenderMessage* operator[](const size_type aIndex);
	inline void operator <<(SRenderMessage* aObject);
	inline size_type operator!() const;
	inline void SwapWrite();
	inline void SwapRead();
	inline void ClearWrite();
	inline void ClearAll();

private:
	CU::StaticArray<CU::GrowingArray<SRenderMessage*, size_type>, eSize> myBuffers; //TODO: Fixa staticArray sizetype?

	std::mutex mySwapMutex;

	eBufferIndex myWriteTo;
	eBufferIndex myReadFrom;
	eBufferIndex myFreeBuffer;

	volatile bool myHasFresh;
};

inline void CSynchronizer::ClearAll()
{
	mySwapMutex.lock();

	for (char i = 0; i < myBuffers.Size(); ++i)
	{
		myBuffers[i].DeleteAll();
	}

	mySwapMutex.unlock();
}

inline CSynchronizer::CSynchronizer()
{
	myBuffers[eFirst].Init(BUFFER_SIZE);
	myBuffers[eSecond].Init(BUFFER_SIZE);
	myBuffers[eThird].Init(BUFFER_SIZE);

	myWriteTo = eFirst;
	myReadFrom = eSecond/*eFirst*/;
	myFreeBuffer = eThird;

	myHasFresh = false;
}

inline CSynchronizer::~CSynchronizer()
{
	ClearAll();
}

 SRenderMessage* CSynchronizer::operator[](const size_type aIndex)
{
	return myBuffers[myReadFrom][aIndex];
}

inline void CSynchronizer::operator<<(SRenderMessage * aObject)
{
	myBuffers[myWriteTo].Add(aObject);
}

inline CSynchronizer::size_type CSynchronizer::operator!() const
{
	return myBuffers[myReadFrom].Size();
}

inline void CSynchronizer::SwapWrite()
{
	mySwapMutex.lock();

	eBufferIndex lastFree = myFreeBuffer;
	myFreeBuffer = myWriteTo;
	myWriteTo = lastFree;

	myHasFresh = true;

	mySwapMutex.unlock();

	ClearWrite();

}

inline void CSynchronizer::SwapRead()
{
	if (myHasFresh == false)
	{
		return;
	}
	
	mySwapMutex.lock();

	myHasFresh = false;

	//// MemoryPoolio this!
	//myBuffers[myReadFrom].DeleteAll();

	eBufferIndex lastFree = myFreeBuffer;
	myFreeBuffer = myReadFrom;
	myReadFrom = lastFree;
	//DL_PRINT("Read buffer: %d", myReadFrom);


	mySwapMutex.unlock();
}

inline void CSynchronizer::ClearWrite()
{
	myBuffers[myWriteTo].DeleteAll();
}
