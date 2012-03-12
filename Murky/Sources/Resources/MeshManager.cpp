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

	MeshObject* mesh = NULL;
	MeshLoader loader;
	if (loader.LoadOBJ(name)) {
		HRESULT hr = S_OK;
		mesh = new MeshObject();
		m_Meshes.push_back(mesh);
		mesh->name = name;

		/*VerPC* vertices = new VerPC[loader.GetTotalVerts()];
		float* positions = loader.GetPositions();
		float* texCoords = loader.GetTexCoords(); // we dont need it really now

		for (int i = 0; i<loader.GetTotalVerts(); i++)
		{
			vertices[i].Pos = XMFLOAT3( *(positions + 0), *(positions + 1), *(positions + 2) );
			positions += 3;

			vertices[i].Color = XMFLOAT3(1.f,1.f,0.f);
			texCoords += 2;
		}
		
		D3D11_BUFFER_DESC vertexDesc;
		ZeroMemory(&vertexDesc, sizeof(vertexDesc));
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.ByteWidth = sizeof(VerPC) * loader.GetTotalVerts();

		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = vertices;

		hr = m_d3dDevice->CreateBuffer(&vertexDesc, &resourceData, &mesh->m_VertexBuffer);

		delete []vertices;
		if (FAILED(hr)) {
			DXTRACE_MSG(L"Failed to create vertex buffer!");
			return false;
			}*/


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
			}
		mesh->mStride = sizeof(SimpleVertex);
		mesh->mTotalVertices = 3;
		Log::logger.AddLine(L"Mesh: "+name+L" loaded");
	return mesh;
	}