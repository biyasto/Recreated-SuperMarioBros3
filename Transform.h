#pragma once
#include <d3d9.h>
#include <d3dx9.h>

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

class Transform
{
public:
	Vector2 Position;
	Vector2 Scale;
	float Rotation;
};

Vector2 VectorZero();

Vector2 VectorUnit();
Vector2 Multiply(float k, Vector2 vec);
Transform CreateTransform(Vector2 position, Vector2 scale, float rotation);

