#include "HitEffect.h"
#include "../../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../../../../Framework/Ultis/Ultis.h"

CHitEffect::CHitEffect()
{
	LoadAnimation();
	SetState("ENEMY-DAMAGED");
	startHitTime = 0;
}

void CHitEffect::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
}

void CHitEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (GetTickCount64() - startHitTime > 300 && startHitTime != 0)
		isEnabled = false;
}

void CHitEffect::Render(CCamera* cam, int alpha)
{
	CGameObject::Render(cam, alpha);
}

void CHitEffect::SetStartHitTime(DWORD t)
{
	startHitTime = t;
}