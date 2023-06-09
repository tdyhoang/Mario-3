﻿#include <Windows.h>
#include "Animation.h"
#include "../../Game.h"
#include <vector>
#include <string>
#include "../../../Ultis/Ultis.h"

using namespace std;

CAnimation::CAnimation(string aniID, DWORD defaultTime)
{
	id = aniID;
	currentFrame = -1;
	this->defaultFrameTime = defaultTime;
	transform.position = D3DXVECTOR2(0.0f, 0.0f);
	isLoop = true;
	isPlaying = true;
}

CAnimation::CAnimation(const CAnimation& obj)
{
	this->id = obj.id;
	this->defaultFrameTime = obj.defaultFrameTime;
	this->lastFrameTime = obj.lastFrameTime;
	this->currentFrame = -1;
	std::vector<LPAnimationFrame> newFrames(obj.animFrames);
	this->animFrames = newFrames;
	this->transform = transform.CreateTransformation(obj.transform.position, obj.transform.scale, obj.transform.rotationAngle);
	this->isLoop = obj.isLoop;
	this->isPlaying = obj.isPlaying;
}

void CAnimation::Add(LPSprite sprite, DWORD frameTime)
{
	int t = frameTime;
	if (frameTime <= 0) t = this->defaultFrameTime;

	LPAnimationFrame frame = new CAnimationFrame(sprite, t);
	animFrames.push_back(frame);
}

void CAnimation::Render(D3DXVECTOR2 position, int alpha)
{
	if (animFrames.size() == 0) return;

	DWORD now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
		if (CGame::GetTimeScale() != 0.0f || gameObject->IsIgnoreTimeScale())
		{
			DWORD t = animFrames[currentFrame]->GetTime();
			auto timeScale = (gameObject->IsIgnoreTimeScale() == false) ? CGame::GetTimeScale() : 1.0f;
			t = t / timeScale;
			if (now - lastFrameTime > t)
			{
				if (currentFrame == animFrames.size() - 1 && isLoop == false)
				{
					gameObject->EndAnimation();
					isPlaying = false;

				}
				else if (isPlaying == true)
				{
					currentFrame++;
					lastFrameTime = now;
					if (currentFrame >= animFrames.size()) currentFrame = 0;
				}
			}
		}
	if (animFrames[currentFrame]->GetSprite() != NULL)
		animFrames[currentFrame]->GetSprite()->Draw(position, transform.scale, transform.rotationAngle, D3DXCOLOR(255, 255, 255, alpha));
}

void CAnimation::SetPlay(bool isPause)
{
	if (isPause == false && isPlaying == false)
		currentFrame = -1;
	isPlaying = true;
}

LPAnimationFrame CAnimation::GetAnimFrame()
{
	if (currentFrame != -1)
		return animFrames[currentFrame];
	return NULL;
}

void CAnimation::ResetAnimation()
{
	DebugOut(L"RESET ANIMATION \n");
	currentFrame = -1;
	isPlaying = true;
}