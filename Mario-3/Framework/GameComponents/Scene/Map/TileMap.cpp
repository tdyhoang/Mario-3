#include "TileMap.h"
#include "../../Grid/Grid.h"
#include <iostream>
#include <map>
#include "../../Game.h"
#include "../../../Ultis/Ultis.h"
#include "../../Graphics/Texture/TextureManager.h"
#include "../../../../Game/GameObjects/Platforms/SolidBox.h"
#include "../../../../Game/GameObjects/Platforms/GhostPlatform.h"
#include "../../../../Game/GameObjects/Misc/QuestionBlock.h"
#include "../../../../Game/GameObjects/Misc/Pipe.h"
//#include "../../../../Game/GameObjects/Misc/EmptyBlock.h"

using namespace std;

const D3DXVECTOR2 translateConst = D3DXVECTOR2(24, 40);

CTileMap::CTileMap()
{
	tileWidth = 1;
	tileHeight = 1;
	width = 1;
	height = 1;
	foreground = NULL;
	grid = NULL;
}

CTileMap::CTileMap(int width, int height, int tileWidth, int tileHeight)
{
	this->width = width;
	this->height = height;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;
	foreground = NULL;
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
	{
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
			{
				for (Layer* layer : layers)
				{
					RenderLayer(layer, i, j, x, y);
				}
			}

		}
	}

}

