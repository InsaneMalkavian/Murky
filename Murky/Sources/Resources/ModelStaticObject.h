#pragma once
#include "BaseObject.h"
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
		void Render(void);
	private:
		int								placeholder;
	};

