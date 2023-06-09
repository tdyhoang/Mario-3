#pragma once

#include <Windows.h>
#include "../Sprite/Sprite.h"

class CAnimationFrame;
typedef CAnimationFrame* LPAnimationFrame;
class CAnimationFrame
{
private:
	LPSprite sprite;
	DWORD time;
public:
	CAnimationFrame(LPSprite sprite, DWORD time);
	LPSprite GetSprite();
	DWORD GetTime();
};