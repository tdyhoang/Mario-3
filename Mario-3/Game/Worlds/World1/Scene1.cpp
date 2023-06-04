#include "Scene1.h"
#include "../../../Framework/Ultis/Ultis.h"
#include "../../../Framework/GameComponents/Const.h"
#include "../../../Framework/GameComponents/Game.h"
#include "../../../Framework/GameComponents/Scene/SceneManager.h"

CScene1::CScene1()
{
	this->id = "PlayScene";
	this->filePath = CGame::GetInstance()->GetFilePathByCategory("Scene", "scene1-1");
}

void CScene1::Load()
{
	CScene::Load();
	DebugOut(L"[INFO] Loaded Scene \n");
}

void CScene1::Unload()
{
	CScene::Unload();
}