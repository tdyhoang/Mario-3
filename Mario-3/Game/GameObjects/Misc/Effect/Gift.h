#pragma once

#include "EffectObject.h"
#include "../../ItemInfo.h"

class CGift : public CEffectObject
{
protected:
	ItemTag itemTag;
	PowerupTag powerupTag;
public:
	CGift();
	void OnCollisionEnter(CHitBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	void OnTriggerEnter(CHitBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions);
	void OnOverlappedEnter(CHitBox* selfCollisionBox, CHitBox* otherCollisionBox) override;
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;
	void PowerUp(CHitBox* otherCollisionBox);

	PowerupTag GetPowerupTag() { return powerupTag; }
	void SetPowerupTag(PowerupTag pT) { this->powerupTag = pT; }
};