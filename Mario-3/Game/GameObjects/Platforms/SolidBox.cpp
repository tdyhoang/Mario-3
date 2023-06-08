#include "SolidBox.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "../../../Framework/GameComponents/Graphics/Texture/TextureManager.h"
#include "../../../Framework/GameComponents/Const.h"
#include "../../../Framework/GameComponents/Game.h"

CSolidBox::CSolidBox()
{
	Init();
}

void CSolidBox::Init()
{
	CHitBox* box = new CHitBox();
	box->SetGameObjectAttach(this);
	hitBoxs->push_back(box);
	physicsBody->SetDynamic(false);
	SetTag(ObjectTag::Solid);
}

bool CSolidBox::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	return true;
}

CSolidBox::~CSolidBox()
{
	CGameObject::~CGameObject();
}