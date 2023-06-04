#include "TileMap.h"
#include "../../Grid/Grid.h"

#include <iostream>
#include <map>
#include "../../Game.h"
#include "../../../Ultis/Ultis.h"
#include "../../Graphics/Texture/TextureManager.h"

using namespace std;

CTileMap::CTileMap()
{
	tileWidth = 1;
	tileHeight = 1;
	width = 1;
	height = 1;
	foreground = NULL;
	poolBricks = new CObjectPool();
	poolCoins = new CObjectPool();
	card = NULL;
	grid = NULL;
}

CTileMap::CTileMap(int width, int height, int tileWidth, int tileHeight)
{
	this->width = width;
	this->height = height;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;
	foreground = NULL;
	poolBricks = new CObjectPool();
	poolCoins = new CObjectPool();
	card = NULL;
	player = NULL;
}

TileSet* CTileMap::GetTileSetByTileID(int id)
{
	if (tileSets.size() <= 1)
		return (*tileSets.begin()).second;

	auto iterator = tileSets.lower_bound(id);

	if (iterator != tileSets.begin() && (*iterator).first != id)
		--iterator;

	return iterator->second;

}

void CTileMap::Render(CCamera* camera, bool isRenderForeground)
{
	int col = abs(camera->GetPositionCam().x / tileWidth);
	int row = abs(camera->GetPositionCam().y / tileHeight);

	D3DXVECTOR2 camSize = D3DXVECTOR2(camera->GetWidthCam() / tileWidth, camera->GetHeightCam() / tileHeight);

	for (int i = col; i < camSize.x + col + 4; i++)
		for (int j = row; j < camSize.y + row + 4; j++)
		{
			int x = i * tileWidth - camera->GetPositionCam().x;
			int y = j * tileHeight - camera->GetPositionCam().y;
			if (isRenderForeground == true)
			{
				if (foreground == NULL)
					continue;
				RenderLayer(foreground, i, j, x, y);
			}
			else
				for (Layer* layer : layers)
					RenderLayer(layer, i, j, x, y);
		}
}

CTileMap* CTileMap::LoadMap(std::string filePath, std::string fileMap, std::vector<LPGameObject>& listGameObjects, CGameObject* player, CScene* scene)
{
	currentGOIndex = Index({ -1, -1 });
	string fullPath = filePath + fileMap;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(fullPath.c_str()) == tinyxml2::XML_SUCCESS)
	{
		OutputDebugString(L"Begin Load TMX Map \n");
		if (auto* root = doc.RootElement(); root != nullptr)
		{
			graph = new CGraph();

			root->QueryIntAttribute("width", &width);
			root->QueryIntAttribute("height", &height);
			root->QueryIntAttribute("tilewidth", &tileWidth);
			root->QueryIntAttribute("tileheight", &tileHeight);

			this->grid = new CGrid(D3DXVECTOR2(width * tileWidth, height * tileHeight));
			this->player = player;
			this->scene = scene;
			
			for (auto* element = root->FirstChildElement("tileset"); element != nullptr; element = element->NextSiblingElement("tileset"))
			{
				TileSet* tileSet = new TileSet();
				element->QueryIntAttribute("firstgid", &tileSet->firstgid);
				element->QueryFloatAttribute("tilewidth", &tileSet->tileSize.x);
				element->QueryFloatAttribute("tileheight", &tileSet->tileSize.y);
				element->QueryIntAttribute("tilecount", &tileSet->tileCount);
				element->QueryIntAttribute("columns", &tileSet->columns);

				if (auto* imgDom = element->FirstChildElement("image"); imgDom != nullptr)
				{
					string imgPath = imgDom->Attribute("source");
					imgPath = filePath + imgPath;
					tileSet->textureID = std::to_string(tileSet->firstgid);
					CTextureManager::GetInstance()->Add(std::to_string(tileSet->firstgid), imgPath, D3DCOLOR_ARGB(0, 0, 0, 0));
					tileSet->texture = CTextureManager::GetInstance()->GetTexture(std::to_string(tileSet->firstgid));
					tileSets[tileSet->firstgid] = tileSet;
				}
			}

			for (auto* element = root->FirstChildElement("layer"); element != nullptr; element = element->NextSiblingElement("layer"))
			{
				std::string name = element->Attribute("name");
				auto layer = LoadLayer(element);
				if (layer != NULL)
				{
					if (name.compare("Foreground") == 0)
						foreground = layer;
					else
						layers.push_back(layer);
				}
			}

			for (auto* element = root->FirstChildElement("objectgroup"); element != nullptr; element = element->NextSiblingElement("objectgroup"))
			{
				std::string name = element->Attribute("name");
				int objectGroupdID;
				element->QueryIntAttribute("id", &objectGroupdID);

				CGameObject* gameObject = NULL;
				for (auto* object = element->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement("object"))
				{
					int id, x, y, width, height;
					int type = 0;
					int cellX = -1, cellY = -1;
					object->QueryIntAttribute("id", &id);
					object->QueryIntAttribute("x", &x);
					object->QueryIntAttribute("y", &y);
					object->QueryIntAttribute("width", &width);
					object->QueryIntAttribute("height", &height);


					D3DXVECTOR2 position = D3DXVECTOR2(x, y);
					D3DXVECTOR2 size = D3DXVECTOR2(width, height);
					string nameObject = std::to_string(id);

					if (auto* properties = object->FirstChildElement(); properties != nullptr)
					{
						for (auto* property = properties->FirstChildElement(); property != nullptr; property = property->NextSiblingElement())
						{
							std::string propName = property->Attribute("name");
							if (propName.compare("cellx") == 0)
								property->QueryIntAttribute("value", &cellX);
							if (propName.compare("celly") == 0)
								property->QueryIntAttribute("value", &cellY);
						}
						currentGOIndex = Index({ cellX, cellY });
					}

					if (cellX != -1 && cellY != -1)
						gameObject->SetIndex({ cellX, cellY });
				}
			}
		}
		if (listGameObjects.size() == 0 && scene->IsSpacePartitioning() == false)
		{
			DebugOut(L"[ERROR] Cannot load game objects \n");
			return nullptr;
		}
		return this;
	}
	DebugOut(L"[ERROR] Cannnot load file map \n");
	return nullptr;
}

