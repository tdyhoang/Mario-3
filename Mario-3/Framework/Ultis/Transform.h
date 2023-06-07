#pragma once

#include <d3d9.h>
#include <d3dx9.h>
struct Transform
{
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	float rotationAngle;

	Transform CreateTransformation(D3DXVECTOR2 position, D3DXVECTOR2 scale, float rotationAngle)
	{
		Transform result;
		result.position = position;
		result.scale = scale;
		result.rotationAngle = rotationAngle;
		return result;
	}
};