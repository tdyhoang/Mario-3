#pragma once

#include "../Camera/Camera.h"
#include "../../../../TinyXML/tinyxml2.h"
#include <map>
#include <vector>
#include "../../Graphics/Texture/TextureManager.h"
#include "../../Graph/Graph.h"
#include "../../Scene/Scene.h"
#include "../../../Ultis/Index.h"

struct TileSet;
struct Layer;
class CGraph;
class CObjectPool;
class CGrid;
class CScene;
struct Index;
class CTileMap
{
private:
	int width;
	int height;
	int tileWidth;
	int tileHeight;

	std::map<int, TileSet*> tileSets;
	std::vector<Layer*> layers;
	Layer* foreground;
	CGraph* graph;
	CGrid* grid;
	CGameObject* player;
	CScene* scene;
	std::unordered_map<int, std::unordered_map<int, CGameObject*>> objectGroups;
	Index currentGOIndex;

public:
	CTileMap();
	CTileMap(int width, int height, int tileWidth, int tileHeight);

	TileSet* GetTileSetByTileID(int id);
	CTileMap* LoadMap(std::string filePath, std::string fileMap, std::vector<LPGameObject>& listGameObjects, CGameObject* player, CScene* scene);
	Layer* LoadLayer(tinyxml2::XMLElement* layerElement);

	CGameObject* LoadSolidBox(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string name, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadGhostBox(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string name, std::vector<LPGameObject>& listGameObjects);

	CGameObject* LoadQuestionBlock(D3DXVECTOR2 position, int type, std::string name, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadPipe(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string direction, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadEmptyBlock(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects);

	void Render(CCamera* camera, bool isRenderForeground);
	void RenderLayer(Layer* layer, int i, int j, int x, int y);
	CGraph* GetGraph();

	void AddObjectToList(CGameObject* gO);
	void AddObjectToList(CGameObject* gO, std::vector<LPGameObject>& listGameObjects);
	CGrid* GetGrid();
	~CTileMap();
};

struct TileSet
{
	int firstgid;
	D3DXVECTOR2 tileSize;
	int tileCount;
	int columns;
	LPDIRECT3DTEXTURE9 texture;
	std::string textureID;
	int margin;
	int spacing;
	D3DCOLOR trans;
	void Clear()
	{
		CTextureManager::GetInstance()->ClearTextureById(textureID);
	}
};

struct Layer
{
	int id;
	int width;
	int height;
	int** tiles;
	bool isVisible;
	void Clear()
	{
		for (int i = 0; i < width; i++)
		{
			delete[] tiles[i];
		}
		delete[] tiles;
	}
};