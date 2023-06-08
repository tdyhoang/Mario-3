#pragma once
#include <d3dx9.h>
#include "../../GameObject/GameObject.h"
#include "../../../Ultis/RectFrame.h"

class CGameObject;
typedef CGameObject* LPGameObject;

struct CameraPropertySet;

class CCamera
{
protected:
	DWORD dt;
	float vx;
	D3DXVECTOR2 posCam;
	float widthCam;
	float heightCam;

	LPGameObject gameObject;
	bool isDisablePosX;
	bool isDisablePosY;
	bool isAutoX;
	RectFrame currentBoundary;
	std::unordered_map<int, CameraPropertySet> cameraPropertieSets;
public:
	CCamera(int wid, int hei);

	virtual void Update();

	D3DXVECTOR2 Transform(D3DXVECTOR2 posWorld);
	D3DXVECTOR2 TransformCamToWorld(D3DXVECTOR2 posInCam);
	bool CheckObjectInCamera(LPGameObject gO);
	bool CheckRectInCamera(RECT rect);

	int GetSpeedXCam();
	void SetSpeedXCam(float v);

	D3DXVECTOR2 GetPositionCam();
	virtual void SetPositionCam(D3DXVECTOR2 pos);

	float GetWidthCam();
	void SetWidthCam(float w);

	float GetHeightCam();
	void SetHeightCam(float h);

	RectFrame GetCurrentBoundary();
	void SetCurrentBoundary(RectFrame bound);

	CameraPropertySet GetCameraProperties(int id);
	void AddCameraProperties(int id, D3DXVECTOR2 pos, RectFrame boundary, bool isDisableX, bool isDiableY);
	void AddCameraProperties(int id, CameraPropertySet camProps);

	LPGameObject GetGameObject();
	void SetGameObject(LPGameObject gO);

	bool GetDisablePosX();
	void SetDisablePosX(bool isDisableX);

	bool GetDisablePosY();
	void SetDisablePosY(bool isDisableY);

	bool GetAutoX();
	void SetAutoX(bool isAutoX);
};

struct CameraPropertySet
{
	D3DXVECTOR2 camPosition;
	RectFrame boundarySet;
	bool disableX;
	bool disableY;
	static CameraPropertySet Empty()
	{
		D3DXVECTOR2 pos(-1, -1);
		RectFrame rect{};
		rect.left = -1;
		rect.right = -1;
		rect.top = -1;
		rect.bottom = -1;
		return CameraPropertySet{ pos, rect };
	}
	static bool IsEmpty(CameraPropertySet x)
	{
		if (x.boundarySet.bottom == -1 && x.boundarySet.left == -1 && x.boundarySet.top == -1 && x.boundarySet.right == -1)
			if (x.camPosition.x == -1 && x.camPosition.y == -1)
				return true;
		return false;
	}
};