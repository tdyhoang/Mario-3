#include "Mario.h"

#include "../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../../../Framework/GameComponents/Game.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "../../../Framework/GameComponents/Const.h"
#include "MarioConst.h"
#include "../../../Framework/GameComponents/Graphics/Sprite/Sprite.h"

#include <cstdlib>
#include <cctype> 
#include <string>
#include "MarioHitBox.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"
#include "MarioController.h"
#include "../../Worlds/World1/Scene1.h"

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

void CMario::SetPhysicsState(MarioStateSet physState)
{
	currentPhysicsState = physState;
}

MarioStateSet CMario::GetPhysicsState()
{
	return currentPhysicsState;
}

void CMario::SetCurrentPhysicsState(MarioStateSet state)
{
	this->currentPhysicsState = state;
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
	SetState("IDLE");
	CMarioHitBox* hitBox = new CMarioHitBox();
	hitBox->SetSizeBox(SUPER_MARIO_BBOX);
	hitBox->SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	hitBox->SetGameObjectAttach(this);
	hitBox->SetName("Mario");
	hitBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	hitBoxs->push_back(hitBox);
}

void CMario::InitProperties()
{
	physicsBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	physicsBody->SetDynamic(true);
	physicsBody->SetGravity(MARIO_GRAVITY);

	currentPhysicsState =
	{
		MoveOnGroundStates::Idle,
		JumpOnAirStates::Stand
	};
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
	previousPosition = D3DXVECTOR2(0.0f, 0.0f);
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
}

void CMario::EndAnimation()
{
	CGameObject::EndAnimation();
	if (currentState.compare("KICK") == 0)
	{
		isKick = false;
	}
}

