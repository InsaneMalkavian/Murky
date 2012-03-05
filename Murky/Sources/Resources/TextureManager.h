#pragma once
#include "Utility.h"
#include "TexObject.h"

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
	size_t GetNumber(void) {return m_Textures.size();} // returns number of loaded textures

private:
	ID3D11Device* m_d3dDevice;
	std::vector<TexObject*> m_Textures;
};