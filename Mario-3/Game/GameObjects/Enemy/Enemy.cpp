#include "Enemy.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"
#include "../Misc/QuestionBlock.h"
#include "../Misc/Brick.h"

CEnemy::CEnemy()
{
	this->SetTag(ObjectTag::Enemy);
	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
	countDeadCallback = 0;
	countBeated = 0;
	isHeadShot = false;
	isHeadShotByFireBall = false;
	hitFX = new CHitEffect();
	isStatic = false;
}

void CEnemy::SetEnemyType(std::string eT)
{
	enemyType = eT;
}

std::string CEnemy::GetEnemyType()
{
	return enemyType;
}

void CEnemy::SetEnemyTag(EnemyTag eT)
{
	enemyTag = eT;
}

EnemyTag CEnemy::GetEnemyTag()
{
	return enemyTag;
}

void CEnemy::SetStartPosition(D3DXVECTOR2 sT)
{
	this->startPosition = sT;
}

D3DXVECTOR2 CEnemy::GetStartPosition()
{
	return startPosition;
}

void CEnemy::OnCollisionEnter(CHitBox* selfHitBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto hitBox = collisionEvent->obj;
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Solid || hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::GhostPlatform
			|| hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::QuestionBlock || hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Brick
			|| hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::SwitchBlock || hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Pipe)
		{
			if (collisionEvent->ny < 0 && isOnGround == false)
				isOnGround = true;
			if (collisionEvent->nx != 0)
			{
				auto normal = physicsBody->GetNormal();
				normal.x = -normal.x;
				physicsBody->SetNormal(normal);
				if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::QuestionBlock && enemyTag == EnemyTag::KoopaShell)
				{
					auto questionBlock = static_cast<CQuestionBlock*>(hitBox->GetGameObjectAttach());
					questionBlock->Bounce();
				}
				if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Brick && enemyTag == EnemyTag::KoopaShell)
				{
					auto brick = static_cast<CBrick*>(hitBox->GetGameObjectAttach());
					brick->Bounce();
					brick->Debris();
				}
			}
		}
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Enemy)
		{
			auto otherObject = hitBox->GetGameObjectAttach();
			auto otherEnemyObject = static_cast<CEnemy*>(otherObject);
			if (collisionEvent->nx != 0)
				if (otherEnemyObject->GetEnemyTag() == this->enemyTag ||
					(this->enemyTag != EnemyTag::KoopaShell && otherEnemyObject->GetEnemyTag() == EnemyTag::KoopaShell))
				{
					auto normal = physicsBody->GetNormal();
					auto velocity = physicsBody->GetVelocity();
					normal.x = -normal.x;
					velocity.x *= normal.x;
					transform.position.x = velocity.x * dt;
					physicsBody->SetNormal(normal);
					physicsBody->SetVelocity(velocity);
				}
		}
		else if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::MarioFireBall)
		{
			if (collisionEvent->nx != 0 || collisionEvent->ny != 0)
			{
				isHeadShot = true;

				if (enemyTag == EnemyTag::Koopa || enemyTag == EnemyTag::ParaKoopa)
					this->OnDamaged(hitBox->GetGameObjectAttach());
			}
			else
				OnDie();
		}
	}
}

void CEnemy::OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox)
{
	if (otherHitBox->GetGameObjectAttach()->GetTag() == ObjectTag::RaccoonTail || otherHitBox->GetGameObjectAttach()->GetTag() == ObjectTag::MarioFireBall)
	{
		isHeadShot = true;

		if (enemyTag == EnemyTag::Koopa || enemyTag == EnemyTag::ParaKoopa)
			this->OnDamaged(otherHitBox->GetGameObjectAttach());
	}
	else
		OnDie();
}

bool CEnemy::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	if (tag == ObjectTag::VenusFireBall || tag == ObjectTag::Gift)
		return false;
	if (MarioTag(tag) == true)
		return false;
	if (GiftTag(tag) == true || tag == ObjectTag::Coin)
		return false;
	if (tag == ObjectTag::Enemy)
	{
		auto enemy = static_cast<CEnemy*>(gO);
		if (enemyTag != EnemyTag::KoopaShell && enemy->GetEnemyTag() != EnemyTag::KoopaShell)
		{
			if (enemyTag != enemy->GetEnemyTag())
				return false;
		}
		if (enemyTag == EnemyTag::Piranha || enemyTag == EnemyTag::Venus)
		{
			if (StaticTag(tag) == true || tag == ObjectTag::GhostPlatform)
				return false;
		}
	}
	return true;
}

void CEnemy::SetHitFX(CHitEffect* hitFX)
{
	this->hitFX = hitFX;
}

void CEnemy::SetIsHeadShot(bool isHeadShot)
{
	this->isHeadShot = isHeadShot;
}

void CEnemy::SetIsHeadShotByFireball(bool isHeadShotByFireball)
{
	this->isHeadShotByFireBall = isHeadShotByFireball;
}

void CEnemy::SetTarget(CGameObject* target)
{
	if (target == NULL)
		return;
	this->target = target;
}

void CEnemy::CollideUnderMario()
{
	if (enemyTag == EnemyTag::Piranha || enemyTag == EnemyTag::Venus)
		return;
	countBeated++;
}
