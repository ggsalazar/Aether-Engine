#pragma once
#include <queue>
#include "Math/Vec2.h"

using std::vector, std::queue, std::priority_queue;

class Core;
class Entity;
class TileMap;

class Pathfinding {
    friend class Renderer;
public:
    struct Node {
        Vec2i pos = { 0, 0 };
        bool debug = false, walkable = false, occupied = false;
        //TO-DO: flyable, swimmable
        bool claimed = false; //True when a different creature has already claimed the goal node
        uchar cost = 1; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
        //For A* calculations:
        //g = currently known cost of cheapest known path from start->n, h is heuristic cost from n->goal, f = g + h, or
        // our current best guess as to how cheap a path could be from start to finish if it goes through n.
        float g = 0.f, h = 0.f, f = 0.f;

        Node* parent = nullptr;

        Node(const Vec2i i_pos = {}, const bool i_debug = false, const bool i_walkable = false, const uchar i_cost = 1) :
            pos(i_pos), debug(i_debug), walkable(i_walkable), cost(i_cost) {}

        //Compare struct, compares f values of Nodes for pathfinding (we want the lowest f value at the top of the open list)
        struct Compare {
            bool operator()(const Node* a, const Node* b) const {
                return a->f > b->f;
            }
        };
    };


    Pathfinding() = default;
    void Init(TileMap* t, vector<Entity*>* e);

    void UpdateNodeGrid();

    vector<Vec2i> FindPath(const Vec2i& start, Vec2i& goal, Entity* target = nullptr);

    bool LineOfSight(const Vec2i& from, const Vec2i& to) const;

private:
    vector<vector<Node>> grid;
    Vec2u grid_size;

    TileMap* tmp = nullptr;
    vector<Entity*>* entities = nullptr;

    vector<Vec2i> SmoothPath(const vector<Vec2i>& raw) const;
    float Heuristic(const Vec2i& a, const Vec2i& b);
};