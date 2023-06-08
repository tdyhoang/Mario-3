#include "QuestionBlock.h"
#include "../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "../../../Framework/GameComponents/Game.h"
#include "../Mario/Mario.h"
#include "../Mario/MarioController.h"

CQuestionBlock::CQuestionBlock()
{
	CBlock::CBlock();
	SetTag(ObjectTag::QuestionBlock);
	LoadAnimation();
	SetState("SEALED");
	isEnabled = true;
	bounceState = 0;
	startBounceTime = 0;
	bounceDelta = 0.0f;
	target = NULL;
}

void CQuestionBlock::SetTarget(CGameObject* target)
{
	this->target = target;
}

void CQuestionBlock::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation("SEALED", animations->Get("ani-question-block"));
	AddAnimation("EMPTY", animations->Get("ani-empty-block"));
}

void CQuestionBlock::SetItemInfo(ItemInfo info)
{
	this->itemInfo = info;
}

void CQuestionBlock::Bounce()
{
	if (bounceState == 0)
	{
		startBounceTime = GetTickCount64();
		bounceState = 1;
		if (itemInfo.quantity >= 0)
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
}

void CQuestionBlock::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (bounceState == 2)
	{
		bounceDelta += 0.48 * dt;

		if (GetTickCount64() - startBounceTime > 50 && startBounceTime != 0)
		{
			bounceDelta = 0.0f;
			startBounceTime = 0;
			bounceState = 0;
			if (itemInfo.quantity <= 0)
				bounceState = -1;
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

void CQuestionBlock::Render(CCamera* cam, int alpha)
{
	relativePositionOnScreen.y = bounceDelta;
	CGameObject::Render(cam, alpha);
}

bool CQuestionBlock::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	return false;
}

void CQuestionBlock::OnOverlappedEnter(CHitBox* selfCollisionBox, CHitBox* otherCollisionBox)
{
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == ObjectTag::RaccoonTail)
	{
		Bounce();
	}
}