#include "Cell.h"
#include "../Const.h"

using namespace std;

CCell::CCell(Index index)
{
	this->index = index;
}

void CCell::AddObject(CGameObject* gO)
{
	if (gameObjects.find(gO) == gameObjects.end())
		gameObjects.insert(gO);
}

void CCell::RemoveObject(CGameObject* gO)
{
	if (gameObjects.find(gO) != gameObjects.end())
		gameObjects.erase(gO);
}

std::unordered_set<CGameObject*> CCell::GetListGameObject()
{
	return gameObjects;
}

CCell::~CCell()
{
	for (auto gameObject : gameObjects)
	{
		delete gameObject;
		gameObject = NULL;
	}
	gameObjects.clear();
}