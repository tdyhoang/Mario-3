#include "Gift.h"
#include "../../../../Framework/Ultis/Ultis.h"
#include "../../Mario/Mario.h"

void CGift::OnCollisionEnter(CHitBox* selfHitBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto hitBox = collisionEvent->obj;
		if (MarioTag(hitBox->GetGameObjectAttach()->GetTag()))
			PowerUp(hitBox);
		if (powerupTag == PowerupTag::SuperMushroom && StaticTag(hitBox->GetGameObjectAttach()->GetTag()))
			if (collisionEvent->nx != 0)
			{
				auto normal = physicsBody->GetNormal();
				normal.x = -normal.x;
				physicsBody->SetNormal(normal);
			}
	}
}

void CGift::OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox)
{
	if (MarioTag(otherHitBox->GetGameObjectAttach()->GetTag()))
	{
		PowerUp(otherHitBox);
	}
	if (powerupTag == PowerupTag::SuperMushroom && StaticTag(otherHitBox->GetGameObjectAttach()->GetTag()))
	{
		auto normal = physicsBody->GetNormal();
		normal.x = -normal.x;
		physicsBody->SetNormal(normal);
	}
}

bool CGift::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	return MarioTag(tag);
}

void CGift::PowerUp(CHitBox* otherHitBox)
{
	isEnabled = false;
	if (physicsBody->IsDynamic() == true)
	{
		GetPhysicsBody()->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
		GetPhysicsBody()->SetGravity(0.0f);
	}
	hitBoxs->at(0)->SetEnable(false);

	auto gO = otherHitBox->GetGameObjectAttach();
	auto mario = static_cast<CMario*>(gO);

	mario->SetPowerUp(true);
	mario->OnDamaged();
	mario->SetPowerUpItem(powerupTag);
}