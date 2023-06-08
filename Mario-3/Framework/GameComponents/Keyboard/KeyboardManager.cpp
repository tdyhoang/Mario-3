#include "KeyboardManager.h"
#include "../../Ultis/Ultis.h"
#include "../Game.h"
#include "../Scene/SceneManager.h"
#include "comdef.h"

LPKeyboardManager CKeyboardManager::instance = NULL;

CKeyboardManager::~CKeyboardManager()
{
	if (dInput != NULL) dInput->Release();
	if (dInputDevice != NULL)
	{
		dInputDevice->Unacquire();
		dInputDevice->Release();
	}
}

LPKeyboardManager CKeyboardManager::GetInstance()
{
	if (instance == NULL) instance = new CKeyboardManager();
	return instance;
}

void CKeyboardManager::InitKeyboard(LPKeyEventHandler handler)
{
	HRESULT
		hr = DirectInput8Create
		(
			GetModuleHandle(NULL),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&dInput, NULL
		);
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DirectInput8 Create failed!\n");
		return;
	}

	hr = dInput->CreateDevice(GUID_SysKeyboard, &dInputDevice, NULL);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] CreateDevice failed!\n");
		return;
	}

	hr = dInputDevice->SetDataFormat(&c_dfDIKeyboard);

	hr = dInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipdw{};

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 1024;

	hr = dInputDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = dInputDevice->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
		return;
	}

	this->keyEventHandler = handler;

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
}

void CKeyboardManager::ProcessKeyboard()
{
	HRESULT hr;
	hr = dInputDevice->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		CGame::SetTimeScale(0.0f);
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = dInputDevice->Acquire();
			if (h == DI_OK)
			{
				CGame::SetTimeScale(1.0f);
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	if (keyEventHandler == nullptr) return;

	DWORD dwElements = 1024;
	hr = dInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyEventHandler->OnKeyDown(KeyCode);
		else
			keyEventHandler->OnKeyUp(KeyCode);
		keyEventHandler->KeyState();
	}

}

bool CKeyboardManager::CheckESCKey()
{
	if (this->GetKeyStateDown(DIK_ESCAPE))
	{
		DebugOutTitle(L"Nhan ESC");
		DebugOut(L"Nhan ESC \n");
		CGame::GetInstance()->End();
		PostQuitMessage(0);
		return true;
	}
	return false;
}

void CKeyboardManager::SetHWND(HWND h)
{
	hWnd = h;
}

bool CKeyboardManager::GetKeyStateDown(int keyCode)
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene->GetSceneId() == "intro")
		if (keyCode != DIK_Q && keyCode != DIK_W)
			return false;
	return (keyStates[keyCode] & 0x80) > 0;
}

bool CKeyboardManager::GetKeyStateUp(int keyCode)
{
	return (keyStates[keyCode] & 0x80) <= 0;
}