#include "SuperMario.h"
#include "../../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../../../../Framework/Ultis/Ultis.h"

CSuperMario::CSuperMario()
{
	CMario::Init();
	hitBoxs->at(0)->SetName("Super-Mario");
	marioStateTag = MarioStates::SuperMario;

	CSuperMario::LoadAnimation();
	CSuperMario::Init();
	CMario::InitProperties();
	canSquat = true;
	canAttack = false;
}
void CSuperMario::Init()
{
	this->SetState("IDLE");
}
void CSuperMario::LoadAnimation()
{
	CMario::LoadAnimation();

	DebugOut(L"Init Super Mario \n");
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("IDLE", animationManager->Get("ani-super-mario-idle"));
	AddAnimation("WALK", animationManager->Get("ani-super-mario-walk"));
	AddAnimation("RUN", animationManager->Get("ani-super-mario-walk"));
	AddAnimation("HIGH-SPEED", animationManager->Get("ani-super-mario-run"));
	AddAnimation("FLY", animationManager->Get("ani-super-mario-high-jump"));
	AddAnimation("JUMP", animationManager->Get("ani-super-mario-jump"));
	AddAnimation("SQUAT", animationManager->Get("ani-super-mario-squat"));
	AddAnimation("SKID", animationManager->Get("ani-super-mario-skid"));
	AddAnimation("FALL", animationManager->Get("ani-super-mario-jump"));
	AddAnimation("PICK-UP-WALK", animationManager->Get("ani-super-mario-pick-up-walk"));
	AddAnimation("PICK-UP-IDLE", animationManager->Get("ani-super-mario-pick-up-idle"));
	AddAnimation("PICK-UP-JUMP", animationManager->Get("ani-super-mario-pick-up-jump"));
	AddAnimation("KICK", animationManager->Get("ani-super-mario-kick"), false);
	AddAnimation("IDLE-FRONT", animationManager->Get("ani-super-mario-idle-front"));
}

CSuperMario::~CSuperMario()
{
	CGameObject::~CGameObject();

}