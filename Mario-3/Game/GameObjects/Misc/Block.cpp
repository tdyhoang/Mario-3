#include "Block.h"

const D3DXVECTOR2 BLOCK_BBOX(48, 48);

CBlock::CBlock()
{
	Init();
}

void CBlock::Init()
{
	tag = ObjectTag::Solid;
	CHitBox* box = new CHitBox();
	box->SetSizeBox(BLOCK_BBOX);
	box->SetGameObjectAttach(this);
	this->hitBoxs->push_back(box);
	this->physicsBody->SetDynamic(false);
}

bool CBlock::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	return false;
}