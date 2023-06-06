#pragma once

#ifndef GAME_H
#define GAME_H

#include <d3dx9.h>
#include <d3d9.h>
#include <dinput.h>
#include <unordered_map>
#include "Const.h"
#include <string>

class CGame
{
private:
	int screenWidth;
	int screenHeight;

	int fps;

	static CGame* instance;
	static float timeScale;
	static DWORD deltaTime;

	HWND hWnd;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddv = NULL;

	/*Draw*/
	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> gameSource;

public:
	static CGame* GetInstance();
	void InitDirectX(HWND hWnd, int scrWidth, int scrHeight, int fps);
	void Draw(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));
	void Draw(D3DXVECTOR2 position, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha = 253);

	void DrawFlipX(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));
	void DrawFlipY(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));

	void Init();
	void Request();
	void Run();
	void End();
	void Clean();
	void Render();
	void Update();

	int GetScreenWidth() { return screenWidth; }
	int GetScreenHeight() { return screenHeight; }

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
	static float GetTimeScale() { return timeScale; }
	static void SetTimeScale(float time) { timeScale = time; }

	float GetFixedDeltaTime() { return 20; }
	DWORD GetDeltaTime() { return deltaTime; }

	bool ImportGameSource();
	std::string GetFilePathByCategory(std::string category, std::string id);
};

#endif