#pragma once

#ifndef MARIO_H
#define MARIO_H

#include "../../../Framework/GameComponents/GameObject/GameObject.h"
#include "../../GameObjects/Mario/Mario.h"

class CMario : public CGameObject
{
public:
	CMario();
	void LoadAnimation();
	void Update();
	~CMario();
};
#endif