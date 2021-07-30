#include "Transform.h"

Vector2 VectorZero()
{
    return Vector2(0.0f, 0.0f);
}

Vector2 VectorUnit()
{
    return Vector2(1.0f, 1.0f);
}

Vector2 Multiply(float k, Vector2 vec)
{
    return Vector2(k * vec.x, k * vec.y);
}

Transform CreateTransform(Vector2 position, Vector2 scale, float rotation)
{
    Transform result;
    result.Position = position;
    result.Scale = scale;
    result.Rotation = rotation;
    return result;
}