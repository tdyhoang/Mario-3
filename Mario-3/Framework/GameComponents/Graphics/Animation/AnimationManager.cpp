#include "AnimationManager.h"
#include "../../../Ultis/Ultis.h"
#include "../../../../TinyXML/tinyxml2.h"
#include "../Sprite/SpriteManager.h"
#include "../../Const.h"
#include "../../Game.h"
#include "../Texture/TextureManager.h"

using namespace std;

LPAnimationManager CAnimationManager::instance = NULL;

LPAnimationManager CAnimationManager::GetInstance()
{
	if (instance == NULL) instance = new CAnimationManager();
	return instance;
}

void CAnimationManager::Init()
{
	DebugOut(L"[INFO] Begin Animation Manager Init \n");
	auto root = CGame::GetInstance();
	InitAnAnimationSet("tex-mario", root->GetFilePathByCategory("Animation", "ani-mario"));
}

bool CAnimationManager::InitAnAnimationSet(std::string textureName, std::string filePath)
{
	if (LoadAnimation(textureName, filePath) == false)
	{
		DebugOut(L"[ERROR] Cannot Load", textureName + "Animation File \n");
		return false;
	}
	return true;
}

bool CAnimationManager::LoadAnimation(std::string texName, std::string filePath)
{
	OutputDebugStringW(ToLPCWSTR(filePath.c_str()));
	DebugOut(L"\n");

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
	{
		OutputDebugStringW(ToLPCWSTR(doc.ErrorStr()));
		return false;
	}

	if (auto* root = doc.RootElement(); root != nullptr)
		for (auto* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
		{
			string gameObjectID = element->Attribute("gameObjectId");
			string textureID = element->Attribute("textureId");
			if (textureID != texName)
				continue;

			LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture(textureID);

			if (tex != nullptr)
				OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));
			else
				return false;

			OutputDebugStringW(ToLPCWSTR("Gameobject id: " + gameObjectID + '\n'));
			OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

			for (auto* node = element->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
			{
				string aniId = node->Attribute("aniId");
				int frameTime;
				node->QueryIntAttribute("frameTime", &frameTime);
				string name = node->Attribute("name");
				OutputDebugStringW(ToLPCWSTR(aniId + ':' + to_string(frameTime) + ':' + name + '\n'));
				LPAnimation animation = new CAnimation(aniId, frameTime);

				for (auto* subnode = node->FirstChildElement(); subnode != nullptr; subnode = subnode->NextSiblingElement())
				{
					string id = subnode->Attribute("id");
					LPSprite sprite = CSpriteManager::GetInstance()->Get(id);
					animation->Add(sprite, frameTime);
					OutputDebugStringW(ToLPCWSTR("|--" + id + ':' + to_string(frameTime) + '\n'));
				}

				AddAnimation(aniId, animation);
			}

			return true;
		}
}

void CAnimationManager::AddAnimation(std::string id, LPAnimation ani)
{
	animations.insert(make_pair(id, ani));
}

LPAnimation CAnimationManager::Get(std::string id)
{
	LPAnimation ani = animations.at(id);
	if (ani == NULL)
	{
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
		return nullptr;
	}
	return ani;
}

LPAnimation CAnimationManager::Clone(std::string id)
{
	LPAnimation oriAni = Get(id);
	LPAnimation copiedAni = new CAnimation(*oriAni);
	return copiedAni;
}


void CAnimationManager::Clear()
{
	for (auto& x : animations)
	{
		LPAnimation ani = x.second;
		delete ani;
	}
	animations.clear();
}