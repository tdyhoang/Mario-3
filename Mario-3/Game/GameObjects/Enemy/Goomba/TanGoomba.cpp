#include "TanGoomba.h"
#include "../../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"

CTanGoomba::CTanGoomba()
{
	LoadAnimation();
}

void CTanGoomba::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("WALK", animationManager->Clone("ani-goomba-walk"));
	AddAnimation("DIE", animationManager->Clone("ani-goomba-die"));
	AddAnimation("IDLE", animationManager->Clone("ani-goomba-idle"));
	SetState("WALK");
}