#pragma once
#include "States.h"
class StateManager
	{
	public:
		StateManager(void);
		~StateManager(void);
	private:
		int mPlaceholder;
		StateBase* state;
	};

