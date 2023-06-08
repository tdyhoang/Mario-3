#pragma once

#include "Block.h"
#include "../ItemInfo.h"

class CQuestionBlock : public CBlock
{
private:
	ItemInfo itemInfo;
	int bounceState;
	float bounceDelta;
	DWORD startBounceTime;
	CGameObject* target;
public:
	CQuestionBlock();
	void SetTarget(CGameObject* target);
	void LoadAnimation() override;
	void SetItemInfo(ItemInfo info);
	void Bounce();
	virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	virtual void Render(CCamera* cam, int alpha = 255);
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;
	void OnOverlappedEnter(CHitBox* selfCollisionBox, CHitBox* otherCollisionBox) override;
};