void CMario::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	CGameObject::Update(dt, cam, uiCam);
	auto keyboard = CKeyboardManager::GetInstance();
	auto velocity = physicsBody->GetVelocity();
	auto normal = physicsBody->GetNormal();
	previousVelocity = velocity;
	float acceleration = 0.0f;

	D3DXVECTOR2 drag = physicsBody->GetDragForce();
	if (isAutogo == false)
	{
		if (keyboard->GetKeyStateDown(DIK_RIGHT) || keyboard->GetKeyStateDown(DIK_LEFT))
		{
			if (keyboard->GetKeyStateDown(DIK_A))
			{
				currentPhysicsState.move = MoveOnGroundStates::Run;
				acceleration = MARIO_RUNNING_ACCELERATION;
				targetVelocity.x = MARIO_RUNNING_SPEED;
			}
			else
			{
				currentPhysicsState.move = MoveOnGroundStates::Walk;
				acceleration = MARIO_WALKING_ACCELERATION;
				targetVelocity.x = MARIO_WALKING_SPEED;
			}

			normal.x = (keyboard->GetKeyStateDown(DIK_RIGHT)) ? 1 : -1;
			physicsBody->SetNormal(normal);
			physicsBody->SetAcceleration(acceleration * normal.x);

			if (abs(velocity.x) < targetVelocity.x)
				velocity.x += physicsBody->GetAcceleration() * dt;


			FrictionProcess(velocity.x, dt, false);
			physicsBody->SetVelocity(velocity);

			SkidProcess(velocity);
			if (previousVelocity.x * velocity.x <= 0)
				isSkid = false;
			if (isSkid == true)
				currentPhysicsState.move = MoveOnGroundStates::Skid;
		}
		else
		{
			FrictionProcess(velocity.x, dt, true);
			physicsBody->SetVelocity(velocity);

			if (velocity.x == 0)
				if (currentPhysicsState.move != MoveOnGroundStates::Idle)
					currentPhysicsState.move = MoveOnGroundStates::Idle;
			isSkid = false;
		}

#pragma region P-METER
		if (feverState != 3)
		{
			if (currentPhysicsState.move == MoveOnGroundStates::Run
				&& abs(velocity.x) > MARIO_RUNNING_SPEED * 0.15f
				&& pMeterCounting < 7
				&& currentPhysicsState.jump == JumpOnAirStates::Stand
				&& feverState != 2
				&& isHold == false)
			{
				if (feverState != -1)
					feverState = 1;

				pMeterCounting += 0.005F * dt;
				if (pMeterCounting > 7)
				{
					pMeterCounting = 7;
				}
			}
			else if (feverState != 2 && feverState != -1)
			{
				feverState = 0;
				pMeterCounting -= 0.005F * dt;
				if (pMeterCounting < 0)
					pMeterCounting = 0;
			}
#pragma endregion

#pragma region FEVER STATE
			if (pMeterCounting >= 6 && feverState == 0)
			{
				feverState = 2;
				lastFeverTime = GetTickCount64();
			}
			else if (pMeterCounting > 0 && feverState == -1)
			{
				if (pMeterCounting >= 7)
				{
					canFly = true;
				}
			}
			if (feverState == 2)
			{
				pMeterCounting = 7;
				if (GetTickCount64() - lastFeverTime > 2000)
				{
					feverState = 3;
					pMeterCounting -= 0.005F * dt;
				}
			}
#pragma endregion

		}
		else
		{
			pMeterCounting -= 0.005F * dt;
			if (pMeterCounting <= 0)
				feverState = 0;
		}

		if (pMeterCounting >= 6 && (abs(velocity.x) > 0))
			currentPhysicsState.move = MoveOnGroundStates::HighSpeed;

#pragma region STATE JUMP
		if (keyboard->GetKeyStateDown(DIK_X) && isOnGround == true)
		{
			if (canLowJumpContinous == true)
			{
				velocity.y -= MARIO_JUMP_FORCE;
				isOnGround = false;
			}
			currentPhysicsState.jump = JumpOnAirStates::LowJump;
		}
		else if (currentPhysicsState.jump == JumpOnAirStates::LowJump)
		{
			if (velocity.y > 0)
			{
				currentPhysicsState.jump = JumpOnAirStates::Fall;
			}
			else
				currentPhysicsState.jump = JumpOnAirStates::LowJump;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Jump && canLowJumpContinous == false && canHighJump == true)
		{
			if ((keyboard->GetKeyStateDown(DIK_S) && isFly == false) || bounceAfterJumpOnEnemy == true)
			{
				float jumpMaxHeight;
				float force = MARIO_PUSH_FORCE;
				if ((feverState == 2 && abs(velocity.x) > MARIO_RUNNING_SPEED * 0.85f) || bounceAfterJumpOnEnemy == true)
				{
					jumpMaxHeight = MARIO_SUPER_JUMP_HEIGHT;
					force = MARIO_PUSH_FORCE * 1.5f;
				}
				else
				{
					jumpMaxHeight = MARIO_HIGH_JUMP_HEIGHT;
					force = MARIO_PUSH_FORCE;
				}
				if (abs(beforeJumpPosition) - abs(transform.position.y) <= jumpMaxHeight)
				{
					velocity.y = -force;
				}
				else
				{
					velocity.y = -force / 2;
					canHighJump = false;
				}
				if (bounceAfterJumpOnEnemy == true)
				{
					bounceAfterJumpOnEnemy = false;
					canHighJump = false;
					stopBounce = true;
				}
			}
		}
		if (velocity.y > 0)
		{
			currentPhysicsState.jump = JumpOnAirStates::Fall;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Fly)
		{
			if (isOnGround == true)
				currentPhysicsState.jump = JumpOnAirStates::Stand;
			if (marioStateTag != MarioStates::RacoonMario)
			{
				feverState = 0;
				pMeterCounting = 0;
			}
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Fall)
		{
			if (isOnGround == true)
				currentPhysicsState.jump = JumpOnAirStates::Stand;
			isJump = false;
		}

#pragma endregion

		physicsBody->SetVelocity(velocity);
		physicsBody->SetNormal(normal);


		if (canSquat == true)
			CrouchProcess(keyboard);
		GoToWarpPipeProcess();

		InvincibleProcess();
		if (isKick == true)
		{
			previousPhysicsState.move = currentPhysicsState.move;
			currentPhysicsState.move = MoveOnGroundStates::Kick;
		}
		if (currentPhysicsState.move == MoveOnGroundStates::Kick && isKick == false)
			currentPhysicsState.move = previousPhysicsState.move;
	}
	else
	{
		WarpPipeProcess(cam);
		GoalRouletteProcess(cam);
		DieProcess(cam);
	}
}

