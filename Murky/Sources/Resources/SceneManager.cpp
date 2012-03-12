#include "SceneManager.h"

// @TODO: kind of hack
#include "Shaders.h"

using namespace std;

SceneManager::SceneManager(Renderer* render) : mRender(render) {
	mMeshManager = new MeshManager(mRender);
	}

SceneManager::~SceneManager(void) {
	}

void SceneManager::Render(void) {
	// iterate through all models
	// we have to sort model by shader, set up shaders and after that call Render
	// so 
	for(list<ModelStaticObject>::iterator list_iter = mStaticModels.begin(); list_iter != mStaticModels.end(); ++list_iter) {
		list_iter->Render(mRender->GetDeviceContext());
		}
	}

ModelStaticObject* SceneManager::Get(std::wstring name) {
	list<ModelStaticObject>::iterator list_iter;
	for (list_iter = mStaticModels.begin(); list_iter != mStaticModels.end(); ++list_iter)
	if (list_iter->name == name) {
		list_iter->refCount++;
		Log::logger.AddLine(L"StaticModel: "+name+L" already loaded");
		return &*list_iter;
		}
	mStaticModels.push_back(ModelStaticObject());
	// here we load static model
	list_iter = mStaticModels.end();
	--list_iter;
	list_iter->name = name;
	list_iter->mPixelShader = fx::SimpleShader::PS;
	list_iter->mVertexShader = fx::SimpleShader::VS;
	list_iter->mInputLayout = fx::SimpleShader::IL;
	list_iter->mMesh = mMeshManager->Get(L"Test");
	return  &*list_iter;
	}