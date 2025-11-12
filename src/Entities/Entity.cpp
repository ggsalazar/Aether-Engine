#include "Entity.h"

//Just trust
Engine* Entity::engine = nullptr;
Game* Entity::game = nullptr;

Entity::Entity(const Sprite::Info& spr_info) : sprite(spr_info) {
    Entity::MoveTo(Vec2f(sprite.GetPos()));
}

void Entity::SetEngine(Engine* e, Game* g) {
    engine = e;
    game = g;
    SEC = engine->GetFPS();
}

template<typename T>
void Entity::MoveBy(const Vec2<T>& offset) {
    pos += Round(offset);

    Entity::Move();
}

template<typename T>
void Entity::MoveTo(const Vec2<T>& new_pos) {
    pos = Round(new_pos);

    Entity::Move();
}

void Entity::Move() {
    sprite.MoveTo(pos);
    size = sprite.GetScaledSize();

    //bbox position will always be top left
    bbox.x = pos.x - floor(sprite.GetOrigin().x * size.x);
    bbox.y = pos.y - floor(sprite.GetOrigin().y * size.y);
    bbox.w = size.x;
    bbox.h = size.y;
}