void CMario::Render(CCamera* cam, int alpha)
{
	SetScale(D3DXVECTOR2(physicsBody->GetNormal().x, 1.0f));

#pragma region Update State
	if (isSmokeEffectAnimation == false && isGoToWarpPipe == false)
	{
#pragma region Move On Ground
		switch (currentPhysicsState.move)
		{
		case MoveOnGroundStates::Idle:
		{
			if (isHold == true)
				SetState("PICK-UP-IDLE");
			else
				SetState("IDLE");
			break;
		}
		case MoveOnGroundStates::Walk:
		{
			if (isHold == true)
				SetState("PICK-UP-WALK");
			else
				SetState("WALK");
			break;
		}
		case MoveOnGroundStates::Run:
		{
			if (isHold == true)
				SetState("PICK-UP-RUN");
			else
				SetState("RUN");
			break;
		}
		case MoveOnGroundStates::Skid:
		{
			if (isHold == false)
			{
				auto normal = physicsBody->GetNormal();
				SetScale(D3DXVECTOR2(-normal.x, 1.0f));
				SetState("SKID");
				break;
			}

		}
		case MoveOnGroundStates::Crouch:
		{
			if (isHold == false)
				SetState("SQUAT");
			break;
		}
		case MoveOnGroundStates::HighSpeed:
		{
			if (isHold == false)
				SetState("HIGH-SPEED");
			break;
		}
		case MoveOnGroundStates::Attack:
		{
			if (isHold == false)
				SetState("ATTACK");
			break;
		}
		case MoveOnGroundStates::JumpAttack:
		{
			if (isHold == false)
				SetState("JUMP-ATTACK");
			break;
		}
		case MoveOnGroundStates::Kick:
		{
			if (isHold == false)
				SetState("KICK");
			break;
		}
		}
#	pragma endregion

#pragma region Jump On Air
		if (currentPhysicsState.move != MoveOnGroundStates::JumpAttack)
		{
			switch (currentPhysicsState.jump)
			{
			case JumpOnAirStates::Jump: case JumpOnAirStates::HighJump:
			case JumpOnAirStates::LowJump:
			{
				if (isHold)
					SetState("PICK-UP-JUMP");
				else
					SetState("JUMP");
				break;
			}
			case JumpOnAirStates::Fall:
			{
				if (isHold)
					SetState("PICCK-UP-JUMP");
				else
					SetState("FALL");
				break;
			}
			case JumpOnAirStates::Fly:
			{
				if (isHold)
					SetState("PICK-UP-JUMP");
				else
					SetState("FLY");
				break;
			}
			case JumpOnAirStates::Float:
			{
				if (!isHold)
					SetState("FLOAT");
				break;
			}
			}
			if (feverState == 2 && isOnGround == false)
			{
				if (!isHold)
					SetState("FLY");
			}
		}

#pragma endregion
	}
	else if (isGoToWarpPipe == false)
	{
		SetState("DAMAGED");
	}
	else
	{
		SetState("IDLE-FRONT");
	}
	if (isDie)
		SetState("DIE");
#pragma endregion

	SetRelativePositionOnScreen(hitBoxs->at(0)->GetPosition());
	countChangeAlpha++;

	if (isDamaged == true && isSmokeEffectAnimation == false && isPowerUp == false)
	{
		if ((GetTickCount64() - timeStartChangeAlpha > 100 && timeStartChangeAlpha != 0) || timeStartChangeAlpha == 0)
		{
			alpha = (countChangeAlpha % 2 == 0) ? 0 : 255;
		}
	}
	CGameObject::Render(cam, alpha);

}

