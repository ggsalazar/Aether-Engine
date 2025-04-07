#include "Entity.h"

Entity::Entity(const Structure& s, const AnimInfo& a_i, const Animation::Transform& t, int init_dfc) :
    structure(s), pos(t.x, t.y),
    dfc(init_dfc), sound(sb), anim(make_unique<Animation>(structure.game, structure.window, this, a_i, t)) {

    SetBBox();
    bbox_debug.setFillColor(Color(0, 255, 0, 127)); //Green, 50% opacity
    pos_debug.setSize(Vector2f(structure.game.GetResScale(), structure.game.GetResScale()));
    pos_debug.setFillColor(Color(255, 0, 0, 127)); //Red, 50% opacity
}

void Entity::SetBBox() {
    pos_debug.setPosition(Vector2f(pos.x - structure.game.GetResScale()*.5, pos.y - structure.game.GetResScale()*.5));

    w = anim->GetSpriteW() * anim->sprite.getScale().x;
    h = anim->GetSpriteH() * anim->sprite.getScale().y;

    //position.x/y will always be left+top
    bbox.position.x = pos.x - anim->GetOrigin().x * w;
    bbox.position.y = pos.y - anim->GetOrigin().y * h;
    bbox.size.x = w;
    bbox.size.y = h;
    
    //bbox debug
    bbox_debug.setPosition(bbox.position);
    bbox_debug.setSize(bbox.size);
}

void Entity::Draw(const bool debug) {
    anim->Draw();

    if (debug) {
        structure.window.draw(bbox_debug);
        structure.window.draw(pos_debug);
    }
}

void Entity::Move() {
    //Ensure that the bbox and sprite of the entity is always matched to its actual, literal position
    pos_debug.setPosition(Vector2f(pos.x - 1, pos.y - 1));
    anim->sprite.setPosition(pos);
    float ori_x = anim->GetOrigin().x;
    float ori_y = anim->GetOrigin().y;
    bbox.position.x = pos.x - (ori_x * w);
    bbox.position.y = pos.y - (ori_y * h);
    bbox_debug.setPosition(bbox.position);
}

void Entity::PlaySound() {
    //Play assigned noise with slight pitch shift
    float pitch_shift = (rand() % 10) * .01;
    if (rand() % 2) pitch_shift *= -1;
    sound.setPitch(sound.getPitch() + pitch_shift);
    sound.play();
    sound.setPitch(1);
}
