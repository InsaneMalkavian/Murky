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
		
        ID3D11VertexShader*		m_VertexShader;
        ID3D11PixelShader*		m_PixelShader;
		
        ID3D11InputLayout*		m_InputLayout;
        ID3D11Buffer*			m_VertexBuffer;
		ID3D11Buffer*			m_IndexBuffer;
        size_t					m_TotalVertices;
		
		// @TODO: code below should be moved to ModelObject

        ID3D11ShaderResourceView*	m_ColorMap;
        ID3D11SamplerState*			m_ColorMapSampler;
		
        ID3D11Buffer*			m_ViewCB;
        ID3D11Buffer*			m_ProjCB;
        ID3D11Buffer*			m_WorldCB;
        //XMMATRIX				m_ProjMatrix;
	};

