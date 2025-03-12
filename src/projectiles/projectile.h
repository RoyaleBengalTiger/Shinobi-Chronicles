#pragma once
#include "../animation/animation.h"
#include "../map/surface.h"
using namespace std;
#define MAX_PROJECTILE 5
#define BOSS_MAX_PROJECTILE 10
#define BOSS_BURST_PROJECTILE  3

class projectile : public sprite
{
public:
    virtual void load() = 0;
    Vector2 pos;
    Vector2 size;
    Vector2 speed;
    bool active;
    int direction;
    void sprite_update();
    virtual void update_projectile(surface &m) = 0;
};