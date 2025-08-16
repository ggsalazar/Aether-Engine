#include "Entity.h"

#undef PlaySound

int Entity::SEC = 0;

Entity::Entity(const Sprite::Info s_i)
    : pos(s_i.pos) /*sound(sb)*/ {

    sprite.Init(s_i);
    Entity::MoveTo(Vec2f(pos));

    SEC = game->GetFPS();
}

void Entity::Draw() {
    game->renderer.DrawSprite(sprite);
}

void Entity::MoveBy(const Vec2f offset) {
    pos += Round(offset);

    Entity::Move();
}

void Entity::MoveTo(const Vec2f new_pos) {
    pos = Round(new_pos);

    Entity::Move();
}

void Entity::PlaySound() {
    //Play assigned noise with slight pitch shift
    float pitch_shift = (rand() % 10) * .01;
    if (rand() % 2) pitch_shift *= -1;
    /*
    sound.setPitch(sound.getPitch() + pitch_shift);
    sound.play();
    sound.setPitch(1);
    */
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
