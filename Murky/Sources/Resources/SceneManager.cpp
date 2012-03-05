#include "SceneManager.h"

using namespace std;

SceneManager::SceneManager(void) {
	}

SceneManager::~SceneManager(void) {
	}

void SceneManager::Render(void) {
	// iterate through all models
	for(list<ModelStaticObject>::iterator list_iter = mStaticModels.begin(); list_iter != mStaticModels.end(); ++list_iter) {
		list_iter->Render();
		}
	}