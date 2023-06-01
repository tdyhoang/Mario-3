#pragma once
#ifndef __GAME_GLOBAL__
#define __GAME_GLOBAL__
#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>

class GameGlobal
{
public:
    GameGlobal();
    ~GameGlobal();

    static HINSTANCE GetCurrentHINSTACE();
    static HWND getCurrentHWND();

    static void SetCurrentHINSTACE(HINSTANCE hInstance);
    static void SetCurrentHWND(HWND hWnd);

    static void SetCurrentSpriteHandler(LPD3DXSPRITE spriteHandler);
    static LPD3DXSPRITE GetCurrentSpriteHandler();

    static void SetCurrentDevice(LPDIRECT3DDEVICE9 device);
    static LPDIRECT3DDEVICE9 GetCurrentDevice();

    static void SetWidth(int width);
    static int GetWidth();

    static void SetHeight(int height);
    static int GetHeight();

    static LPDIRECT3DSURFACE9 backSurface;

    static bool isGameRunning;

private:
    static HINSTANCE mHInstance; //windows hInstance
    static HWND mHwnd; // hwnd
    static LPD3DXSPRITE mSpriteHandler; //spriteHanlder
    static int mWidth, mHeight; //size
    static LPDIRECT3DDEVICE9 mDevice; //device directX
};

#endif