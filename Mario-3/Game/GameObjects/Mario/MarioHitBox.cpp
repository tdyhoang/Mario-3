#include "MarioHitBox.h"
#include "MarioConst.h"
#include "../../../Framework/Ultis/Ultis.h"

void CMarioHitBox::CollisionHandle(DWORD dt, std::vector<CollisionEvent*>& collisionEvents, LPPhysicsBody phyBody, D3DXVECTOR2 vel, int mintx, int minty, float nx, float ny)
{
	CHitBox::CollisionHandle(dt, collisionEvents, phyBody, vel, mintx, minty, nx, ny);

	for (auto collisionEvent : collisionEvents)
	{
		auto hitBox = collisionEvent->obj;
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Enemy)
		{
			if (collisionEvent->ny < 0)
			{
				if (mario->StopBounce() == false)
					mario->JumpProcess(-0.4, true);

				auto otherObject = hitBox->GetGameObjectAttach();
			}
			if (collisionEvent->ny > 0)
				mario->OnDamaged();
			if (collisionEvent->nx != 0)
				auto otherObject = hitBox->GetGameObjectAttach();
		}
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::VenusFireBall)
		{
			mario->OnDamaged();
			hitBox->GetGameObjectAttach()->Enable(false);
		}
	}
}

void CMarioHitBox::SetGameObjectAttach(LPGameObject gO)
{
	CHitBox::SetGameObjectAttach(gO);
	mario = dynamic_cast<CMario*>(gO);
}