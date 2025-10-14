#include "Entity.h"
#include "../Engine/Engine.h" //Game

//Just trust
Engine* Entity::engine = nullptr;
Game* Entity::game = nullptr;

Entity::Entity(const Sprite::Info s_i) : sprite(s_i) /*, sound(sb)*/ {
    Entity::MoveTo(Vec2f(sprite.GetPos()));
}

void Entity::SetEngine(Engine* e, Game* g) {
    engine = e;
    game = g;
    SEC = engine->GetFPS();
}

void Entity::Draw() {
    sprite.Draw();
}

void Entity::MoveBy(const Vec2f offset) {
    pos += Round(offset);

    Entity::Move();
}
void Entity::MoveBy(const Vec2i offset) {
	pos += offset;

	Entity::Move();
}

void Entity::MoveTo(const Vec2f new_pos) {
    pos = Round(new_pos);

    Entity::Move();
}
void Entity::MoveTo(const Vec2i new_pos) {
    pos = new_pos;

    Entity::Move();
}

void Entity::PlaySFX() {
    ///Play the sound
}

void Entity::PlaySFXPitchShift() {
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