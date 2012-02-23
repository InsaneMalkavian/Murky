#include "StateStart.h"

StateStart::StateStart(void) {
	Log::logger.AddLine("Start");
	}

StateStart::~StateStart(void) {
	Log::logger.AddLine("~Start");
	}

void StateStart::Create() {
	}

void StateStart::Destroy() {
	}

void StateStart::SetState() {
	}