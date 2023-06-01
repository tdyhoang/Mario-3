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

CAnimationManager::~CAnimationManager()
{

}

CAnimationManager::CAnimationManager()
{
}

bool CAnimationManager::Init()
{
	DebugOut(L"[INFO] Begin Animation Manager Init \n");
	auto root = CGame::GetInstance();
	// Generate animations here
	return true;
}

bool CAnimationManager::LoadAnimation(std::string texName, std::string filePath)
{
	// Load from XML
	OutputDebugStringW(ToLPCWSTR(filePath.c_str()));
	DebugOut(L"\n");

	tinyxml2::XMLDocument document;
	if (!document.LoadFile(filePath.c_str()))
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorStr()));
		return false;
	}

	tinyxml2::XMLElement* root = document.RootElement();

	for (tinyxml2::XMLElement* info = root->FirstChildElement(); info != nullptr; info = info->NextSiblingElement())
	{
		string gameObjectID = info->Attribute("gameObjectId");
		string textureID = info->Attribute("textureId");
		if (textureID != texName)
			continue;

		LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture(textureID);
		if (tex != nullptr)
			OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));
		else
			return false;

		OutputDebugStringW(ToLPCWSTR("Gameobject id: " + gameObjectID + '\n'));
		OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));

		for (tinyxml2::XMLElement* node = info->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
		{
			string aniId = node->Attribute("aniId");
			int frameTime;
			node->QueryIntAttribute("frameTime", &frameTime);
			string name = node->Attribute("name");
			OutputDebugStringW(ToLPCWSTR(aniId + ':' + to_string(frameTime) + ':' + name + '\n'));
			LPAnimation animation = new CAnimation(aniId, frameTime);

			// Sprite ref
			for (tinyxml2::XMLElement* sprNode = node->FirstChildElement(); sprNode != nullptr; sprNode = sprNode->NextSiblingElement())
			{
				string id = sprNode->Attribute("id");
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
	for (auto x : animations)
	{
		LPAnimation ani = x.second;
		delete ani;
	}
	animations.clear();
}