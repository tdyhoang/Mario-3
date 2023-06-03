#pragma once

#ifndef GAME_H
#define GAME_H

#include <d3dx9.h>
#include <d3d9.h>
#include <unordered_map>
#include <string>

class CGame
{
private:
	int displayWidth;
	int displayHeight;

	int fps;

	static CGame* instance;
	static float timeScale;
	static float deltaTime;

	HWND hWnd; // Window Handle

	LPDIRECT3D9 d3d = NULL; // Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL; // Direct3D device handle

	/*Draw*/
	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL; // Sprite helper libary

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> gameSource;


public:
	static CGame* GetInstance();
	void Init();
	void InitDirectX(HWND hWnd, int disWidth, int disHeight, int fps);

	void Run(); // while-loop game
	void End();

	void Draw(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));
	void Draw(D3DXVECTOR2 position, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha = 253);

	void DrawFlipX(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));
	void DrawFlipY(D3DXVECTOR2 position, D3DXVECTOR2 pointCenter, LPDIRECT3DTEXTURE9 texture, RECT rect, D3DXCOLOR transcolor = D3DCOLOR_XRGB(255, 0, 255));

	void Clean();
	void Render();
	void Update();

	int GetDisplayWidth() { return displayWidth; }
	int GetDisplayHeight() { return displayHeight; }


	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static float GetTimeScale() { return timeScale; }
	static void SetTimeScale(float time) { timeScale = time; }

	bool ImportGameSource();
	std::string GetFilePathByCategory(std::string category, std::string id);
};

#endif