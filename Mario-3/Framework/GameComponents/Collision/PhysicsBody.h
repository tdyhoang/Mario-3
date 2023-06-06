#pragma once

#include "HitBox.h"
#include <d3d9.h>
#include <d3d9.h>
#include "../GameObject/ObjectTag.h"

struct CollisionEvent;
typedef CollisionEvent* LPCollisionEvent;

class CHitBox;
typedef CHitBox* LPHitBox;

class CGameObject;
typedef CGameObject* LPGameObject;

class CPhysicsBody
{
private:
	D3DXVECTOR2 velocity;
	float gravity;
	float acceleration;
	D3DXVECTOR2 bounceForce;

	D3DXVECTOR2 dragForce;
	D3DXVECTOR2 normal;

	bool isDynamic;
	bool isTrigger;
public:
	CPhysicsBody();
	~CPhysicsBody();
	void PhysicsUpdate(LPGameObject cO, std::vector<LPGameObject>* coObjects);
	void Update(LPGameObject gO);

	bool CheckAABB(RectFrame selfBox, RectFrame otherBox);
	static void SweptAABB(
		float ml,
		float mt,
		float mr,
		float mb,
		float dx,
		float dy,
		float sl,
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny,
		ObjectTag tag);

	LPCollisionEvent SweptAABBEx(LPHitBox cO, LPHitBox cOOther);
	void CalcPotentialCollisions(LPGameObject cO, std::vector<LPGameObject>* coObjects, std::vector<LPCollisionEvent>& coEvents);
	void FilterCollision(
		std::vector<LPCollisionEvent>& coEvents,
		std::vector<LPCollisionEvent>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny);

	D3DXVECTOR2 GetVelocity();
	void SetVelocity(D3DXVECTOR2 s);

	float GetGravity();
	void SetGravity(float gravity);

	float GetAcceleration();
	void SetAcceleration(float acc);

	D3DXVECTOR2 GetDragForce();
	void SetDragForce(D3DXVECTOR2 drag);

	D3DXVECTOR2 GetNormal();
	void SetNormal(D3DXVECTOR2 n);

	bool IsDynamic();
	void SetDynamic(bool isDynamic);

	bool IsTrigger();
	void SetTrigger(bool isTrigg);

	D3DXVECTOR2 GetBounceForce();
	void SetBounceForce(D3DXVECTOR2 bF);
};



struct CollisionEvent
{
	LPHitBox obj;
	float t, nx, ny;

	float dx, dy;

	CollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPHitBox obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCollisionEvent& a, LPCollisionEvent& b)
	{
		return a->t < b->t;
	}
};