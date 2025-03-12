#include "character.h"


Vector2 character::get_pos(){
    return pos;
}
Vector2 character::get_size(){
    return size;
}
int character::get_direction(){
    return direction;
}

level character::get_elevation_level(){
    return elevation_level;
}
void character::set_health(float h){
    health = h;
}
float character::get_health(){
    return health;
}
bool character::got_shot(projectile *p){
    if(!(p->active)) return false;
    Rectangle object_rect =  {pos.x,pos.y,size.x,size.y};
    Rectangle bullet_rect =  {p->pos.x,p->pos.y,p->size.x,p->size.y};
    if(CheckCollisionRecs(object_rect, bullet_rect)) return true;
    return false;    
}


bool character::grounded_check(surface &m){
    bool check = false;
    int platform_no; 
    if(!speed.y){
        Rectangle character_rect = {pos.x,pos.y+gravity,size.x,size.y};
        for(int i = 0;i<m.platform.size();i++){
            if(CheckCollisionRecs(character_rect,m.platform[i])){
                check = true;
                platform_no = i;
                break;
            }
        }
        switch (platform_no)
        {
        case 0:
            elevation_level = bottom;
            break;
        case 1:
            elevation_level = mid;
            break;
        case 2:
            elevation_level = upper_left;
            break;
        case 3:
            elevation_level = upper_right;
            break;        
        default:
            break;
        }
    }
    return check;
}

bool character::side_crash_check(int update,surface &m){
    bool check = 0;
    Rectangle player_rect_next = {pos.x+update,pos.y,size.x,size.y};
    Rectangle player_rect = {pos.x,pos.y,size.x,size.y};
    for(int i=0;i<m.platform.size();i++) if(CheckCollisionRecs(player_rect_next,m.platform[i]) && !CheckCollisionRecs(player_rect,m.platform[i])) check = true;
    return check;
}

void character::gravity_apply(){
    speed.y += gravity;
    pos.y += speed.y;
}

void character::draw(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    int frameWidth = texture[state].width / frame_count[state];
    Rectangle src_rec = {direction*frameWidth*frame[state],0,direction*frameWidth,texture[state].height};
    Rectangle des_rec = {pos.x,pos.y,size.x,size.y};
    Vector2 origin = { 0.0f, 0.0f };
    // Rectangle health_rec = {pos.x,pos.y- 10 *height/800,size.x * health/100,10 * height/800};
    // DrawRectangleRec(health_rec,RED);
    DrawTexturePro(texture[state],src_rec,des_rec,origin,0,WHITE);
}

void character::sprite_update(){
    current_time += GetFrameTime();
    if(current_time >= frame_time[state]){
        current_time = 0;
        if(direction == 1) frame[state]++;
        else frame[state]--;
        if (frame[state] >= frame_count[state]) frame[state] = 0;
        else if(frame[state] <= -1) frame[state] = frame_count[state]-1;
    }
}
void character::wall_fix(surface &m){
    //wall fix
    if(pos.x < (m.wall[0].x + m.wall[0].width)) pos.x = m.wall[0].x + m.wall[0].width + 1;
    if(pos.x > (m.wall[1].x-size.x)) pos.x = m.wall[1].x - size.x - 1;
}


void character::crash_fix(surface &m){


    //surface collision
    for(int i=0;i<4;i++){
        Rectangle player_rect = {pos.x,pos.y,size.x,size.y};
        if(CheckCollisionRecs(player_rect,m.platform[i])){
            if(speed.y>0){
                pos.y = m.platform[i].y-size.y;
                speed.y=0;
            }
            if(speed.y<0){
                pos.y = m.platform[i].y+m.platform[i].height;
                speed.y=0;
            }
        }
    }
}