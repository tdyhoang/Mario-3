#pragma once

#include "../../../Framework/GameComponents/GameObject/GameObject.h"

class CBlock : public CGameObject
{
public:
	CBlock();
	virtual void Init();
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;
};