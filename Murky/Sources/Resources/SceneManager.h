#pragma once
#include <list>
#include "ModelStaticObject.h"
#include "MeshManager.h"
#include "Renderer.h"

class SceneManager
	{
	public:
		SceneManager(Renderer* render);
		~SceneManager(void);
		void Render(void);
		ModelStaticObject* Get(std::wstring name);

	private:
		std::list<ModelStaticObject>		mStaticModels;
		MeshManager*						mMeshManager;
		Renderer*							mRender;
	};

