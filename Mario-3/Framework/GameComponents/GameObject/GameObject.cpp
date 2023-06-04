#include "GameObject.h"
#include "../Const.h"
#include "../../Ultis/Ultis.h"
#include "../Scene/SceneManager.h"

using namespace std;

CGameObject::CGameObject()
{
	this->currentState = "";
	isEnabled = false;
	ignoreTimeScale = false;
	currentState = "IDLE";
	isCheckWithCollision = true;
	nodeTag = NodeTag::None;
	isInGrid = false;
	isStatic = true;
}

D3DXVECTOR2 CGameObject::GetGameObjectSize(CGameObject* gO)
{
	D3DXVECTOR2 size = D3DXVECTOR2(0, 0);
	if (size.x == 0 || size.y == 0)
	{
		auto anim = gO->GetAnimationByState(gO->GetCurrentState());
		if (!anim) return size;
		auto animFrame = anim->GetAnimFrame();
		if (!animFrame) return size;
		auto sprite = animFrame->GetSprite();
		if (!sprite) return size;

		size.x = sprite->GetWidth();
		size.y = sprite->GetHeight();
	}
	return size;
}

bool CGameObject::IsDestroyed()
{
	return isDestroyed;
}

void CGameObject::SetDestroy(bool isDes)
{
	this->isDestroyed = isDes;
}

void CGameObject::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene->IsLoaded() == false)
		return;
}

void CGameObject::Render(CCamera* cam, int alpha)
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene->IsLoaded() == false)
		return;
	bool curState = animations.find(currentState) != animations.end();
	if (curState == false || animations.empty())
		return;
	animations.find(currentState)->second->SetScale(transform.scale);
	animations.find(currentState)->second->SetRotation(transform.rotationAngle);

	D3DXVECTOR2 posInCam = cam->Transform(transform.position + relativePositionOnScreen);
	posInCam.x = trunc(posInCam.x);
	posInCam.y = trunc(posInCam.y) + 18;
	animations.at(currentState)->Render(posInCam, alpha);
}

void CGameObject::FrictionProcess(float& speed, DWORD dt, bool isStop)
{
	if (speed > 0)
	{
		speed += -(isStop ? ACCELERATION_FRICTION * 2.5f : ACCELERATION_FRICTION) * dt;
		if (speed < 0)
			speed = 0;
	}
	if (speed < 0)
	{
		speed += (isStop ? ACCELERATION_FRICTION * 2.5f : ACCELERATION_FRICTION) * dt;
		if (speed > 0)
			speed = 0;
	}
}

std::string CGameObject::GetCurrentState()
{
	return currentState;
}

void CGameObject::AddAnimation(std::string stateName, LPAnimation animation, bool isLoop)
{
	animation->SetLoopAnimation(isLoop);
	animation->SetGameObject(this);
	animations.insert(make_pair(stateName, animation));
}

void CGameObject::SetRelativePositionOnScreen(D3DXVECTOR2 rP)
{
	this->relativePositionOnScreen = rP;
}

bool CGameObject::IsEnabled()
{
	return isEnabled;
}

void CGameObject::Enable(bool isEnabled)
{
	this->isEnabled = isEnabled;
}

bool CGameObject::IsIgnoreTimeScale()
{
	return ignoreTimeScale;
}

void CGameObject::SetIgnoreTimeSCale(bool isIgnoreTimeScale)
{
	this->ignoreTimeScale = isIgnoreTimeScale;
}

D3DXVECTOR2 CGameObject::GetScale()
{
	return transform.scale;
}

void CGameObject::SetScale(D3DXVECTOR2 s)
{
	this->transform.scale = s;
}

float CGameObject::GetRotation()
{
	return transform.rotationAngle;
}

void CGameObject::SetRotation(float r)
{
	this->transform.rotationAngle = r;
}

D3DXVECTOR2 CGameObject::GetPosition()
{
	return this->transform.position;
}

void CGameObject::SetPosition(D3DXVECTOR2 p)
{
	this->transform.position = p;
}

std::string CGameObject::GetState()
{
	return currentState;
}

void CGameObject::SetState(string state)
{
	if (animations.find(state) == animations.end())
		return;
	lastState = currentState;
	currentState = state;
	animations.at(state)->SetPlay(false);
}

bool CGameObject::IsStatic()
{
	return isStatic;
}

void CGameObject::SetStatic(bool setStatic)
{
	this->isStatic = setStatic;
}

bool CGameObject::IsInGrid()
{
	return isInGrid;
}

void CGameObject::SetInGrid(bool setInGrid)
{
	isInGrid = setInGrid;
}

Index CGameObject::GetIndex()
{
	return currentCellIndex;
}

void CGameObject::SetIndex(Index index)
{
	currentCellIndex = index;
}


LPAnimation CGameObject::GetAnimationByState(std::string state)
{
	if (animations.find(state) != animations.end())
		return animations.at(state);
	return NULL;
}