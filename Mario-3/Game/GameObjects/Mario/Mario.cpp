#include "Mario.h"

#include "../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../../../Framework/GameComponents/Game.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "../../../Framework/GameComponents/Const.h"
#include "../../../Framework/GameComponents/Graphics/Sprite/Sprite.h"

#include <cstdlib>
#include <cctype> 
#include <string>
// #include "MarioHitBox.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"
// #include "MarioController.h"

using namespace std;

CMario::CMario()
{
	LoadAnimation();
	InitProperties();
	tag = ObjectTag::Player;
}

void CMario::SetAutoGo(bool autoGo)
{
	isAutogo = autoGo;
}

void CMario::SetIsInIntro(bool intro)
{
	isInIntro = intro;
}

void CMario::SetPMeterCounting(int pMeterCounting)
{
	this->pMeterCounting = pMeterCounting;
}

void CMario::SetFeverState(int fState)
{
	this->feverState = fState;
}

void CMario::SetDamageFlag(bool isDamaged)
{
	this->isDamaged = isDamaged;
}

void CMario::SetChangeSmokeEffectFlag(bool isChangeSmokeEffect)
{
	this->isSmokeEffectAnimation = isChangeSmokeEffect;
}

void CMario::SetChangeLevelFlag(bool flag)
{
	this->isChangeLevel = flag;
}

void CMario::SetPowerUp(bool pU)
{
	this->isPowerUp = pU;
}

void CMario::SetCountChangeAlpha(int count)
{
	countChangeAlpha = count;
}

void CMario::SetCountSmokeEffectActivate(int count)
{
	countSmokeEffectActivate = count;
}

void CMario::SetTimeStartDamaged(DWORD t)
{
	timeStartDamaged = t;
}

void CMario::SetTimeStartSmokeEffect(DWORD t)
{
	timeStartSmokeEffect = t;
}

void CMario::SetTimeStartChangeLevel(DWORD t)
{
	timeStartChangeLevel = t;
}

void CMario::SetTimeStartChangeAlpha(DWORD t)
{
	timeStartChangeAlpha = t;
}

int CMario::GetPMeterCounting()
{
	return pMeterCounting;
}

int CMario::GetFeverState()
{
	return feverState;
}

bool CMario::GetDamageFlag()
{
	return isDamaged;
}

bool CMario::GetChangeSmokeEffectFlag()
{
	return isSmokeEffectAnimation;
}

bool CMario::GetChangeLevelFlag()
{
	return isChangeLevel;
}

bool CMario::GetCountChangeAlpha()
{
	return countChangeAlpha;
}

bool CMario::GetCountSmokeEffectActivate()
{
	return countSmokeEffectActivate;
}

bool CMario::IsPowerUp()
{
	return isPowerUp;
}

DWORD CMario::GetTimeStartDamaged()
{
	return timeStartDamaged;
}

DWORD CMario::GetTimeStartSmokeEffect()
{
	return timeStartSmokeEffect;
}

DWORD CMario::GetTimeStartChangeLevel()
{
	return timeStartChangeLevel;
}

DWORD CMario::GetTimeStartChangeAlpha()
{
	return timeStartChangeAlpha;
}

void CMario::Init()
{
	this->SetState("IDLE");
	/*CMarioHitBox* hitBox = new CMarioHitBox();
	hitBox->SetSizeBox(SUPER_MARIO_BBOX);
	hitBox->SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	hitBox->SetGameObjectAttach(this);
	hitBox->SetName("Mario");
	hitBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->hitBoxs->push_back(hitBox);*/
}

void CMario::InitProperties()
{
	physicsBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	physicsBody->SetDynamic(true);
	physicsBody->SetGravity(MARIO_GRAVITY);

	targetVelocity.x = 0.0f;
	targetVelocity.y = 0.0f;

	previousNormal = physicsBody->GetNormal();
	ignoreTimeScale = true;
	isEnabled = true;
	isOnGround = false;
	canHighJump = false;
	isJump = false;	isSkid = false;
	isAttack = false;
	canFly = false;
	isFly = false;
	stopBounce = false;
	isKick = false;
	isDamaged = false;
	isGoToWarpPipe = false;
	canGoToWarpPipe = false;
	isSmokeEffectAnimation = false;
	isChangeLevel = false;
	isSwitchCamera = false;
	bounceAfterJumpOnEnemy = false;
	isInIntro = false;
	feverTime = 2000;
	lastFeverTime = 0;
	feverState = 0;
	pMeterCounting = 0.0f;
	beforeJumpPosition = 0.0f;
	timeStartSmokeEffect = 0;
	timeStartChangeLevel = 0;
	timeStartDamaged = 0;
	countSmokeEffectActivate = 0;
	countChangeAlpha = 0;
	timeStartChangeAlpha = 0;
	isPowerUp = false;
	isAutogo = false;
	isHitGoalRoulette = false;
	isDie = false;
	label = NULL;
	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
	ventDirection = { 0, 0, 0, 0 };
	uiCamera = NULL;
	previousPosition = D3DXVECTOR2(0.0f, 0.0f);
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation("DAMAGED", animationManager->Get("ani-mario-damaged"), false);
}

