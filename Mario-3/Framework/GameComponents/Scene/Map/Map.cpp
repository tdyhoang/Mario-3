#include "Map.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "../../../Ultis/Ultis.h"

using namespace std;

CMap::CMap(std::string filePath, std::string fileMap, CGameObject* player, CScene* scene)
{
	LoadTilemap(filePath, fileMap, player, scene);
}

CMap::~CMap()
{
	DebugOut(L"[INFO] Deleting map \n");
	delete tileMap;
}

void CMap::LoadTilemap(std::string filePath, std::string fileMap, CGameObject* player, CScene* scene)
{
	tileMap = new CTileMap();
	tileMap->LoadMap(filePath, fileMap, listGameObjects, player, scene);
}

void CMap::Update(CCamera* camera, DWORD dt)
{
	if (camera == NULL)
		return;
	camera->Update();
}

void CMap::Render(CCamera* camera, bool isRenderForeground)
{
	if (camera == NULL)
	{
		DebugOut(L"[ERROR] Camera is NULL \n");
		return;
	}

	tileMap->Render(camera, isRenderForeground);
}

std::vector<LPGameObject> CMap::GetListGameObjects()
{
	return listGameObjects;
}

CTileMap* CMap::GetTileMap()
{
	return tileMap;
}