#include "SpriteManager.h"
#include "../../Game.h"
#include "../../Const.h"
#include "../../../Ultis/Ultis.h"
#include "../../../../TinyXML/tinyxml2.h"
#include "../Texture/TextureManager.h"

using namespace std;

LPSpriteManager CSpriteManager::instance = NULL;

void CSpriteManager::Init()
{
	auto root = CGame::GetInstance();
	LoadSprite("tex-mario", root->GetFilePathByCategory("Sprite", "spr-mario"));
	LoadSprite("tex-misc", root->GetFilePathByCategory("Sprite", "spr-misc"));
	LoadSprite("tex-enemies", root->GetFilePathByCategory("Sprite", "spr-enemies"));
}

void CSpriteManager::Add(string id, RECT rect, LPDIRECT3DTEXTURE9 tex, int xPivot, D3DXCOLOR transcolor)
{
	LPSprite s = new CSprite(id, xPivot, rect, tex, transcolor);
	sprites.insert(make_pair(id, s));
}

bool CSpriteManager::LoadSprite(std::string texName, std::string filePath)
{
	DebugOut(L"[INFO] Begin Load Sprite \n");
	OutputDebugStringW(ToLPCWSTR(filePath.c_str()));
	DebugOut(L"\n");
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
	{
		OutputDebugStringW(ToLPCWSTR(doc.ErrorStr()));
		return false;
	}
	if (auto* root = doc.RootElement(); root != nullptr)
		for (auto* element = root->FirstChildElement(); element != nullptr; element->NextSiblingElement())
		{
			string textureID = element->Attribute("id");
			if (textureID != texName)
				continue;

			LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->GetTexture(textureID);

			if (tex != nullptr)
				OutputDebugStringW(ToLPCWSTR("Texture id: " + textureID + '\n'));
			else
				return false;
			for (auto* node = element->FirstChildElement(); node != nullptr; node = node->NextSiblingElement())
			{
				string spriteID = node->Attribute("id");
				int left, top, width, height, pivotX, transcolor = 0;
				D3DXCOLOR color;
				node->QueryIntAttribute("left", &left);
				node->QueryIntAttribute("top", &top);
				node->QueryIntAttribute("width", &width);
				node->QueryIntAttribute("height", &height);
				if (node->QueryIntAttribute("xPivot", &pivotX) != tinyxml2::XML_SUCCESS)
					pivotX = -1;
				pivotX *= 3;
				OutputDebugStringW(ToLPCWSTR(spriteID + ':' + to_string(left) + ':' + to_string(top) + ':' + to_string(width) + ':' + to_string(height) + ':' + to_string(pivotX) + '\n'));

				RECT rect{};
				rect.left = left * 3;
				rect.top = top * 3;
				rect.right = (left + width) * 3;
				rect.bottom = (top + height) * 3;

				Add(spriteID, rect, tex, pivotX);
			}
			return true;
		}
	return false;
}

LPSprite CSpriteManager::Get(string id)
{
	return sprites[id];
}

void CSpriteManager::Clear()
{
	for (auto& x : sprites)
	{
		LPSprite s = x.second;
		delete s;
	}

	sprites.clear();
}

LPSpriteManager CSpriteManager::GetInstance()
{
	if (instance == NULL) instance = new CSpriteManager();
	return instance;
}