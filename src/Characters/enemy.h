#pragma once
#include "player.h"
using namespace std;

class enemy : public character
{
private:
    string name;
    float player_got_hit_interval;
    Sound player_got_hit;
    Sound enemy_got_hit;
public:
    ~enemy(){
        for(auto &i : texture) UnloadTexture(i.second);
        UnloadSound(player_got_hit);
        UnloadSound(enemy_got_hit);
    }
    void load() override;
    void create(string s);
    void update(surface &m,player &p,vector<player_projectile> &bullet);
};