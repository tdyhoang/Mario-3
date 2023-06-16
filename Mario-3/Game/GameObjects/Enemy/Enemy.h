#pragma once
#include "../../../Framework/GameComponents/GameObject/GameObject.h"
#include "../Misc/Effect/HitEffect.h"

enum class EnemyType
{
	basic, red
};

enum class EnemyTag
{
	Goomba, ParaGoomba, Koopa, KoopaShell, ParaKoopa, Piranha, Venus, BoomerangBrother
};

class CEnemy : public CGameObject
{
protected:
	D3DXVECTOR2 startPosition;
	std::string enemyType;
	EnemyTag enemyTag;
	DWORD startDeadTime;
	bool isDead, isHeadShot, isHeadShotByFireBall;
	int countDeadCallback;
	CHitEffect* hitFX;
	bool isOnGround;
	CGameObject* target;
	int countBeated;
public:
	CEnemy();
	void SetEnemyType(std::string eT);
	std::string GetEnemyType();

	void SetEnemyTag(EnemyTag eT);
	EnemyTag GetEnemyTag();

	void SetStartPosition(D3DXVECTOR2 sT);
	D3DXVECTOR2 GetStartPosition();

	virtual void OnCollisionEnter(CHitBox* selfHitBox, std::vector<CollisionEvent*> collisionEvents) override;
	virtual void OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox) override;
	virtual void OnDamaged(CGameObject* otherGO) {}
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;

	void SetHitFX(CHitEffect* hitFX);
	void SetIsHeadShot(bool isHeadShot);
	void SetIsHeadShotByFireball(bool isHeadShotByFireball);

	void SetTarget(CGameObject* target);

	void CollideUnderMario();
};