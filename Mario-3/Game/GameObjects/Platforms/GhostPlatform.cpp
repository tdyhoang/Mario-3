#include "GhostPlatform.h"

CGhostPlatform::CGhostPlatform()
{
	Init();
}

void CGhostPlatform::Init()
{
	CHitBox* box = new CHitBox();
	box->SetGameObjectAttach(this);
	this->hitBoxs->push_back(box);
	this->physicsBody->SetDynamic(false);
	this->SetTag(ObjectTag::GhostPlatform);
	this->isEnabled = true;
}

bool CGhostPlatform::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	return true;
}

CGhostPlatform::~CGhostPlatform()
{
	CGameObject::~CGameObject();
}