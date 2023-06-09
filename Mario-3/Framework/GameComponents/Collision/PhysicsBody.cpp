#include "PhysicsBody.h"
#include "../Game.h"

#include <vector>
#include <minwindef.h>
#include <algorithm>
#include "../../Ultis/Ultis.h"

using namespace std;

CPhysicsBody::CPhysicsBody()	
{
	isTrigger = false;
	velocity.x = 0;
	velocity.y = 0;
	gravity = 0;
	acceleration = 0;
	dragForce.x = 0;
	dragForce.y = 0;
	normal.x = 1;
	normal.y = 1;
	bounceForce.x = 0;
	bounceForce.y = 0;
}

void CPhysicsBody::PhysicsUpdate(LPGameObject gameObject, std::vector<LPGameObject>* coObjects)
{
	auto cO = gameObject->GetHitBox()->at(0);
	if (gameObject == NULL || gameObject->IsEnabled() == false || isDynamic == false)
		return;


	auto dt = CGame::GetInstance()->GetDeltaTime();
	auto pos = gameObject->GetPosition();
	auto distance = cO->GetDistance();
	vector<CollisionEvent*> coEvents;
	vector<CollisionEvent*> coEventsResult;

	velocity.y += gravity * dt;

	coEvents.clear();

	CalcPotentialCollisions(gameObject, coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		pos.x += distance.x;
		pos.y += distance.y;
		gameObject->SetPosition(pos);
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		pos = gameObject->GetPosition();

		if (isTrigger == false)
		{
			pos.x += min_tx * distance.x + nx * 0.4f;
			pos.y += min_ty * distance.y + ny * 0.4f;
		}
		cO->CollisionHandle(dt, coEvents, this, velocity, min_tx, min_ty, nx, ny);

		if (nx != 0 || ny != 0)
		{
			gameObject->OnCollisionEnter(cO, coEventsResult);
		}
		gameObject->SetPosition(pos);
	}

	for (unsigned i = 0; i < coEvents.size(); i++) delete coEvents[i];
	coEvents.clear();
}
void CPhysicsBody::Update(LPGameObject gameObject)
{
	if (gameObject == NULL || gameObject->IsEnabled() == false)
		return;
	auto dt = CGame::GetInstance()->GetDeltaTime();
	auto hitBoxs = gameObject->GetHitBox();
	auto physicsBody = gameObject->GetPhysicsBody();

	D3DXVECTOR2 distance;
	distance.x = physicsBody->GetVelocity().x * dt;
	distance.y = physicsBody->GetVelocity().y * dt;
	hitBoxs->at(0)->SetDistance(distance);
}

bool CPhysicsBody::CheckAABB(RectFrame selfBox, RectFrame otherBox)
{
	return ((selfBox.left <= otherBox.right) && (selfBox.right >= otherBox.left) && (selfBox.top <= otherBox.bottom) && (selfBox.bottom >= otherBox.top)
		|| (selfBox.left <= otherBox.left) && (selfBox.right >= otherBox.right) && (selfBox.top <= otherBox.top) && (selfBox.bottom >= otherBox.bottom));
}

void CPhysicsBody::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny,
	ObjectTag tag)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;
	nx = ny = 0;

#pragma region Broad-phase Test

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;

#pragma endregion

#pragma region Sweep AABB Test
	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}

	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		tag == ObjectTag::GhostPlatform ? nx = 0.0f : (dx > 0 ? nx = -1.0f : nx = 1.0f);
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : (tag == ObjectTag::GhostPlatform ? ny = 0.0f : ny = 1.0f);
	}
#pragma endregion
}

LPCollisionEvent CPhysicsBody::SweptAABBEx(LPHitBox cO, LPHitBox cOOther)
{
	float sl, st, sr, sb;
	float ml, mt, mr, mb;
	float t, nx, ny;
	float svx, svy;
	auto phyBody = cOOther->GetGameObjectAttach()->GetPhysicsBody();

	svx = phyBody->GetVelocity().x;
	svy = phyBody->GetVelocity().y;
	float sdx = svx * CGame::GetInstance()->GetDeltaTime();
	float sdy = svy * CGame::GetInstance()->GetDeltaTime();

	float dx = cO->GetDistance().x - sdx;
	float dy = cO->GetDistance().y - sdy;

	auto boundingBox = cO->GetBoundingBox();
	ml = boundingBox.left;
	mt = boundingBox.top;
	mr = boundingBox.right;
	mb = boundingBox.bottom;

	auto boundingBoxOther = cOOther->GetBoundingBox();
	sl = boundingBoxOther.left;
	st = boundingBoxOther.top;
	sr = boundingBoxOther.right;
	sb = boundingBoxOther.bottom;

	std::string name = cOOther->GetName();
	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny, cOOther->GetGameObjectAttach()->GetTag()
	);

	CollisionEvent* e = new CollisionEvent(t, nx, ny, dx, dy, cOOther);
	return e;
}


