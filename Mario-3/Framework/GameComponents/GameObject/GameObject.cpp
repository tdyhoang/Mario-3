#include "GameObject.h"
#include "../Game.h"
#include "../Graphics/Texture/TextureManager.h"

using namespace std;

CGameObject::CGameObject()
{
	this->currentState = "";
}

void CGameObject::Render()
{
	DebugOut(L"[INFO] Render Game Object \n");
	OutputDebugString(ToLPCWSTR("[INFO] Current State:" + currentState + "\n"));

	bool curState = animations.find(currentState) != animations.end();
	if (curState == NULL || animations.empty())
	{
		DebugOut(L"Dont have Animation \n");
		return;
	}
	DebugOut(ToLPCWSTR("Position: " + std::to_string(position.x) + "\n"));
	animations.at(currentState)->Render(position);

}

void CGameObject::AddAnimation(std::string stateName, LPAnimation animation)
{
	animations.insert(make_pair(stateName, animation));
}

void CGameObject::SetState(string state)
{
	currentState = state;
}