#include "enemy.h"
#include "json.hpp"
using json = nlohmann::json;


void enemy::load(){
    texture[walking] = LoadTexture("assets/enemy_walk.png");
    texture[jumping_up] = LoadTexture("assets/enemy_jump_up.png");
    texture[jumping_down] = LoadTexture("assets/enemy_jump_down.png");
    texture[shooting] = LoadTexture("assets/enemy_shoot.png");
    player_got_hit = LoadSound("assets/got_hit.mp3");
    enemy_got_hit = LoadSound("assets/hit.mp3");
}

void enemy::create(string s){
    float height = GetScreenHeight();
    float width = GetScreenWidth();
    name = s;
    json jsondata;
    ifstream file(s);
    file>>jsondata;
    file.close();
    pos = {jsondata["pos"][0].get<float>() * width/1600,jsondata["pos"][1].get<float>() * height/800};
    size = {jsondata["size"][0].get<float>() * width/1600,jsondata["size"][1].get<float>() * height/800};
    speed = {jsondata["speed"][0].get<float>() * width/1600,jsondata["speed"][1].get<float>() * height/800};
    direction = jsondata["direction"].get<int>();
    state = jsondata["state"].get<STATE>();
    frame = jsondata["frame"].get<vector<int>>();
    frame_time  = jsondata["frame_time"].get<vector<float>>();
    frame_count = jsondata["frame_count"].get<vector<int>>();
    current_time = 0;
    elevation_level = jsondata["elevation_level"].get<level>();
    health = 100;
    gravity = height/800;
    player_got_hit_interval = frame_count[shooting]*frame_time[shooting];
}

void enemy::update(surface &m,player &p,vector<player_projectile> &bullet){
    gravity_apply();
    crash_fix(m);
    bool grounded_status = grounded_check(m);

    float width = GetScreenWidth();
    float height = GetScreenHeight();


    float update = speed.x;
    if(elevation_level == p.get_elevation_level()){
        if(pos.x < p.get_pos().x) direction = 1;
        else if(pos.x > p.get_pos().x) direction = -1;
        if(abs(pos.x - p.get_pos().x) <= p.get_size().x/2) update = 0;
    }
    else{
        if(elevation_level == upper_left) direction = 1;
        if(elevation_level == upper_right) direction = -1;
        if(!grounded_status){
            if(elevation_level != bottom) update += 3 * width/1600; // speed increases while jumping
            else update += 1 * width/1600;
        }
        //jumping algorithm
        //when grounded only then will be able to jump
        else{
            if(elevation_level == bottom){
                float left = pos.x + size.x/2 - (m.platform[mid].x - 160 * width/1600);
                float right = pos.x + size.x/2 - (m.platform[mid].x + m.platform[mid].width + 160 * width/1600);
                float distance = (abs(left) > abs(right)) ? right : left;
                if(abs(distance) > 16 * width/1600) direction = (distance > 0) ? -1 : 1;
                else{
                    direction = (m.platform[mid].x + m.platform[mid].width/2 > pos.x) ? 1:-1;
                    speed.y = -24 * height/800;
                }
            }
            if(elevation_level == mid && p.get_elevation_level()> elevation_level){
                direction = (p.get_pos().x > pos.x) ? 1 : -1 ;
                if(direction == 1 && (pos.x + size.x >= m.platform[mid].x + m.platform[mid].width)){
                    speed.y += -24 * height/800;
                }
                if(direction == -1 && (pos.x <= m.platform[mid].x)){
                    speed.y += -24 * height/800;
                }
            }
        }
    }
    pos.x += direction*update;

    //state update
    Rectangle player_rect = {p.get_pos().x,p.get_pos().y,p.get_size().x,p.get_size().y};
    Rectangle enemy_rect = {pos.x,pos.y,size.x,size.y};

    if(state != shooting && player_got_hit_interval <frame_count[shooting]*frame_time[shooting]) player_got_hit_interval = frame_count[shooting]*frame_time[shooting];
    if(CheckCollisionRecs(player_rect,enemy_rect)){
        player_got_hit_interval += GetFrameTime();
        state = shooting , p.set_health(p.get_health() - enemy_attack_player);//0.1 player health loss per frame
        if(frame[shooting] == 0 && player_got_hit_interval >= frame_count[shooting]*frame_time[shooting] - 0.01){
            player_got_hit_interval = 0;
            PlaySound(player_got_hit);
        }
    }
    else if(speed.y > 0) state = jumping_down;
    else if(speed.y < 0) state = jumping_up;
    else state = walking;
    sprite_update();
    //enemy respawn 
    for(int i=0;i<bullet.size();i++) if(got_shot(&bullet[i])) PlaySound(enemy_got_hit), health -= player_attack_enemy, bullet[i].active = false;
    if(health <= 0){
        p.score++;
        p.kills++;
        ifstream file(name);
        json data;
        file >> data;
        file.close();
        pos = {data["pos"][0].get<float>() * width/1600,data["pos"][1].get<float>() * height/800};
        speed = {data["speed"][0].get<float>() * width/1600,data["speed"][1].get<float>() * height/800};
        direction = data["direction"].get<int>();
        elevation_level = data["elevation_level"].get<level>();
        health = 100;
    }
}