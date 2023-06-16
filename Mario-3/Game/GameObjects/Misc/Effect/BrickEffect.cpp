#include "BrickEffect.h"
#include "../../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../../../../Framework/GameComponents/Game.h"
#include "../Misc.h"

CBrickEffect::CBrickEffect()
{
	LoadAnimation();
	SetState("BRICK");
	SetScale(D3DXVECTOR2(0.70f, 0.70f));
	physicsBody->SetGravity(DEBRIS_GRAVITY);
	isEnabled = false;
}

void CBrickEffect::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("BRICK", animations->Get("ani-brick-debris"));
}

void CBrickEffect::LinkToPool(CObjectPool* pool)
{
	this->pool = pool;
}

void CBrickEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto deltaTime = dt * CGame::GetTimeScale();
	auto vel = physicsBody->GetVelocity();

	auto pos = GetPosition();
	SetPosition(pos + vel * deltaTime);

	vel.y += physicsBody->GetGravity() * deltaTime;
	vel.y = (vel.y < MUSHROOM_VEL) ? vel.y : MUSHROOM_VEL;
	physicsBody->SetVelocity(vel);

	if (transform.position.y > startPosition.y && *physicsBody->GetVelocity() > 0)
		isEnabled = false;
}