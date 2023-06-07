#pragma once

#include "IState.h"

class CStateMachine
{
protected:
	IState* currentState;
public:
	virtual void SwitchState(IState* state);
	virtual void Process();
};