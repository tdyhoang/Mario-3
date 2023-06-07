#pragma once

#include "../../../Framework/GameComponents/StateMachine/StateMachine.h"
#include "../../../Framework/GameComponents/StateMachine/IState.h"
#include "../../../Framework/GameComponents/GameObject/GameObject.h"
#include "../../../Framework/GameComponents/Scene/Scene.h"

class CGameObject;
typedef CGameObject* LPGameObject;

class CScene;
typedef CScene* LPScene;

class CMarioController : public CStateMachine, public CGameObject
{
private:
	std::unordered_map<std::string, IState*> listMarioStates;
	std::unordered_map<std::string, LPGameObject> listStateObjects;
	LPGameObject currentStateObject;
public:
	CMarioController();
	void Init() override;
	void Process() override;
	void AddStateObjectsToScene(LPScene scene);
	void SwitchToState(std::string state);
	void OnKeyDown(int KeyCode) override;
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;

	void SetCurrentStateObject(LPGameObject gO);
	LPGameObject GetCurrentStateObject();
};