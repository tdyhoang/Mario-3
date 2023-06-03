#pragma once

#ifndef SCENE_H
#define SCENE_H

#include "../GameObject/GameObject.h"

class CScene;
typedef CScene* LPScene;

class CScene
{
protected:
	std::vector<LPGameObject> gameObjects;
	std::string id;
	std::string filePath;
public:
	virtual void Load() = 0;
	virtual void Unload();

	virtual void Update(DWORD dt);
	virtual void Render();

	std::string GetSceneId() { return this->id; }

	void AddObject(LPGameObject gameObject);
	void RemoveObject(LPGameObject gameObject);
};
#endif
