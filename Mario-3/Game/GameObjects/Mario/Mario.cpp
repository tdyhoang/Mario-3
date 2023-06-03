#include "Mario.h"

#include "../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"

CMario::CMario()
{
	LoadAnimation();
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("Run", animationManager->Clone("ani-super-mario-run"));

	SetState("Run");
}