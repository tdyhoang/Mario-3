#include "Goomba.h"
#include "../../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../../../../Framework/Ultis/Ultis.h"
#include "../../../../Framework/GameComponents/Scene/SceneManager.h"

CGoomba::CGoomba()
{
	CGoomba::Init();
}

void CGoomba::Init()
{
	isEnabled = true;
	isJumpMaxHeight = false;

	CHitBox* hitBox = new CHitBox();
	hitBox->SetSizeBox(GOOMBA_BBOX);
	hitBox->SetGameObjectAttach(this);
	hitBox->SetName("Goomba");
	hitBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	hitBoxs->push_back(hitBox);


	physicsBody->SetDynamic(true);
	physicsBody->SetGravity(GOOMBA_GRAVITY);
	physicsBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	beforeHitPosition = D3DXVECTOR2(0.0f, 0.0f);

	currentPhysicsState = GoombaState::Walk;
	enemyTag = EnemyTag::Goomba;
}

void CGoomba::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto velocity = physicsBody->GetVelocity();
	auto normal = physicsBody->GetNormal();
	if (currentPhysicsState == GoombaState::Walk)
	{
		velocity.x = normal.x * GOOMBA_SPEED;
	}
	else if (GetTickCount64() - startDeadTime > GOOMBA_DIE_TIME && currentPhysicsState == GoombaState::Die)
	{
		this->isEnabled = false;
		physicsBody->SetDynamic(false);
		physicsBody->SetGravity(0.0f);
		velocity.y = 0.0f;

	}
	physicsBody->SetVelocity(velocity);
}

void CGoomba::Render(CCamera* cam, int alpha)
{
	auto normal = physicsBody->GetNormal();
	SetScale(D3DXVECTOR2(1.0f, normal.y));
	switch (currentPhysicsState)
	{
	case GoombaState::Walk:
	{
		SetState("WALK");
		break;
	}
	case GoombaState::Die:
	{
		SetState("DIE");
		break;
	}
	case GoombaState::HeadShot:
	{
		SetState("HEAD-SHOT");
		break;
	}
	}
	CGameObject::Render(cam, alpha);
}

void CGoomba::OnDie()
{
	auto v = physicsBody->GetVelocity();
	auto normal = physicsBody->GetNormal();

	if (isHeadShot == true)
	{
		hitBoxs->at(0)->SetEnable(false);

		hitFX->SetStartPosition(this->transform.position);
		hitFX->SetStartHitTime(GetTickCount64());
		currentPhysicsState = GoombaState::HeadShot;
		v.x = GOOMBA_HIT_FORCE_X;
		normal.y = -1;
		countDeadCallback++;

		if (countDeadCallback == 1)
		{
			beforeHitPosition = transform.position;
			v.y = -GOOMBA_HIT_FORCE;
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->AddObject(hitFX);
			activeScene->GetGrid()->Move(D3DXVECTOR2(-1, -1), hitFX);
			hitFX->Enable(true);
		}
	}
	else
	{
		currentPhysicsState = GoombaState::Die;

		v.x = 0.0f;
		v.y = 0.0f;

		countDeadCallback++;
		if (countDeadCallback == 1)
			startDeadTime = GetTickCount64();

		SetRelativePositionOnScreen(D3DXVECTOR2(0, (GOOMBA_BBOX.y - GOOMBA_DIE_BBOX.y) * 0.5f));
		hitBoxs->at(0)->SetSizeBox(GOOMBA_DIE_BBOX);
		physicsBody->SetGravity(0.0f);

	}
	physicsBody->SetVelocity(v);
	physicsBody->SetNormal(normal);
}