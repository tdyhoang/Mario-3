#include "Gift.h"
#include "../../../../Framework/Ultis/Ultis.h"
#include "../../Mario/Mario.h"

void CGift::OnCollisionEnter(CHitBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (MarioTag(collisionBox->GetGameObjectAttach()->GetTag()))
			PowerUp(collisionBox);
		if (powerupTag == PowerupTag::SuperMushroom && StaticTag(collisionBox->GetGameObjectAttach()->GetTag()))
			if (collisionEvent->nx != 0)
			{
				auto normal = physicsBody->GetNormal();
				normal.x = -normal.x;
				physicsBody->SetNormal(normal);
			}
	}
}

void CGift::OnOverlappedEnter(CHitBox* selfCollisionBox, CHitBox* otherCollisionBox)
{
	if (MarioTag(otherCollisionBox->GetGameObjectAttach()->GetTag()))
	{
		PowerUp(otherCollisionBox);
	}
	if (powerupTag == PowerupTag::SuperMushroom && StaticTag(otherCollisionBox->GetGameObjectAttach()->GetTag()))
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

void CGift::PowerUp(CHitBox* otherCollisionBox)
{
	isEnabled = false;
	if (physicsBody->IsDynamic() == true)
	{
		GetPhysicsBody()->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
		GetPhysicsBody()->SetGravity(0.0f);
	}
	hitBoxs->at(0)->SetEnable(false);

	auto gO = otherCollisionBox->GetGameObjectAttach();
	auto mario = static_cast<CMario*>(gO);

	mario->SetPowerUp(true);
	mario->OnDamaged();
	mario->SetPowerUpItem(powerupTag);
}