#pragma once
#include "Vec2.h" //iostream

const float sqrt2 = 1.414f;
const float PI = 3.14159265359f;

namespace Math {
    float Angle(const Vec2f p1, const Vec2f p2, const bool rad = false);
    void Clamp(float& value, const float min, const float max);
    void Clamp(int& val, const int min, const int max);
    float Lerp(const float a, const float b, const float t, const bool angle = false);
    inline Vec2f Lerp(const Vec2f a, const Vec2f b, const float t) { return { Lerp(a.x, b.x, t), Lerp(a.y, b.y, t) }; }
};