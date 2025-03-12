#pragma once

#include "../animation\animation.h"
using namespace std;

class surface
{
public:
    ~surface(){
        for(auto &it : platform_texture) UnloadTexture(it);
        for(auto &it : wall_texture) UnloadTexture(it);
    }
    vector<Rectangle> platform_Default;
    vector<Rectangle> platform;
    vector<Rectangle> wall_Default;
    vector<Rectangle> wall;
    vector<Texture2D> wall_texture;
    vector<Texture2D> platform_texture;
    bool active;
    void load();
    void create();
    void draw();
    void update();
};