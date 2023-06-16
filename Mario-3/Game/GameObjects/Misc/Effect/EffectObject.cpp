#include "EffectObject.h"

CEffectObject::CEffectObject()
{
	tag = ObjectTag::Effect;
}

void CEffectObject::SetStartPosition(D3DXVECTOR2 pos)
{
	this->transform.position = pos;
	this->startPosition = pos;
}

bool CEffectObject::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	return false;
}