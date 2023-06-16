#pragma once

#include "MarioStateSet.h"
#include "../../../Framework/GameComponents/GameObject/GameObject.h"
#include "../../../Framework/GameComponents/StateMachine/IState.h"
#include "../../../Framework/GameComponents/Keyboard/KeyboardManager.h"
#include "../ItemInfo.h"

class CMario : public CGameObject, public IState
{
protected:
	bool isInIntro;
	MarioStates marioStateTag;
	D3DXVECTOR2 targetVelocity, previousVelocity;
	MarioStateSet currentPhysicsState, previousPhysicsState;
	bool isOnGround;
	bool isHighSpeed;
	bool canLowJumpContinous;
	bool isHighJump, canHighJump, isJump;
	int isSkid;
	bool canSquat;
	D3DXVECTOR2 previousNormal;
	D3DXVECTOR2 previousPosition;
	bool canAttack, isAttack, canAttackContinious;
	bool isJumpAttack;
	int feverState;
	DWORD feverTime;
	DWORD lastFeverTime;
	DWORD beginAttackTime;
	float pMeterCounting;
	float beforeJumpPosition;
	bool canFly;
	bool isFly;
	bool bounceAfterJumpOnEnemy, stopBounce;
	bool isHold, isKick;
	bool isDamaged, isSmokeEffectAnimation, isChangeLevel, isSwitchCamera;
	DWORD timeStartSmokeEffect;
	DWORD timeStartChangeLevel;
	DWORD timeStartDamaged;
	DWORD timeStartChangeAlpha;
	int countSmokeEffectActivate;
	int countChangeAlpha;
	bool isPowerUp;
	PowerupTag powerupItem;
	bool isGoToWarpPipe, canGoToWarpPipe;
	RectFrame ventDirection;
	bool isAutogo;
	bool isHitGoalRoulette;
	CGameObject* label;
	bool isDie;
	int dieState = 0;
public:
	CMario();

	void SetAutoGo(bool autoGo);
	void SetIsInIntro(bool intro);
	void SetPhysicsState(MarioStateSet physState);
	MarioStateSet GetPhysicsState();
	void SetCurrentPhysicsState(MarioStateSet state);
	void SetPMeterCounting(int pMeterCounting);
	void SetFeverState(int fState);
	void SetDamageFlag(bool isDamaged);
	void SetChangeSmokeEffectFlag(bool isChangeSmokeEffect);
	void SetChangeLevelFlag(bool flag);
	void SetPowerUp(bool pU);
	void SetPowerUpItem(PowerupTag powerupItem);

	void SetCountChangeAlpha(int count);
	void SetCountSmokeEffectActivate(int count);

	void SetTimeStartDamaged(DWORD t);
	void SetTimeStartSmokeEffect(DWORD t);
	void SetTimeStartChangeLevel(DWORD t);
	void SetTimeStartChangeAlpha(DWORD t);

	int GetPMeterCounting();
	int GetFeverState();

	bool GetDamageFlag();
	bool GetChangeSmokeEffectFlag();
	bool GetChangeLevelFlag();

	bool GetCountChangeAlpha();
	bool GetCountSmokeEffectActivate();

	bool IsPowerUp();
	PowerupTag GetPowerupItem();

	DWORD GetTimeStartDamaged();
	DWORD GetTimeStartSmokeEffect();
	DWORD GetTimeStartChangeLevel();
	DWORD GetTimeStartChangeAlpha();

	void Init() override;
	virtual void InitProperties();
	virtual void LoadAnimation();
	void EndAnimation() override;

	void Update(DWORD dt, CCamera* cam, CCamera* uiCam) override;
	void Render(CCamera* cam, int alpha = 255) override;

	void OnCollisionEnter(CHitBox* selfHitBox, std::vector<CollisionEvent*> otherCollisions);
	void OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox) override;

	bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag) override;

	void CrouchProcess(CKeyboardManager* keyboard);
	void SkidProcess(D3DXVECTOR2 velocity);
	void JumpProcess(float jumpForce, bool bounceAfterJumpOnEnemy);
	void KickProcess(bool isKick);
	void InvincibleProcess();
	void ChangeLevelProcess();
	void FallProcess();
	void GoToWarpPipeProcess();
	void HitGoalRoulette();
	void WarpPipeProcess(CCamera* cam);
	void GoalRouletteProcess(CCamera* cam);
	void DieProcess(CCamera* cam);

	void StopBounce(bool stopBounce);
	bool StopBounce();

	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void OnDamaged();
	void OnGoToWarpPipe();
	void OnDie();

	void SetMarioStateTag(MarioStates tag);
	MarioStates GettMarioStateTag();

	bool CanRun();

	virtual void Access() override;
	virtual void Process() override {}
	virtual void Exit() override;

	~CMario();
};