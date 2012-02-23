#pragma once
#include <Windows.h>
namespace Utility {
	template<typename T>
	void inline SafeRelease(T object) {object->Release;}

	template<typename T> // @TODO: dont use this, some kind of deprecated
	void inline SafeDelete(T object) {delete object; object = NULL;}
	};