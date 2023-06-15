#include "EmptyBlock.h"
#include "../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"

CEmptyBlock::CEmptyBlock()
{
	LoadAnimation();
	SetState("EMPTY");
	isEnabled = true;
}

void CEmptyBlock::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("EMPTY", animations->Get("ani-empty-block"));
}