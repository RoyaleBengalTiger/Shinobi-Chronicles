#pragma once

#include "../../include/raylib.h"
#include "../../cpp_compiler\lib\gcc\x86_64-w64-mingw32\14.2.0\include\c++\x86_64-w64-mingw32\bits\stdc++.h"
#include "../enum/enums.h"
using namespace std;

class sprite
{
public:
    vector<int> frame;
    vector<float> frame_time;
    vector<int> frame_count;
    float current_time;
    float frame_angle;
    unordered_map<STATE,Texture2D> texture;
    virtual void sprite_update() = 0;
    virtual void draw() = 0;
};