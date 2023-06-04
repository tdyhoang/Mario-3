#pragma once

#ifndef MAP_H
#define MAP_H

#include <string>
#include <map>

#include "../../Graphics/Sprite/Sprite.h"
#include "../Camera/Camera.h"
#include "../../GameObject/GameObject.h"
#include "TileMap.h"
#include "../Scene.h"

class CGameObject;
typedef CGameObject* LPGameObject;

class CCamera;
class CTileMap;
class CScene;

class CMap
{
private:
	CTileMap* tileMap;
	std::vector<LPGameObject> listGameObjects;
public:
	CMap(std::string filePath, std::string fileMap, CGameObject* player, CScene* scene);
	~CMap();
	void LoadTilemap(std::string filePath, std::string fileMap, CGameObject* player, CScene* scene);

	void Update(CCamera* camera, DWORD dt);
	void Render(CCamera* camera, bool isRenderForeground);

	std::vector<LPGameObject> GetListGameObjects();
	CTileMap* GetTileMap();
};

#endif