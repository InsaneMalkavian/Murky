#include "MeshManager.h"
#include "MeshLoader.h"
#include "Datatypes.h"

using namespace std;

MeshManager::MeshManager(Renderer* render) : mRender(render) {
	}

MeshManager::~MeshManager(void) {
	}

MeshObject* MeshManager::Get(wstring name) {
	for (size_t i = 0; i<m_Meshes.size(); ++i)
	if (m_Meshes[i]->name == name) {
		m_Meshes[i]->refCount++;
		Log::logger.AddLine(L"Mesh: "+name+L" already loaded");
		return m_Meshes[i];
		}

	if (name==L"test")
	{
		MeshObject* mesh = NULL;
		HRESULT hr = S_OK;
		mesh = new MeshObject();
		m_Meshes.push_back(mesh);
		mesh->name = name;


		// Create vertex buffer
		SimpleVertex vertices[] =
		{
			XMFLOAT3( 0.0f, 0.5f, 0.5f ),
			XMFLOAT3( 0.5f, -0.5f, 0.5f ),
			XMFLOAT3( -0.5f, -0.5f, 0.5f ),
		};
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex)*3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;
		hr = mRender->GetDevice()->CreateBuffer(&bd, &InitData, &mesh->mVertexBuffer);
		if (FAILED(hr))
			return false;
		mesh->mStride = sizeof(SimpleVertex);
		mesh->mTotalVertices = 3;
		Log::logger.AddLine(L"Mesh: "+name+L" loaded");
		return mesh;
	}
	MeshLoader loader;
	loader.LoadDAE(name+L".dae");

	MeshObject* mesh = NULL;
	HRESULT hr = S_OK;
	mesh = new MeshObject();
	m_Meshes.push_back(mesh);
	mesh->name = name;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = loader.GetTotalSize();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = loader.VertexBuffer();
	hr = mRender->GetDevice()->CreateBuffer(&bd, &InitData, &mesh->mVertexBuffer);
	if (FAILED(hr))
		return false;
	mesh->mStride = loader.GetVertexSize();
	mesh->mTotalVertices = loader.GetTotalVerts();
	Log::logger.AddLine(L"Mesh: "+name+L" loaded");
	return mesh;
	}