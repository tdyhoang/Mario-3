#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>

#include "../Graphics/Animation/Animation.h"
#include "../Scene/Camera/Camera.h"
#include "../Collision/PhysicsBody.h"
#include "../Collision/HitBox.h"
#include "ObjectTag.h"
#include "../Scene/Scene.h"
#include "../Graph/Node.h"
#include "../../Ultis/Index.h"

class CScene;
typedef CScene* LPScene;

class CPhysicsBody;
typedef CPhysicsBody* LPPhysicsBody;

class CHitBox;
typedef CHitBox* LPHitBox;

class CGameObject;
typedef CGameObject* LPGameObject;

class CCamera;
typedef CCamera* LPCamera;

struct CollisionEvent;

class CAnimation;
typedef CAnimation* LPAnimation;

struct Index;

class CGameObject : public CNode
{
protected:
	int id;
	DWORD dt;
	Index currentCellIndex;

	Transform transform;
	D3DXVECTOR2 relativePositionOnScreen = D3DXVECTOR2(0.0f, 0.0f);

	std::string currentState, lastState;

	bool isEnabled;
	bool ignoreTimeScale;
	bool isDestroyed;
	bool isStatic;
	bool isInGrid;
	ObjectTag tag;
	std::unordered_map<std::string, LPAnimation> animations;

	std::vector<LPHitBox>* hitBoxs;
	LPPhysicsBody physicsBody;
	bool isCheckWithCollision;
public:
	CGameObject();

	static D3DXVECTOR2 GetGameObjectSize(CGameObject* gO);
	virtual void Init() = 0;
	bool IsDestroyed();
	void SetDestroy(bool isDes);

	virtual void PhysicsUpdate(std::vector<LPGameObject>* coObjects);
	virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam);
	virtual void Render(CCamera* cam, int alpha = 255);

	void FrictionProcess(float& speed, DWORD dt, bool isStop);
	void ResetTempValues();

	virtual void OnCollisionEnter(CHitBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions) {}
	virtual void OnOverlappedEnter(CHitBox* selfHitBox, CHitBox* otherHitBox) {}

	std::string GetCurrentState();
	void AddAnimation(std::string stateName, LPAnimation animation, bool isLoop = true);
	void SetRelativePositionOnScreen(D3DXVECTOR2);
	virtual void EndAnimation() {}
	LPAnimation GetAnimationByState(std::string state);

	virtual void KeyState() {}
	virtual void OnKeyDown(int KeyCode) {}
	virtual void OnKeyUp(int KeyCode) {}

	bool IsEnabled();
	void Enable(bool isEnabled);

	bool IsIgnoreTimeScale();
	void SetIgnoreTimeSCale(bool isIgnoreTimeScale);

	D3DXVECTOR2 GetScale();
	void SetScale(D3DXVECTOR2 s);

	float GetRotation();
	void SetRotation(float r);

	D3DXVECTOR2 GetPosition();
	void SetPosition(D3DXVECTOR2 p);

	LPPhysicsBody GetPhysicsBody();
	void SetPhysicsBody(LPPhysicsBody p);

	std::vector<LPHitBox>* GetHitBox();
	void GetHitBox(std::vector<LPHitBox>* listHitBox);

	std::string GetState();
	void SetState(std::string state);

	ObjectTag GetTag();
	void SetTag(ObjectTag t);

	virtual bool CanCollideWithThisObject(LPGameObject gO, ObjectTag tag);
	bool MarioTag(ObjectTag tag);
	bool StaticTag(ObjectTag tag);
	bool GiftTag(ObjectTag tag);
	bool PlayerAttackItemTag(ObjectTag tag);

	bool IsStatic();
	void SetStatic(bool setStatic);
	bool IsInGrid();
	void SetInGrid(bool setInGrid);

	Index GetIndex();
	void SetIndex(Index index);
};