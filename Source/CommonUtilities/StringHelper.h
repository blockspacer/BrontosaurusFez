#pragma once
#include <stdlib.h> // mbstowcs_s
#include <string.h> // strlen

namespace CU
{
//	inline wchar_t* CharToWChar(const char* text)
//	{
//		size_t size = strlen(text) + 1;
//		wchar_t* wa = new wchar_t[size];
//		size_t outSize;
//#ifdef _WIN32
//		// retval , dst, dstsz, scr, len
//		mbstowcs_s(&outSize, wa, size, text, size - 1);
//#else
//		// dst, src, dstsz or len
//		outSize = mbstowcs(wa, text, size - 1);
//		outSize;
//#endif
//		return wa;
//	}

	inline wchar_t* CharToWChar(wchar_t aBuffer[], const char* aText)
	{
		size_t size = strlen(aText) + 1;
		size_t outSize;
#ifdef _WIN32
		// retval , dst, dstsz, scr, len
		mbstowcs_s(&outSize, aBuffer, size, aText, size - 1);
#else
		// dst, src, dstsz or len
		outSize = mbstowcs(aBuffer, aText, size - 1);
		outSize;
#endif
		return aBuffer;
	}
}