void CMario::EndAnimation()
{
	if (currentState.compare("KICK") == 0)
		isKick = false;
}

void CMario::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	CGameObject::Update(dt, cam, uiCam);
	auto velocity = physicsBody->GetVelocity();
	auto normal = physicsBody->GetNormal();
	previousVelocity = velocity;
	float acceleration = 0.0f;

	D3DXVECTOR2 drag = physicsBody->GetDragForce();
}

void CMario::Render(CCamera* cam, int alpha)
{
	SetScale(D3DXVECTOR2(physicsBody->GetNormal().x, 1.0f));

	SetRelativePositionOnScreen(hitBoxs->at(0)->GetPosition());
	countChangeAlpha++;

	if (isDamaged == true && isSmokeEffectAnimation == false && isPowerUp == false)
		if ((GetTickCount64() - timeStartChangeAlpha > 100 && timeStartChangeAlpha != 0) || timeStartChangeAlpha == 0)
			alpha = (countChangeAlpha % 2 == 0) ? 0 : 255;
	CGameObject::Render(cam, alpha);

}

void CMario::OnCollisionEnter(CHitBox* selfHitBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto hitBox = collisionEvent->obj;
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Solid || hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::GhostPlatform
			|| hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::QuestionBlock || hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Brick
			|| hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::SwitchBlock || hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::MovingPlatform
			|| hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Pipe)
		{
			if (collisionEvent->ny < 0 && isOnGround == false)
			{
				isOnGround = true;
				DebugOut(L"OnGround\n");
			}
			if (collisionEvent->nx != 0)
			{
				auto v = physicsBody->GetVelocity();
				physicsBody->SetVelocity(D3DXVECTOR2(0, v.y));
				if (canFly == false)
					pMeterCounting = 0;
			}
		}
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::QuestionBlock && collisionEvent->ny > 0)
		{
			FallProcess();
		}
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Brick && collisionEvent->ny > 0)
		{
			FallProcess();
		}
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Portal && canGoToWarpPipe == true && isGoToWarpPipe == true)
		{
			isGoToWarpPipe = false;
			canGoToWarpPipe = false;
		}

	}
}

void CMario::OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox)
{
	if (otherHitBox->GetGameObjectAttach()->GetTag() == ObjectTag::VenusFireBall)
	{
		OnDamaged();
		otherHitBox->GetGameObjectAttach()->Enable(false);
	}
	if (otherHitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Label)
	{
		label = otherHitBox->GetGameObjectAttach();
		OnGoToWarpPipe();
	}
	if (otherHitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Portal)
	{
		isGoToWarpPipe = false;
		canGoToWarpPipe = false;
	}
}

bool CMario::CanCollideWithThisObject(LPGameObject gO, ObjectTag tag)
{
	if (MarioTag(tag) || tag == ObjectTag::MarioFireBall || tag == ObjectTag::Label)
		return false;
	if (tag == ObjectTag::Coin)
		return false;
	if (isGoToWarpPipe == true && StaticTag(tag))
		return false;
	if (tag == ObjectTag::Card)
		return false;
	if (tag == ObjectTag::Boomerang)
		return false;
	return true;
}

void CMario::JumpProcess(float jumpForce, bool bounceAfterJumpOnEnemy)
{
	physicsBody->SetVelocity(D3DXVECTOR2(physicsBody->GetVelocity().x, jumpForce));
	isJump = true;
	isOnGround = false;
	this->bounceAfterJumpOnEnemy = bounceAfterJumpOnEnemy;
}

void CMario::KickProcess(bool isKick)
{
	this->isKick = isKick;
}

void CMario::InvincibleProcess()
{
	if (timeStartDamaged != 0)
		if (GetTickCount64() - timeStartDamaged > 2000)
		{
			isDamaged = false;
			timeStartDamaged = 0;
			timeStartChangeLevel = 0;
			timeStartChangeAlpha = 0;
			countSmokeEffectActivate = 0;
			countChangeAlpha = 0;
		}

	if (isSmokeEffectAnimation == true && GetTickCount64() - timeStartSmokeEffect > 800)
	{
		isSmokeEffectAnimation = false;
		timeStartSmokeEffect = 0;
		timeStartChangeLevel = GetTickCount64();

		timeStartChangeAlpha = GetTickCount64();
		countChangeAlpha++;
	}

	if (isDamaged == true && isSmokeEffectAnimation == false)
	{
		countSmokeEffectActivate++;
		if (countSmokeEffectActivate == 1)
		{
			timeStartSmokeEffect = GetTickCount64();
			CGame::SetTimeScale(0.0f);
			isSmokeEffectAnimation = true;
		}
	}

	if (isChangeLevel == true && GetTickCount64() - timeStartChangeLevel > 800)
	{

		timeStartChangeLevel = 0;
		isChangeLevel = false;
		return;
	}

	if (isDamaged == true && timeStartChangeLevel != 0 && isChangeLevel == false)
		ChangeLevelProcess();
}

