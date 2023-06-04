#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>

#include "../Graphics/Animation/Animation.h"
#include "../Scene/Camera/Camera.h"
#include "../Scene/Scene.h"
#include "../Graph/Node.h"
#include "../../Ultis/Index.h"

class CScene;
typedef CScene* LPScene;

class CGameObject;
typedef CGameObject* LPGameObject;

class CCamera;
typedef CCamera* LPCamera;

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
	std::unordered_map<std::string, LPAnimation> animations;
	bool isCheckWithCollision;
public:
	CGameObject();

	static D3DXVECTOR2 GetGameObjectSize(CGameObject* gO);
	bool IsDestroyed();
	void SetDestroy(bool isDes);

	virtual void Update(DWORD dt, CCamera* cam, CCamera* uiCam);
	virtual void Render(CCamera* cam, int alpha = 255);

	void FrictionProcess(float& speed, DWORD dt, bool isStop);

	std::string GetCurrentState();
	void AddAnimation(std::string stateName, LPAnimation animation, bool isLoop = true);
	void SetRelativePositionOnScreen(D3DXVECTOR2);
	LPAnimation GetAnimationByState(std::string state);

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

	std::string GetState();
	void SetState(std::string state);

	bool IsStatic();
	void SetStatic(bool setStatic);
	bool IsInGrid();
	void SetInGrid(bool setInGrid);

	Index GetIndex();
	void SetIndex(Index index);
};

#endif