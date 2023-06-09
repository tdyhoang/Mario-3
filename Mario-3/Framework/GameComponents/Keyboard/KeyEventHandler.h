#pragma once

#include "../GameObject/GameObject.h"
#include <vector>

class CKeyEventHandler;
typedef CKeyEventHandler* LPKeyEventHandler;
class CKeyEventHandler
{
public:
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	virtual void KeyState();
};