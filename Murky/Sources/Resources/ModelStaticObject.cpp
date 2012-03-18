#include "ModelStaticObject.h"

ModelStaticObject::~ModelStaticObject(void)
	{
	}

void ModelStaticObject::Render(ID3D11DeviceContext* context) {
	// Set vertex buffer
    //UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
	context->IASetVertexBuffers( 0, 1, &mMesh->mVertexBuffer, &mMesh->mStride, &offset );

    // Set primitive topology
    context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// @TODO: this code has to be moved to manager
	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, NULL, 0);
	context->PSSetShader(mPixelShader, NULL, 0);
	context->Draw(mMesh->mTotalVertices, 0);

	}