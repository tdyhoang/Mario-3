#pragma once
#ifndef SCENE1_H
#define SCENE1_H

#include "../../../Framework/GameComponents/Scene/Scene.h"
#include "../../GameObjects/Mario/Mario.h"

class CScene1 : public CScene
{
	CMario* mario;
public:
	CScene1();
	void Load();
	void Unload() override;
};

#endif