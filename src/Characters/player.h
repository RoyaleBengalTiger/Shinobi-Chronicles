#pragma once
#include "character.h"
#include "../projectiles/player_projectile.h"
using namespace std;


class player : public character
{
private:
    int deaths;
    float shoot_time;
    vector<player_projectile> Projectile;
    Sound shoot;
    
public:
    ~player(){
        for(auto &i : texture) UnloadTexture(i.second);
        UnloadSound(shoot);
    }
    Texture2D life;
    int kills;
    int score;
    int get_death();
    void load();
    void create();
    void update(surface &m,vector<player_projectile> &p);

};