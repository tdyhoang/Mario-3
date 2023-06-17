#include "MarioHitBox.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "MarioConst.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/Goomba/Goomba.h"
#include "../Misc/Brick.h"

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
					mario->JumpProcess(-MARIO_DEFLECT_Y, true);

				auto otherObject = hitBox->GetGameObjectAttach();
				auto otherEnemyObject = static_cast<CEnemy*>(otherObject);
				otherEnemyObject->CollideUnderMario();
				switch (otherEnemyObject->GetEnemyTag())
				{
				case EnemyTag::Goomba:
				{
					otherEnemyObject->OnDie();
					break;
				}
				case EnemyTag::Venus: case EnemyTag::Piranha:
				{
					mario->OnDamaged();
					auto normal = mario->GetPhysicsBody()->GetNormal();
					mario->GetPhysicsBody()->SetVelocity(D3DXVECTOR2(MARIO_WALKING_SPEED * normal.x, 0.0f));
				}
				case EnemyTag::BoomerangBrother:
				{
					otherEnemyObject->OnDie();
				}
				}
			}
			if (collisionEvent->ny > 0)
				mario->OnDamaged();
			if (collisionEvent->nx != 0)
			{
				auto otherObject = hitBox->GetGameObjectAttach();
				auto otherEnemyObject = static_cast<CEnemy*>(otherObject);
				switch (otherEnemyObject->GetEnemyTag())
				{
				default:
				{
					mario->OnDamaged();
					break;
				}
				}
			}
		}
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::VenusFireBall)
		{
			mario->OnDamaged();
			hitBox->GetGameObjectAttach()->Enable(false);
		}
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Brick)
		{

			if (collisionEvent->ny > 0)
			{
				auto brick = static_cast<CBrick*>(hitBox->GetGameObjectAttach());
				if (brick->IsEnabled() == true)
				{
					brick->Bounce();
					if (mario->GettMarioStateTag() != MarioStates::SmallMario)
						brick->Debris();
				}
			}
		}
	}
}

void CMarioHitBox::SetGameObjectAttach(LPGameObject gO)
{
	CHitBox::SetGameObjectAttach(gO);
	mario = dynamic_cast<CMario*>(gO);
}