void CMario::OnCollisionEnter(CHitBox* selfHitBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto hitBox = collisionEvent->obj;
		if (hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Solid || hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::GhostPlatform
			|| hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::QuestionBlock || hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::Brick
			|| hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::SwitchBlock
			|| hitBox->GetGameObjectAttach()->GetTag() == ObjectTag::MovingPlatform
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
				{
					pMeterCounting = 0;
				}
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

void CMario::CrouchProcess(CKeyboardManager* keyboard)
{
	bool changeAniState = false;
	if (keyboard->GetKeyStateDown(DIK_LEFT) || keyboard->GetKeyStateDown(DIK_RIGHT) || currentState == "ATTACK" || currentState == "IDLE-FRONT")
	{
		changeAniState = true;
	}
	if ((changeAniState == false && keyboard->GetKeyStateDown(DIK_DOWN)))
	{
		hitBoxs->at(0)->SetSizeBox(SUPER_MARIO_CROUCH_BBOX);
		float transformY = SUPER_MARIO_BBOX.y - SUPER_MARIO_CROUCH_BBOX.y;
		hitBoxs->at(0)->SetPosition(D3DXVECTOR2(0.0f, transformY * 0.5f));
		currentPhysicsState.move = MoveOnGroundStates::Crouch;
	}
	else
	{
		hitBoxs->at(0)->SetSizeBox(SUPER_MARIO_BBOX);
		hitBoxs->at(0)->SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	}
}

void CMario::SkidProcess(D3DXVECTOR2 velocity)
{
	auto keyboard = CKeyboardManager::GetInstance();
	int sign = keyboard->GetKeyStateDown(DIK_RIGHT) ? 1 : -1;
	if (isOnGround == true && velocity.x * sign < 0)
	{
		isSkid = true;
	}
}

void CMario::JumpProcess(float jumpForce, bool bounceAfterJumpOnEnemy)
{
	physicsBody->SetVelocity(D3DXVECTOR2(physicsBody->GetVelocity().x, jumpForce));
	isJump = true;
	isOnGround = false;
	this->bounceAfterJumpOnEnemy = bounceAfterJumpOnEnemy;
	if (bounceAfterJumpOnEnemy == true)
		currentPhysicsState.jump = JumpOnAirStates::Jump;
}

void CMario::KickProcess(bool isKick)
{
	this->isKick = isKick;
}

void CMario::InvincibleProcess()
{
	if (timeStartDamaged != 0)
	{
		if (GetTickCount64() - timeStartDamaged > 2000)
		{
			isDamaged = false;
			timeStartDamaged = 0;
			timeStartChangeLevel = 0;
			timeStartChangeAlpha = 0;
			countSmokeEffectActivate = 0;
			countChangeAlpha = 0;
		}
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

	// CHANGE LEVEL
	if (isChangeLevel == true && GetTickCount64() - timeStartChangeLevel > 800)
	{
		timeStartChangeLevel = 0;
		isChangeLevel = false;
		return;
	}

	if (isDamaged == true && timeStartChangeLevel != 0 && isChangeLevel == false)
	{
		ChangeLevelProcess();
	}
}

void CMario::ChangeLevelProcess()
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	auto obj = activeScene->GetMarioController();
	if (obj == NULL) return;
	CMarioController* marioController = static_cast<CMarioController*>(obj);

	if (marioController != NULL)
	{
		CGame::SetTimeScale(1.0f);

		isChangeLevel = true;
	}

}

void CMario::FallProcess()
{
	auto vel = physicsBody->GetVelocity();
	vel.y += 0.2F;
	physicsBody->SetVelocity(vel);
	currentPhysicsState.jump = JumpOnAirStates::Fall;
	isJump = false;
	canHighJump = false;
	isHighJump = false;
}

void CMario::GoToWarpPipeProcess()
{
	if (label == NULL)
		return;
	auto keyboard = CKeyboardManager::GetInstance();
	bool isOnWarpPipe = true;
	auto labelPos = label->GetPosition();
	auto labelSize = label->GetHitBox()->at(0)->GetSizeBox();
	if (this->transform.position.x < labelPos.x - labelSize.x * 0.5f || this->transform.position.x > labelPos.x + labelSize.x * 0.5f)
		isOnWarpPipe = false;
	if (canGoToWarpPipe == true && isOnWarpPipe == true)
	{
		if (keyboard->GetKeyStateDown(DIK_DOWN) && ventDirection.bottom == 1)
		{
			isGoToWarpPipe = true;
			isAutogo = true;
		}
		if (keyboard->GetKeyStateDown(DIK_UP) && ventDirection.top == 1)
		{
			isGoToWarpPipe = true;
			isAutogo = true;
		}
	}
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
			transform.position.y += 0.02F * CGame::GetInstance()->GetDeltaTime();
		if (ventDirection.top == 1)
			transform.position.y -= 0.02F * CGame::GetInstance()->GetDeltaTime();
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
		if (isOnGround == false && physicsBody->GetVelocity().y > 0)
		{
			currentPhysicsState.jump = JumpOnAirStates::Fall;
		}
		if (isOnGround == true)
		{
			currentPhysicsState.jump = JumpOnAirStates::Stand;
			currentPhysicsState.move = MoveOnGroundStates::Walk;
			physicsBody->SetVelocity(D3DXVECTOR2(MARIO_WALKING_SPEED, 0.0f));
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
			hitBoxs->at(0)->SetEnable(false);
			SetPosition(previousPosition);
			dieState = 1;
			break;
		}
		case 1:
		{
			physicsBody->SetVelocity(D3DXVECTOR2(0, -DIE_VELOCITY_Y));
			if (transform.position.y < previousPosition.y - DIE_JUMP)
				dieState = 2;
			break;
		}
		case 2:
		{
			physicsBody->SetVelocity(D3DXVECTOR2(0, DIE_VELOCITY_Y));
			if (transform.position.y >= cam->GetCurrentBoundary().bottom - DIE_FALL)
			{
				auto sceneManager = CSceneManager::GetInstance();
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

void CMario::OnKeyDown(int KeyCode)
{
	if ((KeyCode == DIK_S || KeyCode == DIK_X) && isOnGround == true && currentPhysicsState.jump == JumpOnAirStates::Stand)
	{
		JumpProcess(-MARIO_JUMP_FORCE, false);
		if (KeyCode == DIK_S)
		{
			canHighJump = true;
			canLowJumpContinous = false;
			beforeJumpPosition = transform.position.y;

		}
		if (KeyCode == DIK_X)
		{
			canLowJumpContinous = true;
			if (currentPhysicsState.jump == JumpOnAirStates::Stand
				|| currentPhysicsState.jump == JumpOnAirStates::Jump)
				currentPhysicsState.jump = JumpOnAirStates::LowJump;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Stand && KeyCode == DIK_S)
			currentPhysicsState.jump = JumpOnAirStates::Jump;
	}
	if ((KeyCode == DIK_Z || KeyCode == DIK_A) && canAttack == true && isAttack == false && currentPhysicsState.move != MoveOnGroundStates::Attack)
	{
		beginAttackTime = 0;
		isAttack = true;
	}
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
	if (marioStateTag == MarioStates::SmallMario && isPowerUp == false)
		OnDie();
	else
	{
		isDamaged = true;
		timeStartDamaged = GetTickCount64();
	}
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
	CScene1* scene = new CScene1();
	CSceneManager::GetInstance()->Load(scene);
}

void CMario::SetMarioStateTag(MarioStates tag)
{
	marioStateTag = tag;
}

MarioStates CMario::GettMarioStateTag()
{
	return marioStateTag;
}

bool CMario::CanRun()
{
	return currentPhysicsState.move == MoveOnGroundStates::Run;
}

void CMario::Access()
{
	this->isEnabled = true;
}

void CMario::Exit()
{
	this->isEnabled = false;
}

CMario::~CMario()
{
	CGameObject::~CGameObject();
}