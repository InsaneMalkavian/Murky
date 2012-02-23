#pragma once

#include "Logger.h"
class StateBase {
public:
	StateBase(void) {Log::logger.AddLine("Base");};
	virtual ~StateBase(void) {Log::logger.AddLine("~Base");}
	virtual void Create() = 0;
	virtual void Destroy() = 0;
	virtual void SetState() = 0;
	};