void CPhysicsBody::CalcPotentialCollisions(
	LPGameObject gameObject,
	std::vector<LPGameObject>* goObjects,
	std::vector<LPCollisionEvent>& coEvents)
{
	std::vector<CollisionEvent*> temp;
	auto cO = gameObject->GetHitBox()->at(0);
	for (UINT i = 0; i < goObjects->size(); i++)
	{
		auto cOObjectCollisions = goObjects->at(i)->GetHitBox();
		if (cOObjectCollisions->size() <= 0)
			continue;

		auto coObject = cOObjectCollisions->at(0);
		auto coObjectGO = coObject->GetGameObjectAttach();
		if (cO->IsEnabled() == false || coObject->IsEnabled() == false)
			continue;
		if (coObjectGO->IsEnabled() == false && (coObjectGO->GetTag() != ObjectTag::Solid))
			continue;
		if (coObject == cO)
			continue;
		if (coObjectGO->GetTag() != ObjectTag::Solid && gameObject->GetTag() != ObjectTag::Solid)
		{
			if (CheckAABB(cO->GetBoundingBox(), coObject->GetBoundingBox()) == true || CheckAABB(coObject->GetBoundingBox(), cO->GetBoundingBox()) == true)
			{
				gameObject->OnOverlappedEnter(cO, coObject);
				coObjectGO->OnOverlappedEnter(coObject, cO);
				continue;
			}
		}
		if (gameObject->CanCollideWithThisObject(coObjectGO, coObjectGO->GetTag())
			== false)
			continue;

		LPCollisionEvent e = SweptAABBEx(cO, coObject);
		if (e->t > 0 && e->t <= 1.0f)
		{
			temp.push_back(e);
			if (coObjectGO->GetTag() == ObjectTag::Enemy || coObjectGO->GetTag() == ObjectTag::VenusFireBall)
			{
				SetTrigger(true);
			}
			else
			{
				SetTrigger(false);
			}
		}
		else
			delete e;
	}

	std::sort(temp.begin(), temp.end(), CollisionEvent::compare);
	for (auto latterCollision : temp)
	{
		for (auto prevCollision : coEvents)
		{
			D3DXVECTOR2 distance(cO->GetDistance());
			auto dt = CGame::GetInstance()->GetDeltaTime() * CGame::GetTimeScale();
			auto dist = distance - latterCollision->obj->GetGameObjectAttach()->GetPhysicsBody()->GetVelocity() * dt;

			if (latterCollision->nx != 0)
			{
				dist.y *= prevCollision->t;
				dist.y -= 0.1f;
			}
			else
			{
				dist.x *= prevCollision->t;
				dist.x -= 0.1f;
			}

			float time; D3DXVECTOR2 direction;
			auto mBox = cO->GetBoundingBox();
			auto sBox = latterCollision->obj->GetBoundingBox();
			SweptAABB(mBox.left, mBox.top, mBox.right, mBox.bottom, dist.x, dist.y, sBox.left, sBox.top, sBox.right, sBox.bottom, time, direction.x, direction.y, latterCollision->obj->GetGameObjectAttach()->GetTag());

			if (time <= 0 || time >= 1.0f)
			{
				latterCollision->t = 99999.0f;
				break;
			}
		}
		if (latterCollision->t > 0 && latterCollision->t <= 1.0f)
			coEvents.push_back(latterCollision);
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
}

void CPhysicsBody::FilterCollision(
	vector<LPCollisionEvent>& coEvents,
	vector<LPCollisionEvent>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCollisionEvent c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t;
			nx = c->nx;
			min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t;
			ny = c->ny;
			min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);

}

D3DXVECTOR2 CPhysicsBody::GetVelocity()
{
	return velocity;
}

float CPhysicsBody::GetGravity()
{
	return gravity;
}

float CPhysicsBody::GetAcceleration()
{
	return acceleration;
}

D3DXVECTOR2 CPhysicsBody::GetDragForce()
{
	return dragForce;
}

D3DXVECTOR2 CPhysicsBody::GetNormal()
{
	return normal;
}

bool CPhysicsBody::IsDynamic()
{
	return isDynamic;
}

void CPhysicsBody::SetVelocity(D3DXVECTOR2 s)
{
	velocity = s;
}

void CPhysicsBody::SetDynamic(bool isDynamic)
{
	this->isDynamic = isDynamic;
}

bool CPhysicsBody::IsTrigger()
{
	return isTrigger;
}

void CPhysicsBody::SetTrigger(bool isTrigg)
{
	this->isTrigger = isTrigg;
}

D3DXVECTOR2 CPhysicsBody::GetBounceForce()
{
	return bounceForce;
}

void CPhysicsBody::SetBounceForce(D3DXVECTOR2 bF)
{
	bounceForce = bF;
}

void CPhysicsBody::SetGravity(float gravity)
{
	this->gravity = gravity;
}

void CPhysicsBody::SetAcceleration(float acc)
{
	acceleration = acc;
}

void CPhysicsBody::SetDragForce(D3DXVECTOR2 drag)
{
	dragForce = drag;
}

void CPhysicsBody::SetNormal(D3DXVECTOR2 n)
{
	this->normal = n;
}