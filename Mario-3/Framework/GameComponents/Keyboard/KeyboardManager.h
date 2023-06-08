#pragma once
#include "KeyEventHandler.h"
#include "../Const.h"
#include <dinput.h>

class CKeyboardManager;
typedef CKeyboardManager* LPKeyboardManager;

class CKeyboardManager
{
private:
	static LPKeyboardManager instance;

	LPDIRECTINPUT8 dInput;
	LPDIRECTINPUTDEVICE8 dInputDevice;

	LPKeyEventHandler keyEventHandler;

	BYTE  keyStates[256];
	DIDEVICEOBJECTDATA keyEvents[1024];
	HWND hWnd;

public:
	~CKeyboardManager();

	static LPKeyboardManager GetInstance();

	void InitKeyboard(LPKeyEventHandler handler);

	void ProcessKeyboard();
	bool CheckESCKey();

	bool GetKeyStateDown(int keyCode);
	bool GetKeyStateUp(int keyCode);

	void SetHWND(HWND h);

	LPKeyEventHandler GetKeyEventHandler() { return keyEventHandler; }
};