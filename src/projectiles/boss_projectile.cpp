#include "boss_projectile.h"
int boss_projectile::last_projectile = 0;

int boss_projectile::get_last_projectile(){
    return last_projectile;
}

void boss_projectile::load(){
    texture[rest] = LoadTexture("assets/boss_projectile.png");
}

void boss_projectile::create_projectile()
{
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    pos = {-500,-500};
    size = {(float)120 * width/1600 ,(float)120 * height/800};
    speed = {0,0};
    net_velocity = 15 * sqrt((width*width + height*height)/(3200000));
    active = false;
    angle = 0;
    frame = {0};
    frame_time = {0.15};
    current_time = 0;
    frame_count = {10};
    last_projectile = 0;

}

void boss_projectile::shoot_projectile(Vector2 position,float angl){
    if(!active){
        active = true;
        speed = {net_velocity*cos(angl),net_velocity*sin(angl)};
        pos = position;
        angle = angl * 180/3.14159;
        last_projectile++;
        last_projectile%=BOSS_MAX_PROJECTILE;
    }
}

void boss_projectile::update_projectile(surface &m){}

void boss_projectile::update_projectile(player &p){
    if(active){
        pos.x += speed.x;
        pos.y += speed.y;

        //player got burned
        Rectangle proj_rect = {pos.x,pos.y,size.x,size.y};
        Rectangle player_rect = {p.get_pos().x,p.get_pos().y,p.get_size().x,p.get_size().y};
        if(CheckCollisionRecs(proj_rect,player_rect)) p.set_health(p.get_health() - boss_shoot_player);

        //checks whether projectile is outside of the screen or not
        bool check = CheckCollisionRecs(proj_rect,{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()});
        if(!check) active = false,frame[0] = 0;
        sprite_update();
    }
}

void boss_projectile::draw(){
    if(active){
        int frameWidth = texture[rest].width / frame_count[0];

        Rectangle proj_src = {frame[0]*frameWidth,0,frameWidth,texture[rest].height};
        Rectangle proj_des = {pos.x,pos.y,size.x,size.y};
        DrawTexturePro(texture[rest],proj_src,proj_des,(Vector2){size.x/2,size.y/2},angle,WHITE);
    } 
}