#include "player.h"
#include "json.hpp"
using json = nlohmann::json;

int player::get_death(){
    return deaths;
}

void player::load(){
    texture[rest] = LoadTexture("assets/stance.png");
    texture[walking] = LoadTexture("assets/walk.png");
    texture[running] = LoadTexture("assets/run.png");
    texture[jumping_up] = LoadTexture("assets/jump_up.png");
    texture[jumping_down] = LoadTexture("assets/jump_down.png");
    texture[shooting] = LoadTexture("assets/shoot.png");
    life = LoadTexture("assets/life.png");
    shoot = LoadSound("assets/shoot.mp3");
}

void player::create(){
    float height = GetScreenHeight();
    float width = GetScreenWidth();
    json jsondata;
    ifstream file("player.json");
    file>>jsondata;
    file.close();
    pos = {width/2,height/2};
    size = {jsondata["size"][0].get<float>() * width/1600,jsondata["size"][1].get<float>() * height/800};
    speed = {jsondata["speed"][0].get<float>() * width/1600,jsondata["speed"][0].get<float>() * height/800};
    direction = jsondata["direction"].get<int>();
    state = jsondata["state"].get<STATE>();
    frame = jsondata["frame"].get<vector<int>>();
    frame_time  = jsondata["frame_time"].get<vector<float>>();
    frame_count = jsondata["frame_count"].get<vector<int>>();
    current_time = 0;
    elevation_level = jsondata["elevation_level"].get<level>();
    health = 100;
    gravity = height/800;

    shoot_time = frame_time[shooting]*frame_count[shooting];
    kills = 0;
    deaths = 0;
    score = 0;
}


void player::update(surface &m,vector<player_projectile> &p){
    int update =0;
    bool grounded_status = grounded_check(m);
    //for jump
    if((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) && grounded_status) speed.y -= 24*gravity , grounded_status = false;

    //for left right movement
    if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
        if(side_crash_check(-2*speed.x,m)) update = 0;
        else update = -speed.x;
        direction = -1;
        state = walking;
    }
    //for right movement
    else if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
        if(side_crash_check(2*speed.x,m)) update = 0;
        else update = speed.x;
        direction = 1;
        state = walking;
    }
    else state = rest;
    //for running
    if(state != rest && grounded_status && IsKeyDown(KEY_LEFT_SHIFT)) state = running;
    //movement update
    if(IsKeyDown(KEY_LEFT_SHIFT) && state != rest ) pos.x += 2*update;
    else pos.x += update;

    if(speed.y < 0) state = jumping_up;
    if(speed.y > 0) state = jumping_down;

    gravity_apply();
    wall_fix(m);
    crash_fix(m);



    if(IsKeyPressed(KEY_F)){
        shoot_time = 0;
        PlaySound(shoot);
        p[p[0].get_last_projectile()].shoot_projectile({pos.x+size.x*2/5,pos.y+size.y*2/5},direction);
    }
    if(shoot_time < frame_time[5]*frame_count[5]) state = shooting, shoot_time += GetFrameTime();

    if(health <= 0){
        health = 100;
        deaths++;
        pos = {(float)GetScreenWidth()/2,(float)GetScreenHeight()/2};
        elevation_level = mid;
    }

    sprite_update();
}

