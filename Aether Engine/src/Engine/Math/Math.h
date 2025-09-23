#pragma once
#include "Vec2.h" //iostream

namespace Math {
    void Clamp(float& value, const float min, const float max);
    void Clamp(int& val, const int min, const int max);
    float Lerp(const float a, const float b, const float t, const bool angle = false);
    inline Vec2f Lerp(const Vec2f a, const Vec2f b, const float t) { return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t) }; }
};