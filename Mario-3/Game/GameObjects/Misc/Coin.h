#pragma once

#include "../../../Framework/GameComponents/GameObject/GameObject.h"

class CCoin : public CGameObject
{
	int type;
public:
	CCoin();
	void LoadAnimation();
	void Init();
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;
	void OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox) override;
	void SetType(int type);
	int GetType();
};