#pragma once

#include "../map/surface.h"
#include "../projectiles/player_projectile.h"
using namespace std;


class character : public sprite
{
protected:
    Vector2 pos;
    Vector2 size;
    Vector2 speed;
    int direction;
    STATE state;
    float health;
    level elevation_level;
    float gravity;
public:
    virtual void load() = 0;
    void set_health(float h);
    float get_health();
    Vector2 get_pos();
    Vector2 get_size();
    int get_direction();
    level get_elevation_level();

    void sprite_update();
    void draw()override;

    bool grounded_check(surface &m);
    bool side_crash_check(int update,surface &m);
    bool got_shot(projectile *p);

    void gravity_apply();
    void wall_fix(surface &m);
    void crash_fix(surface &m);

};