#pragma once

#define until_boss 10
#define until_death 5
#define player_attack_boss 2
#define player_attack_enemy 35
#define boss_shoot_player 0.3
#define boss_attack_player 0.2
#define enemy_attack_player 0.1
 
enum STATE{
    rest,
    walking,
    running,
    jumping_up,
    jumping_down,
    shooting,
    melee
};

enum level{
    bottom,
    mid,
    upper_left,
    upper_right
};

enum GAMESTATE{
    menu,
    set_name,
    playing,
    final,
    try_again,
    escape_menu,
    game_end,
    scoreboard,
    settings,
    load_game,
    save_game
};
