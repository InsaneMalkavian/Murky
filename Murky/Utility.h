#pragma once
#include <Windows.h>
#include <D3D11.h>
#include <DxErr.h>
#include <vector>
#include <string>

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