Layer* CTileMap::LoadLayer(tinyxml2::XMLElement* element)
{
	Layer* layer = new Layer();

	int visible;
	if (element->QueryIntAttribute("visible", &visible) != tinyxml2::XML_SUCCESS) layer->isVisible = true;
	else layer->isVisible = visible;
	if (visible == 0)
	{
		delete layer;
		layer = NULL;
		return NULL;
	}
	element->QueryIntAttribute("id", &layer->id);
	element->QueryIntAttribute("width", &layer->width);
	element->QueryIntAttribute("height", &layer->height);

	auto tiles = new int* [layer->width];

	const char* content = element->FirstChildElement()->GetText();
	vector<string> splitted = split(content, ",");

	for (int i = 0; i < layer->width; i++)
	{
		tiles[i] = new int[layer->height];
		for (int j = 0; j < layer->height; j++)
			tiles[i][j] = stoi(splitted[i + static_cast<std::vector<std::string, std::allocator<std::string>>::size_type>(j) * layer->width]);
	}
	layer->tiles = tiles;
	splitted.clear();
	return layer;
}

void CTileMap::RenderLayer(Layer* layer, int i, int j, int x, int y)
{
	if (layer->isVisible == false || layer == NULL)
		return;
	int id = layer->tiles[i % width][j % height];
	auto tileSet = GetTileSetByTileID(id);
	auto firstGid = tileSet->firstgid;
	if (id >= firstGid)
	{
		auto columns = tileSet->columns;
		auto texture = tileSet->texture;
		auto tileSize = tileSet->tileSize;

		RECT r{};
		r.left = ((id - firstGid) % columns) * tileSize.x;
		r.top = ((static_cast<float>(id) - firstGid) / columns) * tileSize.y;
		r.right = r.left + tileSize.x;
		r.bottom = r.top + tileSize.y;
		CGame::GetInstance()->Draw(D3DXVECTOR2(x, y), D3DXVECTOR2(tileSize.x / 2, tileSize.y / 2), texture, r, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

CGraph* CTileMap::GetGraph()
{
	return graph;
}

std::vector<CGameObject*> CTileMap::GetBricks()
{
	return bricks;
}

std::vector<CGameObject*> CTileMap::GetCoins()
{
	return coins;
}

CObjectPool* CTileMap::GetPoolBricks()
{
	return poolBricks;
}

CObjectPool* CTileMap::GetPoolCoins()
{
	return poolCoins;
}

CGameObject* CTileMap::GetCard()
{
	return card;
}

void CTileMap::AddObjectToList(CGameObject* gO)
{
	grid->Insert(gO);
}

void CTileMap::AddObjectToList(CGameObject* gameObject, std::vector<LPGameObject>& gameObjects)
{
	if (gameObject == NULL)
		return;

	if (scene->IsSpacePartitioning() == true)
	{
		if (currentGOIndex.x != -1 && currentGOIndex.y != -1)
			gameObject->SetIndex(currentGOIndex);
		if (gameObject->IsInGrid() == false)
		{
			grid->Insert(gameObject);
			gameObject->SetInGrid(true);
		}
	}
	else
	{
		auto gameObj = find(gameObjects.begin(), gameObjects.end(), gameObject);
		if (gameObj == gameObjects.end())
			gameObjects.push_back(gameObject);
	}
}

CGrid* CTileMap::GetGrid()
{
	return grid;
}

CTileMap::~CTileMap()
{
	for (auto& tileS : tileSets)
	{
		tileS.second->Clear();
		delete tileS.second;
		tileS.second = NULL;
	}
	tileSets.clear();
	for (auto layer : layers)
	{
		layer->Clear();
		delete layer;
		layer = NULL;
	}
	layers.clear();
	delete foreground;
}