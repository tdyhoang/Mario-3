#pragma once

#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

#include "AnimationFrame.h"
#include "../Sprite/Sprite.h"
#include "../../../Ultis/Transform.h"
#include "../../GameObject/GameObject.h"

class CGameObject;
typedef CGameObject* LPGameObject;

class CAnimation;
typedef CAnimation* LPAnimation;

class CAnimation
{
private:
	CGameObject* gameObject;
	std::string id;
	std::vector<LPAnimationFrame> animFrames;
	DWORD defaultFrameTime, lastFrameTime;
	int currentFrame;
	Transform transform;
	D3DXVECTOR2 relativePosition;
	bool isLoop, isPlaying;

public:
	CAnimation(std::string aniID, DWORD defaultTime = 100);
	CAnimation(const CAnimation& obj);

	void Add(LPSprite sprite, DWORD frameTime = 0);
	void Render(D3DXVECTOR2 position, int alpha = 254);
	void ResetAnimation();

	void SetGameObject(LPGameObject gO) { gameObject = gO; }
	void SetLoopAnimation(bool isLoop) { this->isLoop = isLoop; }
	void SetRelativePosition(D3DXVECTOR2 rP) { this->relativePosition = rP; }
	void SetPosition(D3DXVECTOR2 p) { this->transform.position = p; }
	void SetScale(D3DXVECTOR2 s) { this->transform.scale = s; }
	void SetRotation(float r) { this->transform.rotationAngle = r; }
	void SetPlay(bool isPause);

	LPGameObject GetGameObject() { return gameObject; }
	bool GetLoopAnimation() { return isLoop; }
	D3DXVECTOR2 GetRelativePosition() { return relativePosition; }
	D3DXVECTOR2 GetPosition() { return transform.position; }
	D3DXVECTOR2 GetScale() { return transform.scale; }
	float GetRotation() { return transform.rotationAngle; }
	LPAnimationFrame GetAnimFrame();
};

#endif