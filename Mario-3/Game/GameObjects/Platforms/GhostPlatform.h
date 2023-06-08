#pragma once

#include "../../../Framework/GameComponents/GameObject/GameObject.h"

class CGhostPlatform : public CGameObject
{
public:
	CGhostPlatform();
	void Init() override;
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;
	~CGhostPlatform();
};