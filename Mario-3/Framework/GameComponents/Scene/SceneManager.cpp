#include "SceneManager.h"
#include "../../Ultis/Ultis.h"

LPSceneManager CSceneManager::instance = NULL;

LPSceneManager CSceneManager::GetInstance()
{
	if (instance == NULL) instance = new CSceneManager();
	return instance;
}

//Load Resource
void CSceneManager::Load(LPScene scene)
{
	DebugOut(L"[INFO] Begin Load Scene \n");
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
	OutputDebugString(ToLPCWSTR("[INFO] Active Scene: " + activeSceneId + "\n"));
	if (activeSceneId == "") return nullptr;
	if (loadedScenes.find(activeSceneId) != loadedScenes.end())
	{
		return loadedScenes.at(activeSceneId);
	}
	return nullptr;
}