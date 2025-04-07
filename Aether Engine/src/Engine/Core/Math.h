#pragma once
#include <iostream>
#include "Vector2.h"

namespace Math {
    void Clamp(float& value, const float min, const float max);
    float Lerp(const float a, const float b, const float t, const bool angle = false);
    Vector2f Lerp(const Vector2f a, const Vector2f b, const float t) { return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t) }; }
};