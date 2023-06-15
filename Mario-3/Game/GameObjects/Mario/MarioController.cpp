#include "MarioController.h"
#include "../../../Framework/GameComponents/Game.h"
#include "Mario.h"
#include "States/SmallMario.h"
#include "States/SuperMario.h"
#include "../../../Framework/GameComponents/GameObject/ObjectTag.h"
#include "MarioConst.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"
#include <unordered_map>
#include "../../../Framework/Ultis/Ultis.h"

using namespace std;

CMarioController::CMarioController()
{
	Init();
	currentStateObject = NULL;
	SwitchToState("SmallMario");
}
void CMarioController::Init()
{
	SetTag(ObjectTag::PlayerController);
	isEnabled = true;
	CMario* marioStateObject;

	marioStateObject = new CSmallMario();
	listMarioStates.insert(make_pair("SmallMario", marioStateObject));
	listStateObjects.insert(make_pair("SmallMario", marioStateObject));
	marioStateObject->Enable(false);

	marioStateObject = new CSuperMario();
	listMarioStates.insert(make_pair("SuperMario", marioStateObject));
	listStateObjects.insert(make_pair("SuperMario", marioStateObject));
	marioStateObject->Enable(false);

	hitBoxs->clear();

	physicsBody->SetDynamic(false);
}

void CMarioController::Process()
{
	CStateMachine::Process();
}

void CMarioController::AddStateObjectsToScene(LPScene scene)
{
	DebugOut(L"Add mario to scene \n");
	for (auto& obj : listStateObjects)
	{
		scene->AddObject(obj.second);
		scene->AddKeyboardTargetObject(obj.second);
		obj.second->AddObjectToScene(scene);
	}
}

void CMarioController::SwitchToState(std::string state)
{
	SwitchState(listMarioStates.at(state));

	if (currentStateObject != NULL)
	{
		auto controllerPhyBody = currentStateObject->GetPhysicsBody();
		auto currentObj = listStateObjects.at(state);
		auto currentPhyBody = currentObj->GetPhysicsBody();

		currentPhyBody->SetVelocity(controllerPhyBody->GetVelocity());
		currentPhyBody->SetGravity(controllerPhyBody->GetGravity());
		currentPhyBody->SetAcceleration(controllerPhyBody->GetAcceleration());

		listStateObjects.at(state)->SetPosition(currentStateObject->GetPosition());

		D3DXVECTOR2 transform = D3DXVECTOR2(0.0f, 0.0f);
		transform.y = SUPER_MARIO_BBOX.y - SMALL_MARIO_BBOX.y;

		if (listStateObjects.at(state)->GetTag() != ObjectTag::SmallPlayer)
		{
			listStateObjects.at(state)->SetPosition(listStateObjects.at(state)->GetPosition() - transform);
			listStateObjects.at(state)->GetHitBox()->at(0)->SetPosition(transform);
			listStateObjects.at(state)->SetRelativePositionOnScreen(listStateObjects.at(state)->GetHitBox()->at(0)->GetPosition());
		}

		auto stateMarioToBeChanged = static_cast<CMario*>(listStateObjects.at(state));
		auto currentMario = static_cast<CMario*>(currentStateObject);
#pragma region PassParamater For DamageProcess
		stateMarioToBeChanged->SetDamageFlag(currentMario->GetDamageFlag());
		stateMarioToBeChanged->SetChangeSmokeEffectFlag(currentMario->GetChangeSmokeEffectFlag());
		stateMarioToBeChanged->SetChangeLevelFlag(currentMario->GetChangeLevelFlag());

		stateMarioToBeChanged->SetCountChangeAlpha(currentMario->GetCountChangeAlpha());
		stateMarioToBeChanged->SetCountSmokeEffectActivate(currentMario->GetCountSmokeEffectActivate());

		stateMarioToBeChanged->SetTimeStartDamaged(currentMario->GetTimeStartDamaged());
		stateMarioToBeChanged->SetTimeStartSmokeEffect(currentMario->GetTimeStartSmokeEffect());
		stateMarioToBeChanged->SetTimeStartChangeLevel(currentMario->GetTimeStartChangeLevel());
		stateMarioToBeChanged->SetTimeStartChangeAlpha(currentMario->GetTimeStartChangeAlpha());
		stateMarioToBeChanged->SetPowerUp(currentMario->IsPowerUp());
#pragma endregion

#pragma region PassParamater For P-meter
		stateMarioToBeChanged->SetPMeterCounting(currentMario->GetPMeterCounting());
		stateMarioToBeChanged->SetFeverState(currentMario->GetFeverState());
#pragma endregion
	}

	if (listStateObjects.size() != 0)
	{
		currentStateObject = listStateObjects.at(state);
		auto scene = CSceneManager::GetInstance()->GetActiveScene();
		if (scene != NULL)
		{
			auto camera = scene->GetCamera();
			if (camera != NULL)
				camera->SetGameObject(currentStateObject);
		}
	}

	CGame::SetTimeScale(1.0f);
}

void CMarioController::OnKeyDown(int KeyCode)
{
	// For debugging
	switch (KeyCode) {
	case DIK_1: {SwitchToState("SmallMario"); break; }
	case DIK_2: {SwitchToState("SuperMario"); break; }
	/*case DIK_3: {SwitchToState("RaccoonMario"); break; }
	case DIK_4: {SwitchToState("FireMario"); break; }*/
	}
}

void CMarioController::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	Process();
	if (currentStateObject == NULL)
		return;
	this->transform.position = currentStateObject->GetPosition();
	this->physicsBody->SetVelocity(currentStateObject->GetPhysicsBody()->GetVelocity());
}

void CMarioController::SetCurrentStateObject(LPGameObject gO)
{
	this->currentStateObject = gO;
}

LPGameObject CMarioController::GetCurrentStateObject()
{
	return currentStateObject;
}