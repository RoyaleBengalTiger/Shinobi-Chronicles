#include "player_projectile.h"

int player_projectile::last_projectile = 0;


int player_projectile::get_last_projectile(){
    return last_projectile;
}

void player_projectile::load(){
    texture[rest] = LoadTexture("assets/projectile.png");
}

void player_projectile::create_projectile()
{
    pos = {-500,-500};
    size = {(float)40 * GetScreenWidth()/1600 ,(float)17 * GetScreenHeight()/800};
    speed = {(float)30 * GetScreenWidth()/1600,0};
    active = false;
    direction = 1;
    last_projectile = 0;
}

void player_projectile::shoot_projectile(Vector2 position,int dir){
    if(!active){
        active = true;
        pos = position;
        direction = dir;
        last_projectile++;
        last_projectile%=MAX_PROJECTILE;
    }
}

void player_projectile::update_projectile(surface &m){

    if(active){
        pos.x += direction*speed.x;
        if(pos.x > GetScreenWidth() || pos.x < 0 - size.x) active = false;
        Rectangle rect = {pos.x,pos.y,size.x,size.y};
        for(int i=0;i<m.platform.size();i++) if(CheckCollisionRecs(m.platform[i],rect)) active = false;
    }
}

void player_projectile::draw(){
    if(active){
        Rectangle proj_src = {0,0,texture[rest].width*direction,texture[rest].height};
        Rectangle proj_des = {pos.x,pos.y,size.x,size.y};
        DrawTexturePro(texture[rest],proj_src,proj_des,(Vector2){0,0},0,WHITE);
    }
}