#pragma once
#include <list>
#include "ModelStaticObject.h"

class SceneManager
	{
	public:
		SceneManager(void);
		~SceneManager(void);
		void Render(void);
	private:
		std::list<ModelStaticObject>		mStaticModels;
	};

