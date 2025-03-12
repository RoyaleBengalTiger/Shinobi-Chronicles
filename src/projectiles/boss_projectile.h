#pragma once
#include "projectile.h"
#include "../Characters/player.h"

using namespace std;


class boss_projectile : public projectile
{
private:
    float net_velocity;
    static int last_projectile;
    float angle;
public:
    ~boss_projectile(){
        UnloadTexture(texture[rest]);
    }
    void load();
    int get_last_projectile();
    void  create_projectile();
    void update_projectile(surface &m);
    void update_projectile(player &p);
    void draw();
    void shoot_projectile(Vector2 position,float angl);
};