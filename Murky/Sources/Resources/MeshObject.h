#pragma once
#include "Baseobject.h"
#include "Utility.h"

class MeshObject : public BaseObject
	{
	friend class MeshManager;
	public:
		MeshObject(void) {
			ZeroMemory(this, sizeof(MeshObject));
			refCount=1;
		}
		MeshObject(std::wstring name) {
			this->name = name;
			refCount = 1;
			timeAdded = timeLastAccess = 0;
			}
		virtual ~MeshObject(void);
		
        ID3D11Buffer*			mVertexBuffer;
		ID3D11Buffer*			mIndexBuffer;
        size_t					mTotalVertices;
		UINT					mStride;
		
	};

