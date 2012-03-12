#pragma once
#include "Utility.h"
#include "MeshObject.h"
#include "Renderer.h"

class MeshManager
	{
	public:
		MeshManager(Renderer* render);
		~MeshManager(void);
		MeshObject* Get(std::wstring name); // returns wanted resourceview instantly, will cause freeze!
		MeshObject* GetAsync(std::wstring name); // returns dummy RV instantly, notify when wanted RV will be available, no freeze
		void Release(std::wstring name); // call it when texture is not needed
		void Release(MeshObject* mesh); // call it when texture is not needed
		void ReleaseForce(std::wstring name); // will destroy resource instantly. maybe I shouldn't include it in public API
		void ReleaseForce(MeshObject* mesh); // will destroy resource instantly. maybe I shouldn't include it in public API
		size_t GetNumber(void) {return m_Meshes.size();} // returns number of loaded textures

	private:
		std::vector<MeshObject*>			m_Meshes;
		Renderer*							mRender;
};

