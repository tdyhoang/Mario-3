#include "HitBox.h"
#include "../../Ultis/Ultis.h"
#include "../Game.h"
#include "../Graphics/Texture/TextureManager.h"
#include "../Const.h"

CHitBox::CHitBox()
{
	distance.x = 0;
	distance.y = 0;
	localPosition.x = 0;
	localPosition.y = 0;
	name = "";
	id = -1;
	gameObject = NULL;
	sizeBox.x = 1;
	sizeBox.y = 1;
	isEnable = true;
}

void CHitBox::Render(CCamera* camera, int distance)
{
	auto pos = GetWorldPosition();

	auto tex = CTextureManager::GetInstance()->GetTexture("tex-bbox");
	RECT bbRect;
	bbRect.left = 0;
	bbRect.top = 0;
	bbRect.right = sizeBox.x;
	bbRect.bottom = sizeBox.y;

	D3DXVECTOR2 posInCam, camPos;
	camPos = camera->GetPositionCam();

	posInCam.x = trunc(pos.x - camPos.x);
	posInCam.y = trunc(pos.y - camPos.y + 20);

	CGame::GetInstance()->Draw(posInCam, D3DXVECTOR2(sizeBox.x * 0.5f, sizeBox.y * 0.5f), tex, bbRect, D3DCOLOR_ARGB(20, 255, 255, 255));
}

void CHitBox::CollisionHandle(DWORD dt, std::vector<CollisionEvent*>& collisions, LPPhysicsBody phyBody, D3DXVECTOR2 vel, int mintx, int minty, float nx, float ny)
{
	if (nx != 0)
	{
		vel.x = -1 * Sign(vel.x) * phyBody->GetBounceForce().x;
		distance.x = -1 * Sign(distance.x) * phyBody->GetBounceForce().x * dt;
		phyBody->SetVelocity(vel);
	}
	if (ny != 0)
	{
		vel.y = -1 * Sign(vel.y) * phyBody->GetBounceForce().y;
		distance.y = -1 * Sign(distance.y) * phyBody->GetBounceForce().y * dt;
		phyBody->SetVelocity(vel);
	}
}

void CHitBox::SetGameObjectAttach(LPGameObject gO)
{
	this->gameObject = gO;
}

LPGameObject CHitBox::GetGameObjectAttach()
{
	return gameObject;
}

void CHitBox::SetSizeBox(D3DXVECTOR2 size)
{
	sizeBox = size;
}

D3DXVECTOR2 CHitBox::GetSizeBox()
{
	return sizeBox;
}

void CHitBox::SetPosition(D3DXVECTOR2 pos)
{
	this->localPosition = pos;
}

D3DXVECTOR2 CHitBox::GetPosition()
{
	return localPosition;
}

void CHitBox::SetDistance(D3DXVECTOR2 d)
{
	distance = d;
}

D3DXVECTOR2 CHitBox::GetDistance()
{
	return distance;
}

D3DXVECTOR2 CHitBox::GetWorldPosition()
{
	auto worldPos = gameObject->GetPosition() + localPosition;
	return worldPos;
}

bool CHitBox::IsEnabled()
{
	if (gameObject != NULL)
	{
		return isEnable;
	}
	else
		return false;
}

void CHitBox::SetEnable(bool isEnable)
{
	this->isEnable = isEnable;
}

RectFrame CHitBox::GetBoundingBox()
{
	auto pos = GetWorldPosition();
	RectFrame r;
	r.left = pos.x - sizeBox.x * 0.5f;
	r.right = pos.x + sizeBox.x * 0.5f;
	r.top = pos.y - sizeBox.y * 0.5f;
	r.bottom = pos.y + sizeBox.y * 0.5f;
	return r;
}

void CHitBox::SetName(std::string n)
{
	name = n;
}

std::string CHitBox::GetName()
{
	return name;
}

void CHitBox::SetId(int i)
{
	id = i;
}

int CHitBox::GetID()
{
	return id;
}