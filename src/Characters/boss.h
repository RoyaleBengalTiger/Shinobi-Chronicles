#pragma once
#include "player.h"
#include "../projectiles/boss_projectile.h"
using namespace std;


class boss : public character
{
private:
    float shoot_delay;
    Sound boss_shot_projectile;
    Sound player_got_clawed;
    Sound boss_got_hit;
    float player_got_hit_interval;
public:
    ~boss(){
        for(auto &i : texture) UnloadTexture(i.second);
        UnloadSound(player_got_clawed);
        UnloadSound(boss_shot_projectile);
        UnloadSound(boss_got_hit);
    }
    bool active;

    void load() override;
    void draw()override;
    void create();
    void update(surface &m,player &p,vector<player_projectile> &bullet,vector<boss_projectile> &fire);
};