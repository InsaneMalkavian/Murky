#include "StateManager.h"
#include <Windows.h>

StateManager::StateManager(void): state(NULL) {
	state = new StateStart();
	}

StateManager::~StateManager(void) {
	delete state;
	state = NULL;
	}
