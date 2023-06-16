#include "CoinEffect.h"
#include "../../../../Framework/GameComponents/Graphics/Animation/AnimationManager.h"
#include "../Misc.h"

CCoinEffect::CCoinEffect()
{
	LoadAnimation();
	SetState(COIN_STATE_SPIN);
	isEnabled = true;

	CHitBox* box = new CHitBox();
	box->SetSizeBox(D3DXVECTOR2(0.0f, 0.0f));
	box->SetGameObjectAttach(this);
	box->SetEnable(false);
	hitBoxs->push_back(box);
	physicsBody->SetGravity(COIN_GRAVITY);
	physicsBody->SetVelocity(D3DXVECTOR2(0.0f, -COIN_BOUNCE));
	physicsBody->SetDynamic(true);

	SetScale(D3DXVECTOR2(0.4f, 1.0f));

}

void CCoinEffect::LoadAnimation()
{
	auto animations = CAnimationManager::GetInstance();
	AddAnimation(COIN_STATE_SPIN, animations->Get("ani-coin"));
}

void CCoinEffect::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	if (transform.position.y > startPosition.y && *physicsBody->GetVelocity() > 0)
		isEnabled = false;
}