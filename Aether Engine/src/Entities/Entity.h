#pragma once
#include <iostream>
#include "../Structure/Game.h"
#include "../Structure/Scene.h"
#include "../Utils/Util.h"
#include "../Utils/Collision.h"
#include "../Utils/Animation.h"
#include "../Utils/Input.h"
#include "../Utils/Enums.h"

using namespace std;

struct Structure {
    Game& game;
    sf::RenderWindow& window;
    //This allows the Scene of an object to be changed
    Scene* scene;
};

class Entity : public enable_shared_from_this<Entity> {
    friend class Animation;
public:
    //Variables
    sf::Vector2f pos;
    sf::RectangleShape pos_debug;
    sf::FloatRect bbox;
    sf::RectangleShape bbox_debug;
    uint w = 0;
    uint h = 0;
    bool alive = true;
    int dfc = 0; //Distance from camera; draw order, basically - the lower the number, the closer to the camera

    //SFX Stuff
    sf::SoundBuffer sb;
    sf::Sound sound;

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