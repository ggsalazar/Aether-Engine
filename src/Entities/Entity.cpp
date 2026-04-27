#include "Entity.h"

//Just trust
Core* Entity::engine = nullptr;
Game* Entity::game = nullptr;

Entity::Entity(const Sprite::Info& spr_info) : sprite(spr_info) {
    //Default every Entity to the Objects layer
    if (sprite.GetDefaultLayer() == LayerName::NONE)
        sprite.SetDefaultLayer(LayerName::Objects);

    Entity::MoveTo(Vec2f(sprite.GetPos()));
}

void Entity::SetCore(Core* c, Game* g) {
    core = c;
    game = g;
    SEC = core->GetFPS();
}

void Entity::Move() {
    sprite.MoveTo(pos);
    size = sprite.GetSprSize();

    //bbox position will always be top left
    bbox.x = pos.x - floor(sprite.GetOrigin().x * size.x);
    bbox.y = pos.y - floor(sprite.GetOrigin().y * size.y);
    bbox.w = size.x;
    bbox.h = size.y;
}