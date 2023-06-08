#define WIN32_LEAN_AND_MEAN

#include <SDKDDKVer.h>

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "Framework/GameComponents/Const.h"
#include "Framework/GameComponents/Game.h"
#include "Framework/Ultis/Ultis.h"
#include "TinyXML/tinyxml2.h"

constexpr auto MAX_LOADSTRING = 100;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool                LoadFileConfig(int& fps, int& displayWidth, int& displayHeight);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    int fps, displayWidth, displayHeight;
    if (LoadFileConfig(fps, displayWidth, displayHeight) == false)
        return NULL;
    MyRegisterClass(hInstance);

    HWND hWnd = InitInstance(hInstance, nCmdShow);
    if (!hWnd)
    {
        return NULL;
    }

    CGame::GetInstance()->InitDirectX(hWnd, displayWidth, displayHeight, fps);
    CGame::GetInstance()->Init();
    CGame::GetInstance()->Run();

    return 0;
}

bool LoadFileConfig(int& fps, int& displayWidth, int& displayHeight)
{
    CGame::GetInstance()->ImportGameSource();
    auto configFilePath = CGame::GetInstance()->GetFilePathByCategory("Config", "global-config");

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(configFilePath.c_str()) != tinyxml2::XML_SUCCESS)
    {
        OutputDebugStringW(ToLPCWSTR(doc.ErrorStr()));
        return false;
    }
    if (auto* root = doc.RootElement(); root != nullptr)
        for (auto* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
        {
            std::string name = element->Attribute("name");
            if (name.compare("frame-rate") == 0)
                element->QueryIntAttribute("value", &fps);
            else if (name.compare("resolution") == 0)
            {
                element->QueryIntAttribute("width", &displayWidth);
                element->QueryIntAttribute("height", &displayHeight);
            }
        }
    DebugOut(L"conf: %d, %d, %d\n", fps, displayWidth, displayHeight);
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = TITLE;

    return RegisterClassExW(&wcex);
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(TITLE, TITLE, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, DISPLAY_WIDTH, DISPLAY_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return NULL;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        CGame::GetInstance()->End();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}