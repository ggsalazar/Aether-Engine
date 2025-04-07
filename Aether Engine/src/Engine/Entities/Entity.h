#pragma once
#include <iostream>
#include "../Core/Enums.h"
#include "../Core/Vector2.h"
#include "../Graphics/Animation.h"

using namespace std;

class Game;
class Scene;

struct Structure {
    Game& game;
    RenderWindow& window;
    //This allows the Scene of an object to be changed
    Scene* scene;
};

class Entity : public enable_shared_from_this<Entity> {
    friend class Animation;
public:
    //Variables
    Vector2f pos;
    RectangleShape pos_debug;
    FloatRect bbox;
    RectangleShape bbox_debug;
    uint w = 0;
    uint h = 0;
    bool alive = true;
    int dfc = 0; //Distance from camera; draw order, basically - the lower the number, the closer to the camera

    //SFX Stuff
    SoundBuffer sb;
    Sound sound;

                 //Structure MUST be strictly inilitiazed, but this should have been the case anyway so it's a non-issue
    Entity(const Structure& s, const AnimInfo& a_i, const Animation::Transform& t = {}, int init_dfc = 0);
    virtual ~Entity() = default;

    void SetBBox();

    virtual void GetInput() {}
    virtual void Update() {}
    virtual void Draw(const bool debug = false);
    virtual void Move();

    void PlaySound();

protected:
    Structure structure;

    unique_ptr<Animation> anim;
};