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
	std::string id;
	DWORD defaultFrameTime, lastFrameTime;
	int currentFrame;
	bool isLoop, isPlaying;
	Transform transform;
	D3DXVECTOR2 relativePosition;
	std::vector<LPAnimationFrame> animFrames;
	CGameObject* gameObject;

public:
	CAnimation(std::string aniID, DWORD defaultTime = 100);
	CAnimation(const CAnimation& obj);
	void Add(LPSprite sprite, DWORD frameTime = 0);
	void Render(D3DXVECTOR2 position, int alpha = 254);

	void SetPosition(D3DXVECTOR2 p) { this->transform.position = p; } // Relative Position
	void SetScale(D3DXVECTOR2 s) { this->transform.scale = s; }
	void SetRotation(float r) { this->transform.rotationAngle = r; }
	void SetLoopAnimation(bool isLoop) { this->isLoop = isLoop; }
	void SetRelativePosition(D3DXVECTOR2 rP) { this->relativePosition = rP; }
	void SetPlay(bool isPause);
	D3DXVECTOR2 GetPosition() { return transform.position; }
	D3DXVECTOR2 GetScale() { return transform.scale; }
	float GetRotation() { return transform.rotationAngle; }
	bool GetLoopAnimation() { return isLoop; }
	LPAnimationFrame GetAnimFrame();
	D3DXVECTOR2 GetRelativePosition() { return relativePosition; }

	void SetGameObject(LPGameObject gO) { gameObject = gO; }
	LPGameObject GetGameObject() { return gameObject; }
	void ResetAnimation();
};

#endif