#include "Sprite.h"
#include "../../Game.h"

#include "../../../Ultis/Ultis.h"

CSprite::CSprite(std::string id, int xPivot, RECT rect, LPDIRECT3DTEXTURE9 tex, D3DXCOLOR transcolor)
{
	this->id = id;
	this->rect = rect;
	this->texture = tex;
	this->width = rect.right - rect.left;
	this->height = rect.bottom - rect.top;
	this->transcolor = transcolor;
	if (xPivot == -3)
		this->pointCenter = D3DXVECTOR2(width * 0.5, height * 0.5);
	else
		this->pointCenter = D3DXVECTOR2(xPivot, height * 0.5);
}

void CSprite::Draw(D3DXVECTOR2 position, D3DXVECTOR2 scale, float rotation, D3DXCOLOR transcolor)
{
	CGame* game = CGame::GetInstance();
	if (scale.x > 0)
		game->DrawFlipX(position, pointCenter, texture, rect, transcolor);
	else if (scale.y < 0)
		game->DrawFlipY(position, pointCenter, texture, rect, transcolor);
	else
		game->Draw(position, pointCenter, texture, rect, transcolor);
}

RECT CSprite::GetRect()
{
	return rect;
}

void CSprite::SetRect(RECT rect)
{
	this->rect = rect;
}

int CSprite::GetWidth()
{
	return width;
}

void CSprite::SetWidth(int w)
{
	width = w;
}

int CSprite::GetHeight()
{
	return height;
}

void CSprite::SetHeight(int h)
{
	height = h;
}

D3DXVECTOR2 CSprite::GetPointCenter()
{
	return pointCenter;
}

void CSprite::SetPointCenter(D3DXVECTOR2 pointCenter)
{
	this->pointCenter = pointCenter;
}

D3DXCOLOR CSprite::GetTranscolor()
{
	return transcolor;
}

void CSprite::SetTranscolor(D3DXCOLOR transcolor)
{
	this->transcolor = transcolor;
}