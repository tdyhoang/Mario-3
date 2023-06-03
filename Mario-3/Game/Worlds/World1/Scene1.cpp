#include "Scene1.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "../../../Framework/GameComponents/Game.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"

CScene1::CScene1()
{
	id = "PlayScene";
	filePath = CGame::GetInstance()->GetFilePathByCategory("Scene", "scene1-1");
}

void CScene1::Load()
{
	mario = new CMario();
	D3DXVECTOR2 pos;
	pos.x = 10;
	pos.y = 130;
	mario->SetPosition(pos);
	AddObject(mario);
	DebugOut(L"[INFO] End Load Scene 1 \n");
}

void CScene1::Unload()
{
	CScene::Unload();
}