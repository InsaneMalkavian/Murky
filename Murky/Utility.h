#pragma once
#include <Windows.h>
#include <D3D11.h>
#include <DxErr.h>
#include <vector>
#include <string>
#include <stdio.h>
#include "Logger.h"

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)

namespace Util {
	template<typename T>
	void inline SafeRelease(T object) {if (object) object->Release(); object=NULL;}

	template<typename T> // @TODO: dont use this, some kind of deprecated
	void inline SafeDelete(T object) {delete object; object = NULL;}

	};

	#if defined(DEBUG) | defined(_DEBUG)
		#ifndef HR
		#define HR(x)                                              \
		{                                                          \
			HRESULT hr = (x);                                      \
			if(FAILED(hr))                                         \
			{                                                      \
				DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
			}                                                      \
		}
		#endif

	#else
		#ifndef HR
		#define HR(x) (x)
		#endif
	#endif 
/*
	#if defined(DEBUG) | defined(_DEBUG)
		#ifndef LogOnDebug
		#define LogOnDebug(x)                                      \
		{                                                          \
			Log::logger.AddLine(x)                                 \
		}
		#endif

	#else
		#ifndef LogOnDebug
		#define LogOnDebug(x) (0)
		#endif
	#endif  */