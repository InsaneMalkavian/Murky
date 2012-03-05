#pragma once
#include "States.h"
class StateManager
	{
	public:
		StateManager(void);
		~StateManager(void);
	private:
		StateBase* mState;
	};

