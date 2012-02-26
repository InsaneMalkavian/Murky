#pragma once
#include "Utility.h"

// @TODO: base class with refCount, times.
struct TexObject {
	TexObject() {
		ZeroMemory(this, sizeof(TexObject));
		refCount=1;
	}
	TexObject(std::wstring name, ID3D11ShaderResourceView* shaderRV) : name(name), shaderRV(shaderRV) {
		refCount=1;
		timeAdded=0;
		timeLastAccess=0;
		}

	std::wstring					name;				// unique name in filesystem
	ID3D11ShaderResourceView*		shaderRV;			// dx11 resourceview, it is used outside
	size_t							refCount;			// reference count
	size_t							timeAdded;			// creation time, not impl
	size_t							timeLastAccess;		// lastaccess time, not impl
	};

// @TODO: implement times

class TextureManager {
public:
	TextureManager(ID3D11Device* device);
	~TextureManager();
	ID3D11ShaderResourceView* Get(std::wstring name); // returns wanted resourceview instantly, will cause freeze!
	ID3D11ShaderResourceView* GetAsync(std::wstring name); // returns dummy RV instantly, notify when wanted RV will be available, no freeze
	void Release(std::wstring name); // call it when texture is not needed
	void Release(ID3D11ShaderResourceView* rv); // call it when texture is not needed
	void ReleaseForce(std::wstring name); // will destroy resource instantly. maybe I shouldn't include it in public API
	void ReleaseForce(ID3D11ShaderResourceView* rv); // will destroy resource instantly. maybe I shouldn't include it in public API
	size_t GetTexturesNumber(void) {return m_Textures.size();} // returns number of loaded textures

private:
	ID3D11Device* m_d3dDevice;
	std::vector<TexObject> m_Textures;
};