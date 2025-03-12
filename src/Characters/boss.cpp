#include "boss.h"
#include "json.hpp"
using json = nlohmann::json;

void boss::load()
{
    texture[walking] = LoadTexture("assets/boss_walk.png");
    texture[shooting] = LoadTexture("assets/boss_shoot.png");
    texture[melee] = LoadTexture("assets/boss_attack.png");
    boss_shot_projectile = LoadSound("assets/boss_projectile.mp3");
    player_got_clawed = LoadSound("assets/boss_attack.mp3");
    boss_got_hit = LoadSound("assets/hit.mp3");
}

void boss::create()
{
    float height = GetScreenHeight();
    float width = GetScreenWidth();
    json jsondata;
    ifstream file("boss.json");
    file >> jsondata;
    file.close();
    pos = {jsondata["pos"][0].get<float>() * width / 1600, jsondata["pos"][1].get<float>() * height / 800};
    size = {jsondata["size"][0].get<float>() * width / 1600, jsondata["size"][1].get<float>() * height / 800};
    speed = {jsondata["speed"][0].get<float>() * width / 1600, jsondata["speed"][1].get<float>() * height / 800};
    direction = jsondata["direction"].get<int>();
    state = jsondata["state"].get<STATE>();
    frame = jsondata["frame"].get<vector<int>>();
    frame_time = jsondata["frame_time"].get<vector<float>>();
    frame_count = jsondata["frame_count"].get<vector<int>>();
    current_time = 0;
    elevation_level = jsondata["elevation_level"].get<level>();
    health = 100;
    gravity = height / 800;
    active = false;
    shoot_delay = 1.9;
    player_got_hit_interval =  frame_count[melee]*frame_time[melee];
}

void boss::update(surface &m, player &p, vector<player_projectile> &bullet, vector<boss_projectile> &fire)
{
    if (!active) return;
    gravity_apply();
    crash_fix(m);
    bool grounded_status = grounded_check(m);
    float width = GetScreenWidth();
    float height = GetScreenHeight();

    float update = speed.x;
    if (elevation_level == bottom){
        if (p.get_elevation_level() == bottom && abs(pos.x + size.x / 2 - p.get_pos().x - p.get_size().x / 2) >= p.get_size().x * 3 / 4) state = walking;
        else if (p.get_elevation_level() == bottom) update = 0;
        else update = 0, state = shooting;
        direction = ((pos.x + size.x / 2) < (p.get_pos().x + p.get_size().x / 2)) ? 1 : -1;
    }
    else{
        state = walking;
        if (elevation_level == upper_left) direction = 1;
        else direction = -1;
    }
    pos.x += direction * update;

    // melee attack
    Rectangle player_rect = {p.get_pos().x, p.get_pos().y, p.get_size().x, p.get_size().y};
    Rectangle boss_rect = {pos.x, pos.y, size.x, size.y};
    if (CheckCollisionRecs(player_rect, boss_rect)){
        state = melee, p.set_health(p.get_health() - boss_attack_player);
        player_got_hit_interval += GetFrameTime();
        if(frame[melee] == 0 && player_got_hit_interval >= frame_count[melee]*frame_time[melee] - 0.02){
            player_got_hit_interval = 0;
            PlaySound(player_got_clawed);
        }
    }

    // shoot attack
    if (state == shooting){
        shoot_delay += GetFrameTime();
        if ((frame[shooting] == 5)){
            if (shoot_delay >= 1.9){
                PlaySound(boss_shot_projectile);
                shoot_delay = 0;
                float y = p.get_pos().y - pos.y;
                float x = p.get_pos().x - pos.x;
                float angl = atan2(y, x);
                fire[fire[0].get_last_projectile()].shoot_projectile({pos.x + size.x / 2, pos.y + size.y / 2}, angl);
            }
        }
    }
    else shoot_delay = 1.9;

    for (int i = 0; i < bullet.size(); i++) if (got_shot(&bullet[i])) PlaySound(boss_got_hit), health -= player_attack_boss, bullet[i].active = false;

    sprite_update();
}

void boss::draw()
{
    if(!active) return;
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float stretch = 0;
    if (state == shooting)
        stretch = size.y * 0.45;
    int frameWidth = texture[state].width / frame_count[state];
    Rectangle src_rec = {direction * frameWidth * frame[state], 0, direction * frameWidth, texture[state].height};
    Rectangle des_rec = {pos.x, pos.y - stretch, size.x, size.y + stretch};
    Vector2 origin = {0.0f, 0.0f};
    // Rectangle health_rec = {pos.x, pos.y - 10 * height / 800 - stretch, size.x * health / 100, 10 * height / 800};
    // DrawRectangleRec(health_rec, RED);
    DrawTexturePro(texture[state], src_rec, des_rec, origin, 0, WHITE);
}