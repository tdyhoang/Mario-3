#include "QuestionBlock.h"
#include "../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "Effect/CoinEffect.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "../../../Framework/GameComponents/Game.h"
#include "Effect/MushroomEffect.h"
#include "../Mario/Mario.h"
#include "../Mario/MarioController.h"
#include "Misc.h"

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
			switch (itemInfo.tag)
			{
			case ItemTag::Coin:
			{
				CCoinEffect* coinObtainedFX = new CCoinEffect();
				coinObtainedFX->SetStartPosition(transform.position);
				auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
				activeScene->AddObject(coinObtainedFX);
				activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), coinObtainedFX);
				break;
			}
			case ItemTag::PowerUp:
			{
				if (target == NULL)
					break;
				auto marioController = static_cast<CMarioController*>(target);
				auto currentMario = marioController->GetCurrentStateObject();
				if (currentMario != NULL)
				{
					auto mario = static_cast<CMario*>(currentMario);
					switch (mario->GettMarioStateTag())
					{
					case MarioStates::SmallMario:
					{
						CMushroomEffect* mushroomObtainedFX = new CMushroomEffect();
						mushroomObtainedFX->SetStartPosition(transform.position);
						mushroomObtainedFX->StartEffect(mario->GetPhysicsBody()->GetNormal().x);
						auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
						activeScene->AddObject(mushroomObtainedFX);
						activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), mushroomObtainedFX);
						break;
					}
					case MarioStates::SuperMario:
					{
						CMushroomEffect* mushroomObtainedFX = new CMushroomEffect();
						mushroomObtainedFX->SetStartPosition(transform.position);
						mushroomObtainedFX->StartEffect(mario->GetPhysicsBody()->GetNormal().x);
						auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
						activeScene->AddObject(mushroomObtainedFX);
						activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), mushroomObtainedFX);
						break;
					}
					case MarioStates::FireMario:
					{
						CMushroomEffect* mushroomObtainedFX = new CMushroomEffect();
						mushroomObtainedFX->SetStartPosition(transform.position);
						mushroomObtainedFX->StartEffect(mario->GetPhysicsBody()->GetNormal().x);
						auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
						activeScene->AddObject(mushroomObtainedFX);
						activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), mushroomObtainedFX);
						break;
					}
					case MarioStates::RacoonMario:
					{
						CMushroomEffect* mushroomObtainedFX = new CMushroomEffect();
						mushroomObtainedFX->SetStartPosition(transform.position);
						mushroomObtainedFX->StartEffect(mario->GetPhysicsBody()->GetNormal().x);
						auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
						activeScene->AddObject(mushroomObtainedFX);
						activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), mushroomObtainedFX);
						break;
					}
					}
				}

				break;
			}
			}
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
		bounceDelta += BOUNCE_VEL * dt;

		if (GetTickCount64() - startBounceTime > BOUNCE_TIME && startBounceTime != 0)
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
		bounceDelta += -BOUNCE_VEL * dt;

		if (GetTickCount64() - startBounceTime > BOUNCE_TIME && startBounceTime != 0)
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

void CQuestionBlock::OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox)
{
	if (otherHitBox->GetGameObjectAttach()->GetTag() == ObjectTag::RaccoonTail)
		Bounce();
}