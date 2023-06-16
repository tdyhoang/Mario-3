#pragma once

#include "EffectObject.h"
#include "../../ItemInfo.h"

class CGift : public CEffectObject
{
protected:
	ItemTag itemTag;
	PowerupTag powerupTag;
public:
	void OnCollisionEnter(CHitBox* selfHitBox, std::vector<CollisionEvent*> otherCollisions);
	void OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox) override;
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;
	void PowerUp(CHitBox* otherHitBox);
	PowerupTag GetPowerupTag() { return powerupTag; }
	void SetPowerupTag(PowerupTag pT) { this->powerupTag = pT; }
};