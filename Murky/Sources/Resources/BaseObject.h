#pragma once
#include <string>
class BaseObject
	{
	public:
		//BaseObject(void) { }
		//virtual ~BaseObject(void) { }
		std::wstring					name;				// unique name in filesystem
		size_t							refCount;			// reference count
		size_t							timeAdded;			// creation time, not impl
		size_t							timeLastAccess;		// lastaccess time, not impl
	};

