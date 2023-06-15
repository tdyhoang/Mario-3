#include "Brick.h"
#include "../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "Misc.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "../../../Framework/GameComponents/Game.h"
#include "../Mario/Mario.h"
#include "../Mario/MarioController.h"

CBrick::CBrick()
{
	LoadAnimation();
	Init();
	SetState("IDLE");
	isEnabled = true;
	startBounceTime = 0;
	bounceState = 0;
	bounceDelta = 0.0f;
	itemInfo.tag = ItemTag::None;
	itemInfo.quantity = 0;
}

void CBrick::SetTarget(CGameObject* target)
{
	this->target = target;
}

void CBrick::SetItemInfo(ItemInfo info)
{
	this->itemInfo = info;
}

ItemInfo CBrick::GetItemInfo()
{
	return itemInfo;
}

void CBrick::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("IDLE", animations->Get("ani-brick"));
	AddAnimation("EMPTY", animations->Get("ani-empty-block"));
}

void CBrick::Init()
{
	tag = ObjectTag::Brick;
	CHitBox* box = new CHitBox();
	box->SetSizeBox(BRICK_BBOX);
	box->SetGameObjectAttach(this);
	hitBoxs->push_back(box);
	physicsBody->SetDynamic(true);
}

void CBrick::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (bounceState == 2)
	{
		bounceDelta += 0.48 * dt;

		if (GetTickCount64() - startBounceTime > 50 && startBounceTime != 0)
		{
			bounceDelta = 0.0f;
			startBounceTime = 0;
			bounceState = 0;
		}
	}

	if (bounceState == 1)
	{
		bounceDelta += -0.48 * dt;

		if (GetTickCount64() - startBounceTime > 50 && startBounceTime != 0)
		{
			bounceState = 2;
			startBounceTime = GetTickCount64();
		}
	}
}

void CBrick::Render(CCamera* cam, int alpha)
{
	relativePositionOnScreen.y = bounceDelta;
	CGameObject::Render(cam, alpha);
}

void CBrick::OnOverlappedEnter(CHitBox* selfCollisionBox, CHitBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == ObjectTag::RaccoonTail)
	{
		Bounce();
		Debris();
	}
}

void CBrick::OnCollisionEnter(CHitBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
}

bool CBrick::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	return false;
}

void CBrick::SetType(int type)
{
	this->type = type;
}

int CBrick::GetType()
{
	return type;
}

void CBrick::Debris()
{
	if (itemInfo.tag != ItemTag::None)
		return;
	const float velx[4] = { +0.1875f, +0.25f, -0.25f, -0.1875f };
	const float vely[4] = { -0.375f, -0.75f, -0.75f, -0.375f };

	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();

	for (int i = 0; i < 4; i++)
	{
		auto brickFX = brickPool.Init();
		if (brickFX != NULL)
		{
			auto pos = brickFX->GetPosition();
			brickFX->GetPhysicsBody()->SetVelocity(D3DXVECTOR2(velx[i], vely[i]));
			brickFX->Enable(true);
			brickFX->SetPosition(GetPosition());

			auto grid = activeScene->GetGrid();
			if (grid != NULL && activeScene->IsSpacePartitioning() == true)
				grid->Move(pos, brickFX);
		}
	}
	isEnabled = false;
	activeScene->RemoveBrick(this);
	activeScene->RemoveObject(this);
	activeScene->AddDestroyObject(this);
}

void CBrick::Bounce()
{
	if (bounceState != 0)
		return;
	startBounceTime = GetTickCount64();
	bounceState = 1;

	if (itemInfo.quantity > 0)
	{
		itemInfo.quantity--;
		if (itemInfo.quantity <= 0)
		{
			if (itemInfo.quantity < 0)
				itemInfo.quantity = 0;
			if (currentState != "EMPTY")
				SetState("EMPTY");
		}
	}
}

CObjectPool CBrick::GetObjectPool()
{
	return brickPool;
}