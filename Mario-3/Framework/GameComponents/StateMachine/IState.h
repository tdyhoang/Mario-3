#pragma once

class IState
{
public:
	virtual void Access() = 0;
	virtual void Process() = 0;
	virtual void Exit() = 0;
};