CTileMap* CTileMap::LoadMap(std::string filePath, std::string fileMap, std::vector<LPGameObject>& listGameObjects, CGameObject* player, CScene* scene)
{
	currentGOIndex = Index({ -1, -1 });
	string fullPath = filePath + fileMap;
	if (tinyxml2::XMLDocument doc; doc.LoadFile(fullPath.c_str()) == tinyxml2::XML_SUCCESS)
	{
		OutputDebugString(L"Loading TMX \n");
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
				tinyxml2::XMLDocument tsdoc;
				auto tspath = filePath + element->Attribute("source");
				if (tsdoc.LoadFile(tspath.c_str()) == tinyxml2::XML_SUCCESS)
					if (auto* tsroot = tsdoc.RootElement(); tsroot != nullptr)
					{
						TileSet* tileSet = new TileSet();
						tsroot->QueryIntAttribute("firstgid", &tileSet->firstgid);
						tsroot->QueryFloatAttribute("tilewidth", &tileSet->tileSize.x);
						tsroot->QueryFloatAttribute("tileheight", &tileSet->tileSize.y);
						tsroot->QueryIntAttribute("tilecount", &tileSet->tileCount);
						tsroot->QueryIntAttribute("columns", &tileSet->columns);
						tsroot->QueryIntAttribute("margin", &tileSet->margin);
						tsroot->QueryIntAttribute("spacing", &tileSet->spacing);

						if (auto* imgDom = tsroot->FirstChildElement("image"); imgDom != nullptr)
						{
							string imgPath = imgDom->Attribute("source");
							imgPath = filePath + imgPath;
							tileSet->textureID = std::to_string(tileSet->firstgid);
							string transcolor = imgDom->Attribute("trans");
							int red = stoi(transcolor.substr(0, 2), nullptr, 16);
							int green = stoi(transcolor.substr(2, 2), nullptr, 16);
							int blue = stoi(transcolor.substr(4, 2), nullptr, 16);
							CTextureManager::GetInstance()->Add(tileSet->textureID, imgPath, D3DCOLOR_ARGB(0, red, green, blue));
							tileSet->texture = CTextureManager::GetInstance()->GetTexture(std::to_string(tileSet->firstgid));
							tileSets[tileSet->firstgid] = tileSet;
						}
					}
			}

			for (auto* element = root->FirstChildElement("layer"); element != nullptr; element = element->NextSiblingElement("layer"))
			{
				std::string name = element->Attribute("name");
				auto layer = LoadLayer(element);
				if (layer != NULL)
				{
					if (name.compare("ForegroundBlock") == 0)
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
						if (properties != NULL)
						{
							for (auto* property = properties->FirstChildElement(); property != NULL; property = property->NextSiblingElement())
							{
								std::string propName = property->Attribute("name");
								if (propName.compare("cellx") == 0)
								{
									property->QueryIntAttribute("value", &cellX);
								}
								if (propName.compare("celly") == 0)
								{
									property->QueryIntAttribute("value", &cellY);
								}
							}
							currentGOIndex = Index({ cellX, cellY });
						}

					if (name.compare("Solid") == 0)
					{
						gameObject = LoadSolidBox(position, size, nameObject, listGameObjects);
					}
					else if (name.compare("Ghost") == 0)
					{
						gameObject = LoadGhostBox(position, size, nameObject, listGameObjects);
					}
					else if (name.compare("QuestionBlock") == 0)
					{
						object->QueryIntAttribute("type", &type);
						std::string questionBlockName = object->Attribute("name");
						gameObject = LoadQuestionBlock(position, type, questionBlockName, listGameObjects);
					}
					/*else if (name.compare("Block") == 0)
					{
						gameObject = LoadEmptyBlock(position, listGameObjects);
					}*/
					else if (name.compare("Pipe") == 0)
					{
						std::string direction = object->Attribute("type");
						gameObject = LoadPipe(position, size, direction, listGameObjects);
					}

					if (cellX != -1 && cellY != -1)
					{
						gameObject->SetIndex({ cellX, cellY });
					}
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
		tiles[i] = new int[layer->height + i * 3];
		for (int j = 0; j < layer->height; j++)
		{
			tiles[i][j] = stoi(splitted[i + static_cast<std::vector<std::string, std::allocator<std::string>>::size_type>(j) * layer->width]) + j * 3;
		}
	}
	layer->tiles = tiles;
	splitted.clear();
	return layer;
}

CGameObject* CTileMap::LoadSolidBox(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string name, std::vector<LPGameObject>& listGameObjects)
{
	CSolidBox* solid = new CSolidBox();
	solid->SetPosition(position - translateConst + size * 0.5);
	solid->GetHitBox()->at(0)->SetSizeBox(size);
	solid->GetHitBox()->at(0)->SetName(name);
	AddObjectToList(solid, listGameObjects);
	return solid;
}

CGameObject* CTileMap::LoadGhostBox(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string name, std::vector<LPGameObject>& listGameObjects)
{
	CGhostPlatform* ghostPlatform = new CGhostPlatform();
	ghostPlatform->SetPosition(position - translateConst + size * 0.5);
	ghostPlatform->GetHitBox()->at(0)->SetSizeBox(size);
	ghostPlatform->GetHitBox()->at(0)->SetName(name);
	AddObjectToList(ghostPlatform, listGameObjects);
	return ghostPlatform;
}

CGameObject* CTileMap::LoadQuestionBlock(D3DXVECTOR2 position, int type, std::string name, std::vector<LPGameObject>& listGameObjects)
{
	CQuestionBlock* solid = new CQuestionBlock();
	solid->SetPosition(position - translateConst);
	if (name.compare("coin") == 0)
	{
		solid->SetItemInfo({ ItemTag::Coin, type });
	}
	if (name.compare("powerup") == 0)
	{
		solid->SetItemInfo({ ItemTag::PowerUp, type });
	}
	solid->SetTarget(player);
	AddObjectToList(solid, listGameObjects);
	return solid;
}

CGameObject* CTileMap::LoadPipe(D3DXVECTOR2 position, D3DXVECTOR2 size, std::string direction, std::vector<LPGameObject>& listGameObjects)
{
	CPipe* pipe = new CPipe(size);
	auto pos = position + translateConst;
	pos.y -= 12;
	pipe->SetPosition(pos);

	int startX = 0, startY = 0;
	startX += 2;
	int sizeTile = 48;
	RECT headRect[2], bodyRect[2];
	if (direction.compare("up") == 0 || direction.compare("down") == 0)
	{
		startY += 2;

		headRect[0].left = startX * 48;
		headRect[0].top = startY * 48;
		headRect[0].right = headRect[0].left + sizeTile;
		headRect[0].bottom = headRect[0].top + sizeTile;

		headRect[1].left = (startX + 1) * 48;
		headRect[1].top = startY * 48;
		headRect[1].right = headRect[1].left + sizeTile;
		headRect[1].bottom = headRect[1].top + sizeTile;
		pipe->SetHeadRect(headRect[0], headRect[1]);

		bodyRect[0].left = startX * 48;
		bodyRect[0].top = (startY + 1) * 48;
		bodyRect[0].right = bodyRect[0].left + sizeTile;
		bodyRect[0].bottom = bodyRect[0].top + sizeTile;

		bodyRect[1].left = (startX + 1) * 48;
		bodyRect[1].top = (startY + 1) * 48;
		bodyRect[1].right = bodyRect[1].left + sizeTile;
		bodyRect[1].bottom = bodyRect[1].top + sizeTile;

		pipe->SetBodyRect(bodyRect[0], bodyRect[1]);

		if (direction.compare("up") == 0)
			pipe->SetDirection(PipeDirection::Up);
		else if (direction.compare("down") == 0)
			pipe->SetDirection(PipeDirection::Down);
	}
	else
	{
		headRect[0] = RECT{ startX * 48, startY * 48, startX * 48 + 48 , startY * 48 + 48 };
		headRect[1] = RECT{ startX * 48,  (startY + 1) * 48, startX * 48 + 48 ,  (startY + 1) * 48 + 48 };
		pipe->SetHeadRect(headRect[0], headRect[1]);

		bodyRect[0] = RECT{ (startX + 1) * 48, startY * 48,  (startX + 1) * 48 + 48 , startY * 48 + 48 };
		bodyRect[1] = RECT{ (startX + 1) * 48, (startY + 1) * 48,  (startX + 1) * 48 + 48 , (startY + 1) * 48 + 48 };
		pipe->SetBodyRect(bodyRect[0], bodyRect[1]);

	}
	AddObjectToList(pipe, listGameObjects);
	return pipe;
}

//CGameObject* CTileMap::LoadEmptyBlock(D3DXVECTOR2 position, std::vector<LPGameObject>& listGameObjects)
//{
//	CEmptyBlock* emptyBlock = new CEmptyBlock();
//	emptyBlock->SetPosition(position - translateConst);
//	AddObjectToList(emptyBlock, listGameObjects);
//	return emptyBlock;
//}

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

		if (scene->CheckGlobalObject(gameObject->GetTag()))
			scene->AddGlobalObject(gameObject);
		else
		{
			if (currentGOIndex.x != -1 && currentGOIndex.y != -1)
			{
				gameObject->SetIndex(currentGOIndex);
			}
			if (gameObject->IsInGrid() == false)
			{
				grid->Insert(gameObject);
				gameObject->SetInGrid(true);
			}
		}
	}
	else
	{
		auto gameObj = find(gameObjects.begin(), gameObjects.end(), gameObject);
		if (gameObj == gameObjects.end())
		{
			gameObjects.push_back(gameObject);
		}
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