#include "StateManager.h"
#include <Windows.h>

StateManager::StateManager(void): mState(NULL) {
	mState = new StateStart();
	}

StateManager::~StateManager(void) {
	delete mState;
	mState = NULL;
	}
