#include "TextureManager.h"
#include <D3DX11tex.h>
#include <string>
#include "Logger.h"

using namespace std;

static const wstring WSTR_DUMMY_TEXTURE = L"Textures\\dummy.dds";

TextureManager::TextureManager(ID3D11Device* device): m_d3dDevice(device) {
	// there is nothing to do here
	// load some necessary textures (dummy and etc)
	Get(WSTR_DUMMY_TEXTURE);
	Log::logger.AddLine(L"TexMgr started");
}

TextureManager::~TextureManager() {
	for(size_t i = 0; i < m_Textures.size(); ++i)
		Util::SafeRelease(m_Textures[i].shaderRV);
}

ID3D11ShaderResourceView* TextureManager::Get(wstring name) {
	// has this texture already been created?
	for (size_t i = 0; i<m_Textures.size(); ++i)
		if (m_Textures[i].name == name) {
			m_Textures[i].refCount++;
			Log::logger.AddLine(L"Texture: "+name+L" already loaded");
			return m_Textures[i].shaderRV;
			}

	// if not, create it
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory( &loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO) );
	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	ID3D11ShaderResourceView* rv = NULL;
	
	if (GetFileAttributes(name.c_str()) != DWORD(-1)) { // if file exist
		HR(D3DX11CreateShaderResourceViewFromFile(m_d3dDevice, name.c_str(), &loadInfo, 0, &rv, 0 ));
		}
	else { // this will cause endless recurse if dummy.dds doesnt exist!
		rv = Get(WSTR_DUMMY_TEXTURE);
		}

	m_Textures.push_back(TexObject(name, rv));
	Log::logger.AddLine(L"Texture: "+name+L" loaded");
	return rv;
}

// @TODO: implement it ;)
ID3D11ShaderResourceView* TextureManager::GetAsync(std::wstring name) {
	// unsupported
	ID3D11ShaderResourceView* rv = NULL;
	return rv;
	}

void TextureManager::Release(std::wstring name) {
	// has this texture been created?
	size_t removedItem = -1;
	for (size_t i = 0; i<m_Textures.size(); ++i)
		if (m_Textures[i].name == name) {
			m_Textures[i].refCount--;
			removedItem=i;
			Log::logger.AddLine(L"Texture: "+name+L" RefCount decreased");
			break;
			}
	if (removedItem!=-1 && m_Textures[removedItem].refCount==0) { // destroy texture
		Util::SafeRelease(m_Textures[removedItem].shaderRV);
		m_Textures.erase(m_Textures.begin()+removedItem); // maybe there should be +(removedItem-1), check it out!
		}
	}

void TextureManager::Release(ID3D11ShaderResourceView* rv) {
	// unsupported
	}

void TextureManager::ReleaseForce(std::wstring name) {
	// unsupported
	}

void TextureManager::ReleaseForce(ID3D11ShaderResourceView* rv) {
	// unsupported
	}
