#pragma once
#include "../GameObject/GameObject.h"
#include "PhysicsBody.h"
#include "../../Ultis/RectFrame.h"

class CGameObject;
typedef CGameObject* LPGameObject;

class CPhysicsBody;
typedef CPhysicsBody* LPPhysicsBody;

class CCamera;

struct CollisionEvent;

class CHitBox
{
protected:
	LPGameObject gameObject;
	D3DXVECTOR2 sizeBox;
	D3DXVECTOR2 localPosition;
	D3DXVECTOR2 distance;
	std::string name;
	int id;
	bool isEnable;
public:
	CHitBox();
	void Render(CCamera* camera, int distance);
	virtual void CollisionHandle(DWORD dt, std::vector<CollisionEvent*>& collisions, LPPhysicsBody phyBody, D3DXVECTOR2 vel, int mintx, int minty, float nx, float ny);

	virtual void SetGameObjectAttach(LPGameObject gO);
	LPGameObject GetGameObjectAttach();

	void SetSizeBox(D3DXVECTOR2 size);
	D3DXVECTOR2 GetSizeBox();

	void SetPosition(D3DXVECTOR2 pos);
	D3DXVECTOR2 GetPosition();

	void SetDistance(D3DXVECTOR2 d);
	D3DXVECTOR2 GetDistance();

	D3DXVECTOR2 GetWorldPosition();

	bool IsEnabled();
	void SetEnable(bool isEnable);

	RectFrame GetBoundingBox();

	void SetName(std::string n);
	std::string GetName();

	void SetId(int i);
	int GetID();
};
typedef CHitBox* LPHitBox;