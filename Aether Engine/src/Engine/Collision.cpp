#include "Collision.h"
#include "Math/Math.h"

bool Collision::AABB(const Rect& a, const Rect& b) {
    Rect a_norm = a;
    Rect b_norm = b;

    if (a_norm.w < 0) { a_norm.x += a.w; a_norm.w = -a.w; }
    if (a_norm.h < 0) { a_norm.y += a.h; a_norm.h = -a.h; }
    if (b_norm.w < 0) { b_norm.x += b.w; b_norm.w = -b.w; }
    if (b_norm.h < 0) { b_norm.y += b.h; b_norm.h = -b.h; }

    return (a_norm.x + a_norm.w >= b_norm.x && b_norm.x + b_norm.w >= a_norm.x &&
        a_norm.y + a_norm.h >= b_norm.y && b_norm.y + b_norm.h >= a_norm.y);
}

bool Collision::LinePoint(const Line& l, const Vec2i& p) {
    //Vertical line
    if (l.x1 == l.x2) return p.x == l.x1 and min(l.y1, l.y2) <= p.y and p.y <= max(l.y1, l.y2);
    //Horizontal line
    if (l.y1 == l.y2) return p.y == l.y1 and min(l.x1, l.x2) <= p.x and p.x <= max(l.x1, l.x2);

    bool on_line = (p.y - l.y1) * (l.x2 - l.x1) == (l.y2 - l.y1) * (p.x - l.x1);
    bool within_segment = min(l.x1, l.x2) <= p.x and p.x <= max(l.x1, l.x2) and
        min(l.y1, l.y2) <= p.y and p.y <= max(l.y1, l.y2);

    return on_line and within_segment;
}

bool Collision::CirclePoint(const Circle& c, const Vec2i& p) {
    return Distance({ c.x, c.y }, p) <= c.r;
}

bool Collision::RectPoint(const Rect& r, const Vec2i& p) {
    return min(r.x, r.x + r.w) <= p.x and p.x <= max(r.x, r.x + r.w) and
        min(r.y, r.y + r.h) <= p.y and p.y <= max(r.y, r.y + r.h);
}

bool Collision::RectCircle(const Rect& r, const Circle& c) {
    //Get the closest point of the rectangle to the circle
    Vec2i closest_point = { c.x, c.y };
    Math::Clamp(closest_point.x, r.x, r.x + r.w);
    Math::Clamp(closest_point.y, r.y, r.y + r.h);
    return CirclePoint(c, closest_point);
}
