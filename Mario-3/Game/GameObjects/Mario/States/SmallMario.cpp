#include "SmallMario.h"
#include "../MarioConst.h"
#include "../../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../../../../Framework/Ultis/Ultis.h"
#include "../MarioHitBox.h"

CSmallMario::CSmallMario()
{
	CSmallMario::Init();
	CMario::InitProperties();
	CSmallMario::SetState("IDLE");

	tag = ObjectTag::SmallPlayer;
	marioStateTag = MarioStates::SmallMario;

	canSquat = false;
	canAttack = false;
}
void CSmallMario::Init()
{
	LoadAnimation();
	CMarioHitBox* hitBox = new CMarioHitBox();
	hitBox->SetSizeBox(SMALL_MARIO_BBOX);
	hitBox->SetGameObjectAttach(this);
	hitBox->SetName("Small-Mario");
	hitBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	SetRelativePositionOnScreen(D3DXVECTOR2(0.0f, SUPER_MARIO_BBOX.y / 2 - SMALL_MARIO_BBOX.y / 2));
	hitBoxs->push_back(hitBox);
}

void CSmallMario::LoadAnimation()
{
	CMario::LoadAnimation();

	DebugOut(L"Load animation Small Mario \n");
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("IDLE", animationManager->Get("ani-small-mario-idle"));
	AddAnimation("WALK", animationManager->Get("ani-small-mario-walk"));
	AddAnimation("RUN", animationManager->Get("ani-small-mario-run"));
	AddAnimation("JUMP", animationManager->Get("ani-small-mario-jump"));
	AddAnimation("FALL", animationManager->Get("ani-small-mario-jump"));
	AddAnimation("SQUAT", animationManager->Get("ani-small-mario-idle"));
	AddAnimation("HIGH-JUMP", animationManager->Get("ani-small-mario-high-jump"));
	AddAnimation("PICK-UP-WALK", animationManager->Get("ani-small-mario-pick-up-walk"));
	AddAnimation("PICK-UP-IDLE", animationManager->Get("ani-small-mario-pick-up-idle"));
	AddAnimation("PICK-UP-JUMP", animationManager->Get("ani-small-mario-pick-up-jump"));
	AddAnimation("KICK", animationManager->Get("ani-small-mario-kick"), false);
	AddAnimation("IDLE-FRONT", animationManager->Get("ani-small-mario-idle-front"));
	AddAnimation("DIE", animationManager->Get("ani-small-mario-die"));
}

CSmallMario::~CSmallMario()
{
	CGameObject::~CGameObject();
}