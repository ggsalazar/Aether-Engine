#pragma once

class Collision {
public:
    static bool AABB(const FloatRect a, const FloatRect b) {
        return (a.position.x + a.size.x >= b.position.x and b.position.x + b.size.x >= a.position.x and
            a.position.y + a.size.y >= b.position.y and b.position.y + b.size.y >= a.position.y);
    }
};