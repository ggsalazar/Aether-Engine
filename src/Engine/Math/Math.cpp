#include "Math.h"

float Math::Angle(const Vec2f p1, const Vec2f p2, const bool rad) {
    float angle = atan2(p2.y - p1.y, p2.x - p1.x);

    if (!rad) {
        angle *= 57.2958f;

        angle += 360 * (angle < 0);
    }

    return angle;
}

void Math::Clamp(float& value, const float min, const float max) {
    if (value < min) value = min;
    else if (value > max) value = max;
}

void Math::Clamp(int& val, const int min, const int max) {
    if (val < min) val = min;
    else if (val > max) val = max;
}

float Math::Lerp(const float a, const float b, const float t, const bool angle) {
    if (angle) {
        //Get the difference between the angles
        float diff = b - a;

        //Adjust the difference to ensure the intest path is taken
        if (diff > 180) diff -= 360;
        else if (diff < -180) diff += 360;

        return a + diff * t;
    }
    else
        return a + t * (b - a);
}