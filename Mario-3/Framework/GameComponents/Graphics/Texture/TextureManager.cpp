#include "TextureManager.h"
#include "../../Game.h"
#include "../../Const.h"

using namespace std;

LPTexture CTextureManager::instance = NULL;


LPTexture CTextureManager::GetInstance()
{
	if (instance == NULL) instance = new CTextureManager();
	return instance;
}

void CTextureManager::Init()
{
	auto root = CGame::GetInstance();
	LoadTexture("tex-mario", root->GetFilePathByCategory("Texture", "tex-mario"));
	LoadTexture("tex-enemies", root->GetFilePathByCategory("Texture", "tex-enemies"));
}
	
void CTextureManager::Clear()
{
	for (auto& t : textures)
	{
		LPDIRECT3DTEXTURE9 tex = t.second;
		if (tex != NULL) tex->Release();
	}
	textures.clear();
}

void CTextureManager::ClearTextureById(std::string texname)
{
	auto tex = GetTexture(texname);
	if (tex != nullptr)
	{
		textures.erase(texname);
		tex->Release();
		tex = NULL;
	}
}

void CTextureManager::LoadTexture(string  texName, string texPath)
{
	Add(texName, texPath, D3DCOLOR_XRGB(255, 255, 255));
}

LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(std::string id)
{
	if (textures.find(id) == textures.end())
		return nullptr;
	else
		return textures.at(id);
}

void CTextureManager::Add(string id, std::string filePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(ToLPCWSTR(filePath), &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,
		ToLPCWSTR(filePath),
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	textures.insert(make_pair(id, texture));

	OutputDebugStringW(ToLPCWSTR("[INFO] Texture loaded Ok:  " + filePath + "\n"));
	OutputDebugStringW(ToLPCWSTR("[INFO] Texture ID: " + id + "\n"));
}