#pragma once

#include "../main.h"

using namespace std;

class ui
{
private:
    surface map;
    player Player;
    enemy Enemy1,Enemy2;
    boss Boss;
    vector<player_projectile> Player_projectile;
    vector<boss_projectile> Boss_projectile;

    Texture2D homescreen,gamescreen,board_background,set_name_background;
    Font gamefont1;
    Font gamefont2;
    Font gamefont3;
    Font gamefont4;
    Font defaultfont;
    GAMESTATE game;

    GAMESTATE update_menu();
    GAMESTATE update_gameplay();
    GAMESTATE update_final();
    GAMESTATE update_try_again();
    GAMESTATE update_escape_menu();
    GAMESTATE update_set_name();
    GAMESTATE update_scoreboard();

    Music music;
    Music boss_music;

    void draw_menu();
    void draw_gameplay();
    void draw_final();
    void draw_try_again();
    void draw_escape_menu();
    void draw_set_name();
    void draw_scoreboard();

    void draw_stats();

    void loadscores();
    void savescores();

    bool game_status;
    char player_name[30];
    bool edit_mode;
    bool continuing;
    bool pause;
    vector<pair<string,int>> score_list;


public:
    ui() : Player_projectile(MAX_PROJECTILE),Boss_projectile(BOSS_MAX_PROJECTILE),player_name(""){
    }
    ~ui(){
        UnloadTexture(homescreen);
        UnloadTexture(gamescreen);
        UnloadTexture(board_background);
        UnloadTexture(set_name_background);
        UnloadFont(gamefont1);
        UnloadFont(gamefont2);
        UnloadFont(gamefont3);
        UnloadFont(gamefont4);
        UnloadMusicStream(music);
        UnloadMusicStream(boss_music);
    }
    GAMESTATE get_game();
    void load_game();
    void create_game();
    void update_game();
    void draw_game();
    void play_music();
    void update_music();
};