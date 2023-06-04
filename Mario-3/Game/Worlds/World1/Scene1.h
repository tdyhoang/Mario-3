#pragma once
#ifndef SCENE1_H
#define SCENE1_H

#include "../../../Framework/GameComponents/Scene/Scene.h"

class CScene1 : public CScene
{
public:
	CScene1();
	void Load();
	void Unload() override;
};

#endif