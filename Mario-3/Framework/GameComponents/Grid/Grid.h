#pragma once
#include <vector>
#include "Cell.h"

class CCamera;
class CCell;

struct Index;
class CGrid
{
private:
	std::vector<std::vector<CCell*>> cells;
	int columns;
	int rows;
	std::vector <CCell*> activeCells;
public:
	CGrid(D3DXVECTOR2 mapSize);
	void Insert(CGameObject* gameObject);
	void Remove(CGameObject* gameObject);
	void Move(D3DXVECTOR2 oldPosition, CGameObject* gameObject);
	std::vector <CCell*>  FindActiveCells(CCamera* camera);
	std::vector <CGameObject*> FindActiveGameObjects(CCamera* camera);
	RECT GetRectByPosition(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	Index GetCellIndexByPosition(D3DXVECTOR2 pos);
	CCell* GetCell(Index index);
	CCell* GetCell(D3DXVECTOR2 position);
	~CGrid();
};