#pragma once
#include "projectile.h"
#include "../map/surface.h"
using namespace std;


class player_projectile : public projectile
{
private:
    static int last_projectile;
public:
    ~player_projectile(){
        UnloadTexture(texture[rest]);
    }
    void load();
    int get_last_projectile();
    void  create_projectile();
    void update_projectile(surface &m);
    void draw();
    void shoot_projectile(Vector2 position,int dir);
};