#include "MushroomEffect.h"
#include "../../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../Misc.h"
#include "../../../../Framework/Ultis/Ultis.h"

CMushroomEffect::CMushroomEffect()
{
	LoadAnimation();
	SetState("SUPER-MUSHROOM");
	SetScale(D3DXVECTOR2(0.4f, 1.0f));
	SetTag(ObjectTag::SuperMushroom);
	itemTag = ItemTag::PowerUp;
	powerupTag = PowerupTag::SuperMushroom;
	isEnabled = true;

	CHitBox* box = new CHitBox();
	box->SetSizeBox(MUSHROOM_BBOX);
	box->SetGameObjectAttach(this);
	box->SetEnable(true);
	hitBoxs->push_back(box);
	physicsBody->SetGravity(MUSHROOM_GRAVITY);
	physicsBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	physicsBody->SetDynamic(true);

	appearingCounting = 0;
}

void CMushroomEffect::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("SUPER-MUSHROOM", animations->Get("ani-super-mushroom"));
}

void CMushroomEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto velocity = physicsBody->GetVelocity();
	auto normal = physicsBody->GetNormal();
	if (isAppearing == true)
	{
		velocity.y = -MUSHROOM_PUSH_FORCE;
		if (abs(startPosition.y) - abs(transform.position.y) > GROW_MAX_HEIGHT)
			isAppearing = false;
	}
	else
	{
		appearingCounting++;
		if (appearingCounting == 1)
			normal.x = marioFacing;
		velocity.x = normal.x * MUSHROOM_SPEED;
	}
	physicsBody->SetVelocity(velocity);
}

void CMushroomEffect::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CMushroomEffect::StartEffect(float MarioFacing)
{
	isAppearing = true;
	this->marioFacing = MarioFacing;
}

bool CMushroomEffect::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	if (isAppearing == true && (StaticTag(tag) == true || tag == ObjectTag::GhostPlatform))
		return false;
	if (StaticTag(tag) == true || MarioTag(tag) == true || tag == ObjectTag::GhostPlatform)
		return true;
	return false;
}