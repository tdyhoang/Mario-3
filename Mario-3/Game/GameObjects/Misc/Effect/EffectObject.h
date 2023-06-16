#pragma once

#include "../../../../Framework/GameComponents/GameObject/GameObject.h"

class CEffectObject : public CGameObject
{
protected:
	D3DXVECTOR2 startPosition;

public:
	CEffectObject();
	void SetStartPosition(D3DXVECTOR2 pos);
	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;
};