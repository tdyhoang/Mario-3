#include "SceneManager.h"
#include "../../Ultis/Ultis.h"
#include "../Game.h"
#include "../Const.h"

LPSceneManager CSceneManager::instance = NULL;

CSceneManager::CSceneManager()
{
	currentNodeID = -1;
}

LPSceneManager CSceneManager::GetInstance()
{
	if (instance == NULL) instance = new CSceneManager();
	return instance;
}

void CSceneManager::Init()
{
	auto filePath = CGame::GetInstance()->GetFilePathByCategory("Scene", "ui-camera");
	tinyxml2::XMLDocument sceneFile;
	if (sceneFile.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
	{
		DebugOut(L"[ERROR] Cannot load file ui-camera \n");
		return;
	}
	if (auto* root = sceneFile.RootElement(); root != nullptr)
		for (auto* ui = root->FirstChildElement(); ui != nullptr; ui = ui->NextSiblingElement())
		{
			string name = ui->Attribute("name");
			if (name.compare("UICamera") == 0)
			{
				DebugOut(L"[INFO] Load UI camera \n");
				int screenWidth = CGame::GetInstance()->GetScreenWidth();
				int screenHeight = CGame::GetInstance()->GetScreenHeight();

				D3DXVECTOR2 pos, posHUD;
				ui->QueryFloatAttribute("pos_x", &pos.x);
				ui->QueryFloatAttribute("pos_y", &pos.y);

				if (auto* uiCam = ui->FirstChildElement(); uiCam != nullptr)
				{
					std::string nameUICam = uiCam->Attribute("name");

					if (nameUICam.compare("HUD") == 0)
					{
						uiCam->QueryFloatAttribute("pos_x", &posHUD.x);
						uiCam->QueryFloatAttribute("pos_y", &posHUD.y);
					}
				}
			}
		}
}

void CSceneManager::Load(LPScene scene)
{
	if (loadedScenes.find(scene->GetSceneId()) != loadedScenes.end())
		loadedScenes.erase(scene->GetSceneId());
	scene->Load();
	loadedScenes.insert(make_pair(scene->GetSceneId(), scene));
	activeSceneId = scene->GetSceneId();
}

void CSceneManager::Unload(std::string sceneId)
{
	if (loadedScenes.find(sceneId) != loadedScenes.end())
		loadedScenes.at(sceneId)->Unload();
}

LPScene CSceneManager::GetScene(std::string id)
{
	return loadedScenes[id];
}

std::string CSceneManager::GetActiveSceneId()
{
	return activeSceneId;
}

LPScene CSceneManager::GetActiveScene()
{
	if (activeSceneId == "") return nullptr;
	if (loadedScenes.find(activeSceneId) != loadedScenes.end())
		return loadedScenes.at(activeSceneId);
	return nullptr;
}

int CSceneManager::GetNodeID()
{
	return currentNodeID;
}

void CSceneManager::SetNodeID(int id)
{
	this->currentNodeID = id;
}

void CSceneManager::LoadRequestScene()
{
	if (requestedLoadScene.size() > 0)
	{
		auto scene = requestedLoadScene.at(0);
		if (scene != NULL)
		{
			requestedLoadScene.erase(requestedLoadScene.begin());
			Load(scene);
		}
	}
}

void CSceneManager::UnloadRequestScene()
{
	if (requestedUnloadScene.size() > 0)
	{
		auto scene = requestedUnloadScene.at(0);
		if (scene != NULL)
		{
			requestedUnloadScene.erase(requestedUnloadScene.begin());
			Unload(scene->GetSceneId());
		}
	}
}

void CSceneManager::SwitchScene(LPScene scene)
{
	auto activeScene = GetActiveScene();
	if (activeScene == NULL)
		return;
	requestedUnloadScene.push_back(activeScene);
	requestedLoadScene.push_back(scene);
}

CSceneManager::~CSceneManager()
{
	for (auto& s : loadedScenes)
		delete s.second;
	loadedScenes.clear();
}