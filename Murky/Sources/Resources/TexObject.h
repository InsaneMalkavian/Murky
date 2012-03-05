#pragma once
#include "Baseobject.h"
#include "Utility.h"

class TexObject : public BaseObject
	{
	friend class TextureManager;
	public:
		TexObject(void) {
			ZeroMemory(this, sizeof(TexObject));
			refCount=1;
		}
		TexObject(std::wstring name, ID3D11ShaderResourceView* shaderRV) : shaderRV(shaderRV) {
			this->name = name;
			refCount = 1;
			timeAdded = timeLastAccess = 0;
			}
		virtual ~TexObject(void);
	private:
		ID3D11ShaderResourceView*		shaderRV;			// dx11 resourceview, it is used outside
	};