void CMario::ChangeLevelProcess()
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	auto obj = activeScene->GetMarioController();
	if (obj == NULL) return;
	/*CMarioController* marioController = static_cast<CMarioController*>(obj);

	if (marioController != NULL)
	{
		CGame::SetTimeScale(1.0f);

		isChangeLevel = true;
	}*/
}

void CMario::FallProcess()
{
	auto vel = physicsBody->GetVelocity();
	vel.y += 0.2;
	physicsBody->SetVelocity(vel);
	isJump = false;
	canHighJump = false;
	isHighJump = false;
}

void CMario::GoToWarpPipeProcess()
{
	if (label == NULL)
		return;
	bool isOnWarpPipe = true;
	auto labelPos = label->GetPosition();
	auto labelSize = label->GetHitBox()->at(0)->GetSizeBox();
	if (this->transform.position.x < labelPos.x - labelSize.x * 0.5f || this->transform.position.x > labelPos.x + labelSize.x * 0.5f)
		isOnWarpPipe = false;
}

void CMario::HitGoalRoulette()
{
	isAutogo = true;
	isHitGoalRoulette = true;
}

void CMario::WarpPipeProcess(CCamera* cam)
{
	if (isGoToWarpPipe == true)
	{
		this->physicsBody->SetGravity(0.0f);
		this->physicsBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
		if (ventDirection.bottom == 1)
			transform.position.y += 0.02 * CGame::GetInstance()->GetDeltaTime();
		if (ventDirection.top == 1)
			transform.position.y -= 0.02 * CGame::GetInstance()->GetDeltaTime();
		isSwitchCamera = true;
	}
	else if (isHitGoalRoulette == false && isDie == false && isInIntro == false)
	{
		isAutogo = false;
		auto camPos = cam->GetPositionCam();
		camPos.x += cam->GetWidthCam() / 2;
		SetPosition(camPos);
		this->physicsBody->SetGravity(MARIO_GRAVITY);
		isSwitchCamera = false;
	}
}

void CMario::GoalRouletteProcess(CCamera* cam)
{
	if (isHitGoalRoulette == true)
	{
		isAutogo = true;
		if (isOnGround == true)
		{
			physicsBody->SetVelocity(D3DXVECTOR2(0.24f, 0.0f));
			cam->SetDisablePosX(true);
		}
	}
}

void CMario::DieProcess(CCamera* cam)
{

	if (isDie == true)
	{
		switch (dieState)
		{
		case 0:
		{
			this->hitBoxs->at(0)->SetEnable(false);
			SetPosition(previousPosition);
			dieState = 1;
			break;
		}
		case 1:
		{
			physicsBody->SetVelocity(D3DXVECTOR2(0, -0.3));
			if (transform.position.y < previousPosition.y - 100)
				dieState = 2;
			break;
		}
		case 2:
		{
			physicsBody->SetVelocity(D3DXVECTOR2(0, 0.3));
			if (transform.position.y >= cam->GetCurrentBoundary().bottom - 200)
			{
				isAutogo = false;
				CGame::GetInstance()->SetTimeScale(1.0f);
			}
			break;
		}
		}
	}
}

void CMario::StopBounce(bool stopBounce)
{
	this->stopBounce = stopBounce;
}

bool CMario::StopBounce()
{
	return stopBounce;
}

void CMario::OnKeyUp(int KeyCode)
{
	if (KeyCode == DIK_S)
	{
		canHighJump = false;
	}
	if (KeyCode == DIK_A)
	{
		pMeterCounting = 0;
		feverState = 0;
	}
}

void CMario::OnDamaged()
{
	isDamaged = true;
	timeStartDamaged = GetTickCount64();
}

void CMario::OnGoToWarpPipe()
{
	canGoToWarpPipe = true;
}

void CMario::OnDie()
{
	if (isDamaged == true || isGoToWarpPipe == true || isSwitchCamera == true)
		return;
	previousPosition = GetPosition();
	this->isDie = true;
	isAutogo = true;
	CGame::GetInstance()->SetTimeScale(0.0f);
}

CMario::~CMario()
{
	CGameObject::~CGameObject();
}