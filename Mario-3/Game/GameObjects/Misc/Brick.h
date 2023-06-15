#pragma once

#include "../../../Framework/GameComponents/GameObject/GameObject.h"
#include "../ItemInfo.h"

class CBrick : public CGameObject
{
private:
	ItemInfo itemInfo;
	int type;
	CObjectPool brickPool;
	int bounceState;
	float bounceDelta;
	int startBounceTime;
	CGameObject* target;
public:
	CBrick();
	void SetTarget(CGameObject* target);
	void SetItemInfo(ItemInfo info);
	ItemInfo GetItemInfo();
	void LoadAnimation();
	void Init();
	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255);
	void OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox) override;
	void OnCollisionEnter(CHitBox* selfHitBox, std::vector<CollisionEvent*> collisionEvents) override;
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;
	void SetType(int type);
	int GetType();
	void Debris();
	void Bounce();
	CObjectPool GetObjectPool();
};