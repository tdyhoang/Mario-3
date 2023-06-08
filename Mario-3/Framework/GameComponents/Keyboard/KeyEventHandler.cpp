#include <dinput.h>
#include "KeyEventHandler.h"
#include "../../Ultis/Ultis.h"
#include "../Scene/SceneManager.h"

using namespace std;

void CKeyEventHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene == NULL)
		return;
	auto currentTargets = activeScene->GetKeyboardTargetObject();
	for (auto gameObject : currentTargets)
		if (gameObject != NULL && gameObject->IsEnabled() == true)
			gameObject->OnKeyDown(KeyCode);
}

void CKeyEventHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene == NULL)
		return;
	auto currentTargets = activeScene->GetKeyboardTargetObject();
	for (auto gameObject : currentTargets)
		if (gameObject != NULL && gameObject->IsEnabled() == true)
			gameObject->OnKeyUp(KeyCode);
}

void CKeyEventHandler::KeyState()
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene == NULL)
		return;
	auto currentTargets = activeScene->GetKeyboardTargetObject();
	for (auto gameObject : currentTargets)
		if (gameObject != NULL && gameObject->IsEnabled() == true)
			gameObject->KeyState();
}