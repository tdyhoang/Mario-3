#include "Coin.h"
#include "../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "Misc.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"

CCoin::CCoin()
{
	LoadAnimation();
	Init();
	SetState("SPIN");
	isEnabled = true;
}

void CCoin::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("SPIN", animations->Get("ani-coin"));
}

void CCoin::Init()
{
	tag = ObjectTag::Coin;
	CHitBox* box = new CHitBox();
	box->SetSizeBox(COIN_BBOX);
	box->SetGameObjectAttach(this);
	hitBoxs->push_back(box);
	physicsBody->SetDynamic(true);
	physicsBody->SetGravity(0.0f);
}

bool CCoin::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	if (MarioTag(tag))
		return true;
	return false;
}

void CCoin::OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox)
{
	if (MarioTag(otherHitBox->GetGameObjectAttach()->GetTag()))
	{
		this->isEnabled = false;
		auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
		if (activeScene != NULL)
		{
			activeScene->RemoveCoin(this);
			activeScene->RemoveObject(this);
			activeScene->AddDestroyObject(this);
		}
	}
}

void CCoin::SetType(int type)
{
	this->type = type;
}

int CCoin::GetType()
{
	return type;
}