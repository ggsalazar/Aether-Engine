#pragma once
#include "Geometry.h"

using std::min, std::max;

namespace Collision {
    bool AABB(const Rect& a, const Rect& b);

    bool LinePoint(const Line& l, const Vec2i& p);


    //bool Lines(const Line& a, const Line& b);

    bool CirclePoint(const Circle& c, const Vec2i& p);

    /*
    bool CircleLine(const Circle& c, const Vec2i& p) {
        return false;
    }

    bool Circles(const Circle& a, const Circle& b) {
        return false;
    }
    */

    bool RectPoint(const Rect& r, const Vec2i& p);


    //inline static bool RectLine(const Rect& r, const Line& l);

    bool RectCircle(const Rect& r, const Circle& c);
};