#pragma once

#include "../../../Framework/GameComponents/GameObject/GameObject.h"

class CSolidBox : public CGameObject
{
public:
	CSolidBox();
	void Init();
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;
	~CSolidBox();
};