#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>
#include "../Graphics/Animation/Animation.h"


class CGameObject;
typedef CGameObject* LPGameObject;

class CAnimation;
typedef CAnimation* LPAnimation;


class CGameObject
{
private:
	int id;
	DWORD dt;

	D3DXVECTOR2 position;
	D3DXVECTOR2 velocity;
	D3DXVECTOR2 distance; // delta distance since last frame
	D3DXVECTOR2 normal; // nx, ny

	//STATE?
	std::string currentState;

	std::unordered_map<std::string, LPAnimation> animations;
public:
	CGameObject();

	virtual void Render();

	void AddAnimation(std::string stateName, LPAnimation animation);

	void SetPosition(D3DXVECTOR2 p) { this->position = p; }
	void SetSpeed(D3DXVECTOR2 v) { this->velocity = v; }
	D3DXVECTOR2 GetPosition() { return position; }
	D3DXVECTOR2 GetSpeed() { return velocity; }
	void SetState(std::string state);

};

#endif