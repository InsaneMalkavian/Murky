#pragma once
#include <D3D11.h>
#include "BaseObject.h"
#include "MeshManager.h"
#include "Utility.h"

class ModelStaticObject : public BaseObject
	{
	friend class SceneManager;
	public:
		ModelStaticObject(void) {
			ZeroMemory(this, sizeof(ModelStaticObject));
			refCount=1;
		}
		virtual ~ModelStaticObject(void);
		void Render(ID3D11DeviceContext* context);
	private:
		ID3D11VertexShader*		mVertexShader;
        ID3D11PixelShader*		mPixelShader;		
        ID3D11InputLayout*		mInputLayout;
		MeshObject*				mMesh;

        // @TODO: I'm still not sure, is it a right place for it

        //ID3D11ShaderResourceView*	m_ColorMap;
        //ID3D11SamplerState*			m_ColorMapSampler;
		
        //ID3D11Buffer*			m_ViewCB;
        //ID3D11Buffer*			m_ProjCB;
        //ID3D11Buffer*			m_WorldCB;
        //XMMATRIX				m_ProjMatrix;
	};

