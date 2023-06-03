#include "Game.h"
#include "Const.h"
#include "Graphics/Texture/TextureManager.h"
#include "Graphics/Sprite/SpriteManager.h"
#include "Scene/SceneManager.h"
#include "Graphics/Animation/AnimationManager.h"
#include "../Ultis/Ultis.h"
#include "../../TinyXML/tinyxml2.h"
#include "../../Game/Worlds/World1/Scene1.h"

CGame* CGame::instance = NULL;
float CGame::deltaTime = 0.0f;
float CGame::timeScale = 1.0f;

CGame* CGame::GetInstance()
{
	if (instance == NULL) instance = new CGame();
	return instance;
}

void CGame::Init()
{
	DebugOut(L"[INFO] Begin Init Manager \n");

	fps = 60;

	CTextureManager::GetInstance()->Init();
	CSpriteManager::GetInstance()->Init();
	CAnimationManager::GetInstance()->Init();

	CScene1* scene1 = new CScene1();
	CSceneManager::GetInstance()->Load(scene1);

	DebugOut(L"[INFO] End Init Manager \n");
}

void CGame::InitDirectX(HWND hWnd, int disWidth, int disHeight, int fps)
{
	this->fps = fps;
	this->hWnd = hWnd;
	this->displayWidth = disWidth;
	this->displayHeight = disHeight;
	DebugOut(L"[INFO] Begin Init DirectX \n");
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = displayWidth = disWidth;
	d3dpp.BackBufferHeight = displayHeight = disHeight;
	d3dpp.Flags = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = NULL;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.hDeviceWindow = hWnd;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);


	D3DXCreateSprite(d3ddv, &spriteHandler);

	if (!spriteHandler)
	{
		MessageBox(hWnd, L"Creating sprite handler failed!", L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	DebugOut(L"[INFO] End Init DirectX \n");
}

void CGame::Run()
{
	MSG msg;
	bool done = false;

	DWORD frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / fps;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DWORD currentTime = GetTickCount64();
			deltaTime = currentTime - frameStart;

			if (deltaTime >= tickPerFrame)
			{
				frameStart = currentTime;
				Update();
				Render();
				Clean();

				if (deltaTime > tickPerFrame) deltaTime = 0;
			}
			else
			{
				Sleep(tickPerFrame - deltaTime);
			}
		}

	}
}

void CGame::End()
{
	DebugOut(L"[INFO] Game Ending \n");

	CSceneManager::GetInstance()->GetActiveScene()->Unload();
	CTextureManager::GetInstance()->Clear();
	CSpriteManager::GetInstance()->Clear();
	CAnimationManager::GetInstance()->Clear();

	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
	DebugOut(L"[INFO] Game Ended \n");
}

void CGame::Clean()
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	// Clean scene
}

void CGame::Draw(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor)
{
	D3DXVECTOR3 pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
	D3DXVECTOR3 pInt((int)(position.x), (int)(position.y), 0);
	spriteHandler->Draw(texture, &rect, &pCenter, &pInt, transcolor);
}

void CGame::Draw(D3DXVECTOR2 position, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha)
{
	D3DXVECTOR3 pInt((int)(position.x), (int)(position.y), 0);
	spriteHandler->Draw(texture, &rect, NULL, &pInt, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

void CGame::DrawFlipX(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor)
{
	D3DXVECTOR3 pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
	D3DXVECTOR2 pScale(-1, 1);
	D3DXVECTOR3 pInt((int)(position.x), (int)(position.y), 0);
	D3DXMATRIX oldMatrix, newMatrix;

	spriteHandler->GetTransform(&oldMatrix);

	D3DXMatrixTransformation2D(&newMatrix, &position, 0.0f, &pScale, NULL, 0.0f, NULL);
	spriteHandler->SetTransform(&newMatrix);

	spriteHandler->Draw(texture, &rect, &pCenter, &pInt, transcolor);
	spriteHandler->SetTransform(&oldMatrix);
}

void CGame::DrawFlipY(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor)
{
	D3DXVECTOR3 pCenter((int)pointCenter.x, (int)pointCenter.y, 0);
	D3DXVECTOR2 pScale(1, -1);
	D3DXVECTOR3 pInt((int)(position.x), (int)(position.y), 0);
	D3DXMATRIX oldMatrix, newMatrix;

	spriteHandler->GetTransform(&oldMatrix);

	D3DXMatrixTransformation2D(&newMatrix, &position, 0.0f, &pScale, NULL, 0.0f, NULL);
	spriteHandler->SetTransform(&newMatrix);

	spriteHandler->Draw(texture, &rect, &pCenter, &pInt, transcolor);
	spriteHandler->SetTransform(&oldMatrix);
}

void CGame::Render()
{
	D3DCOLOR bgColor = D3DCOLOR_XRGB(0, 0, 0);
	RECT rect = { 0, 0, 600, 600 };
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, bgColor, 1.0f, 0);

	d3ddv->BeginScene();
	spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	if (activeScene != nullptr)
		activeScene->Render();
	spriteHandler->End();

	d3ddv->EndScene();
	d3ddv->Present(NULL, NULL, NULL, NULL);

}

void CGame::Update()
{
	LPSceneManager sceneManger = CSceneManager::GetInstance();
	LPScene activeScene = sceneManger->GetActiveScene();
	if (activeScene != NULL)
		activeScene->Update(deltaTime);
}

bool CGame::ImportGameSource()
{
	auto path = "Resources/root.xml";
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(path) != tinyxml2::XML_SUCCESS)
	{
		OutputDebugStringW(ToLPCWSTR(doc.ErrorStr()));
		return false;
	}
	if (auto* root = doc.RootElement(); root != nullptr)
		for (auto* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
		{
			std::string category = element->Attribute("name");
			OutputDebugStringW(ToLPCWSTR(category + '\n'));

			std::unordered_map<std::string, std::string> bucket;

			for (auto item = element->FirstChildElement(); item != nullptr; item = item->NextSiblingElement())
			{
				std::string id = item->Attribute("id");
				std::string source = item->Attribute("source");
				bucket.insert(make_pair(id, source));
				OutputDebugStringW(ToLPCWSTR("|--" + id + ":" + source + '\n'));
			}
			gameSource.insert(make_pair(category, bucket));
		}
	return true;
}

std::string CGame::GetFilePathByCategory(std::string category, std::string id)
{
	if (gameSource.find(category) != gameSource.end())
	{
		auto& bucket = gameSource.at(category);
		if (bucket.find(id) != bucket.end())
		{
			return bucket.at(id);
		}
		return "";
	}
	return "";
}