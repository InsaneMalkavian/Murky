#pragma once
#include "StateBase.h"
class StateStart : public StateBase
	{
	public:
		StateStart(void);
		virtual ~StateStart(void);
		void Create();
		void Destroy();
		void SetState();
	};

