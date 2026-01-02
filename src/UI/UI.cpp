#define RAYGUI_IMPLEMENTATION
#include "UI.h"
#include "json.hpp"
using json = nlohmann::json;

bool comp(const pair<string,int> &a,const pair<string,int> &b){
    return a.second>b.second;
}

void ui::savescores() {
    json gameData;

    for (const auto &[name, score] : score_list) {
        gameData[name] = score;
    }
    ofstream outputFile("score.json");
    if (outputFile.is_open()) {
        outputFile << gameData.dump(4);
        outputFile.close();
    }
}

void ui::loadscores() {
    json gameData;
    ifstream inputFile("score.json");

    if (inputFile.is_open()) {
        inputFile >> gameData;
        inputFile.close();
    }
    for(auto& [key,value] : gameData.items()){
        score_list.push_back({key,value});
    }
    sort(score_list.begin(),score_list.end(),comp);
    if(score_list.size() > 10) score_list.resize(10);
}


GAMESTATE ui::get_game(){
    return game;
}

void ui::play_music(){
    PlayMusicStream(music);
    SetMusicVolume(music,1);
    SetMusicVolume(boss_music,1);
}
void ui::update_music(){
    UpdateMusicStream(music);
    UpdateMusicStream(boss_music);
}
void ui::load_game(){
    map.load();
    Player.load();
    Enemy1.load();
    Enemy2.load();
    Boss.load();
    for(int i=0;i<MAX_PROJECTILE;i++) Player_projectile[i].load();
    for(int i=0;i<BOSS_MAX_PROJECTILE;i++) Boss_projectile[i].load();
    homescreen = LoadTexture("assets/Picture.png");
    gamescreen = LoadTexture("assets/valley.png");
    board_background = LoadTexture("assets/scoreboard.png");
    set_name_background = LoadTexture("assets/set_name3.png");

    music = LoadMusicStream("assets/background_music.mp3");
    boss_music = LoadMusicStream("assets/boss_music.mp3");

    defaultfont = GetFontDefault();
    gamefont1 = LoadFontEx("assets/njnaruto.ttf",256,0,0);
    gamefont2 = LoadFontEx("assets/Karasha.otf",256,0,0);
    gamefont3 = LoadFontEx("assets/Karasha.otf",256,0,0);
    gamefont4 = LoadFontEx("assets/normal3.ttf",256,0,0);

    loadscores();


    GuiSetFont(gamefont3);

    float width = GetScreenWidth();
    float height = GetScreenHeight();
    int size = 40 * sqrt((width*width + height*height)/(3200000));
    GuiSetStyle(DEFAULT,TEXT_SIZE, size);

    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt((Color){50, 40, 30, 255}));  // Dark brown/black border (Cracked stone)
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt((Color){120, 110, 100, 255})); // Grayish stone base
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt((Color){0,0,0,255})); // Light gray text (engraved look)

    // Hover effect - Slight glow, no blue!
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt((Color){70, 60, 50, 255}));  // Darker, subtle border
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt((Color){140, 130, 120, 255})); // Slightly lighter stone
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt((Color){255, 255, 255, 255})); // Brighter text

    // Pressed effect - Makes it look deeper, like it's being pressed into stone
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, ColorToInt((Color){40, 30, 20, 255}));  // Darker brown border
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt((Color){90, 80, 70, 255}));    // Darker stone shade
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt((Color){200, 200, 200, 255})); // Slightly dimmed white text


    // TEXTBOX Style - Stone Engraved Look
    GuiSetStyle(TEXTBOX, BORDER_WIDTH, 5); // Thicker border to match buttons
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 6); // Some padding for a more structured look

    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt((Color){50, 40, 30, 255}));  // Dark brown/black border (Cracked stone)
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, ColorToInt((Color){0,0,0,0}));  
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt((Color){0,0,0,255}));

    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt((Color){70, 60, 50, 255}));  // Slightly more prominent border
    GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED, ColorToInt((Color){0,0,0,0})); 
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt((Color){0,0,0,255}));

    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt((Color){40, 30, 20, 255}));  // Darker border
    GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, ColorToInt((Color){255, 255, 255, 80}));   
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt((Color){0,0,0,255}));

    GuiSetStyle(TEXTBOX, BORDER_COLOR_DISABLED, ColorToInt((Color){30, 25, 20, 255}));  // Very dark border
    GuiSetStyle(TEXTBOX, BASE_COLOR_DISABLED, ColorToInt((Color){0,0,0,0})); 
    GuiSetStyle(TEXTBOX, TEXT_COLOR_DISABLED, ColorToInt((Color){0,0,0,255}));

    game = menu;
    edit_mode = false;
    game_status = false;
    continuing = false;
    pause = false;
}

void ui::create_game(){
    if(!map.active) map.create(),map.active = true;
    Player.create();
    Enemy1.create("enemy1.json");
    Enemy2.create("enemy2.json");
    Boss.create();
    for(int i=0;i<MAX_PROJECTILE;i++) Player_projectile[i].create_projectile();
    for(int i=0;i<BOSS_MAX_PROJECTILE;i++) Boss_projectile[i].create_projectile();

    Texture2D w = LoadTexture("assets/valley.png");
}


void ui::update_game(){
    if(game == menu) game = update_menu();
    else if(game == playing) game = update_gameplay();
    else if(game == final) game = update_final();
    else if(game == try_again) game = update_try_again();
    else if(game == escape_menu) game = update_escape_menu();
    else if(game == set_name) game = update_set_name();
    else if(game == scoreboard) game = update_scoreboard();
}

GAMESTATE ui::update_menu(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    if((GuiButton((Rectangle){ width/2 - 120*w_s , height/3, 240 * w_s, 80 * h_s }, "START GAME"))){
        return set_name;
    }
    if(GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 100 * h_s, 240 * w_s, 80 * h_s }, "SCOREBOARD")) return scoreboard;
    if(GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 200 * h_s, 240 * w_s, 80 * h_s }, "EXIT")) return game_end;
    return menu;
}

GAMESTATE ui::update_gameplay(){
    if(IsKeyPressed(KEY_P)) pause = !pause;
    if(Boss.get_health() <= 0 && Boss.active){
        Player.score += 10;
        if(continuing){
            Boss.active = false,Player.kills = 0;
            StopMusicStream(boss_music);
            PlayMusicStream(music);
        }
        else return final;
    }
    if(IsKeyPressed(KEY_ESCAPE)) return escape_menu;
    if(Player.get_death() >= until_death) return try_again;
    if(Player.kills >= until_boss && !Boss.active){
        Boss.create();
        Boss.active = true;
        StopMusicStream(music);
        PlayMusicStream(boss_music);
    }
    if(!pause){
        Player.update(map,Player_projectile);
        Enemy1.update(map,Player,Player_projectile);
        Enemy2.update(map,Player,Player_projectile);
        Boss.update(map,Player,Player_projectile,Boss_projectile);
        for(int i=0;i<MAX_PROJECTILE;i++) Player_projectile[i].update_projectile(map);
        for(int i=0;i<BOSS_MAX_PROJECTILE;i++) Boss_projectile[i].update_projectile(Player);
    }
    return playing;
}


GAMESTATE ui::update_final(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    if((GuiButton((Rectangle){ width/2 - 120*w_s, height/2, 240 * w_s, 80 * h_s }, "CONTINUE"))){
        Boss.create();
        continuing = true;
        return playing;
    }
    else if(GuiButton((Rectangle){ width/2 - 120*w_s, height/2 + 100 * h_s, 240 * w_s, 80 * h_s }, "MAIN MENU")){
        score_list.push_back({player_name,Player.score});
        sort(score_list.begin(),score_list.end(),comp);
        if(score_list.size() > 10) score_list.resize(10);
        savescores();
        player_name[0] = '\0';
        game_status = false;
        return menu;
    }
    else if(GuiButton((Rectangle){ width/2 - 120*w_s, height/2 + 200 * h_s, 240 * w_s, 80 * h_s }, "EXIT")){
        score_list.push_back({player_name,Player.score});
        sort(score_list.begin(),score_list.end(),comp);
        if(score_list.size() > 10) score_list.resize(10);
        savescores();
        player_name[0] = '\0';
        return game_end;
    }
    else return final;
}

GAMESTATE ui::update_try_again(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;

    if((GuiButton((Rectangle){ width/2 - 120 * w_s, height/2, 240 * w_s, 80 * h_s }, "TRY AGAIN"))){
        StopMusicStream(boss_music);
        play_music();
        score_list.push_back({player_name,Player.score});
        sort(score_list.begin(),score_list.end(),comp);
        if(score_list.size() > 10) score_list.resize(10);
        savescores();
        create_game();
        continuing = false;
        return playing;
    }
    else if(GuiButton((Rectangle){ width/2 - 120 * w_s, height/2 + 100 * h_s, 240 * w_s, 80 * h_s }, "MAIN MENU")){
        score_list.push_back({player_name,Player.score});
        sort(score_list.begin(),score_list.end(),comp);
        if(score_list.size() > 10) score_list.resize(10);
        savescores();
        player_name[0] = '\0';
        game_status = false;
        return menu;
    }
    else if(GuiButton((Rectangle){ width/2 - 120 * w_s, height/2 + 200 * h_s, 240 * w_s, 80 * h_s }, "EXIT")){
        score_list.push_back({player_name,Player.score});
        sort(score_list.begin(),score_list.end(),comp);
        if(score_list.size() > 10) score_list.resize(10);
        savescores();
        player_name[0] = '\0';
        return game_end;
    }
    else return try_again;
}

GAMESTATE ui::update_escape_menu(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    if((GuiButton((Rectangle){ width/2 - 120*w_s , height/3, 240 * w_s, 80 * h_s }, "RESUME"))) return playing;
    else if(GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 100 * h_s, 240 * w_s, 80 * h_s }, "SCOREBOARD")) return scoreboard;
    else if(GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 200 * h_s, 240 * w_s, 80 * h_s }, "MAIN MENU")){
        score_list.push_back({player_name,Player.score});
        sort(score_list.begin(),score_list.end(),comp);
        if(score_list.size() > 10) score_list.resize(10);
        savescores();
        player_name[0] = '\0';
        game_status = false;
        return menu;
    }
    else if(GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 300 * h_s, 240 * w_s, 80 * h_s }, "QUIT GAME")){
        score_list.push_back({player_name,Player.score});
        sort(score_list.begin(),score_list.end(),comp);
        if(score_list.size() > 10) score_list.resize(10);
        savescores();
        player_name[0] = '\0';  
        return game_end;
    }
    else return escape_menu;
}

GAMESTATE ui::update_set_name(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    int size = 70 * sqrt((width*width + height*height)/(3200000));

    Rectangle rect = {width/5 + MeasureTextEx(gamefont3,"ENTER NAME : ", size,0).x,height/3,width/3,MeasureTextEx(gamefont3,"ENTER NAME : ", size,0).y};
    

    if(IsKeyPressed(KEY_ENTER) && strlen(player_name) >= 3){
        create_game();
        game_status = true;
        return playing;
    }
    if(GuiButton({width * 0.75f,height * 0.75f,200 * w_s,80 * h_s },"BACK")) return menu;
    else return set_name;
}

GAMESTATE ui::update_scoreboard(){

    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    int size = 60 * sqrt((width*width + height*height)/(3200000));

    if(GuiButton({width * 0.75f,height * 0.75f,200 * w_s,80 * h_s },"BACK")) return (game_status ? playing : menu);
    else return scoreboard;
}


void ui::draw_game(){
    BeginDrawing();
    if(game == menu) draw_menu();
    else if(game == playing) draw_gameplay();
    else if(game == final) draw_final();
    else if(game == try_again) draw_try_again();
    else if(game == escape_menu) draw_escape_menu();
    else if(game == set_name) draw_set_name();
    else if(game == scoreboard) draw_scoreboard();
    EndDrawing();
}


void ui::draw_menu(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;

    ClearBackground(WHITE);
    DrawTexturePro(homescreen,{0,0,(float)homescreen.width,(float)homescreen.height},{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},{0,0},0,WHITE);

    int size = 40 * sqrt((width*width + height*height)/(3200000));
    DrawTextEx(gamefont2,"SHINOBI    CHRONICLES", {width / 2 - MeasureTextEx(gamefont2,"SHINOBI   CHRONICLES", size*2,0).x / 2, height * 0.1f },size*2,0, BLACK);

    GuiButton((Rectangle){ width/2 - 120*w_s , height/3, 240 * w_s, 80 * h_s }, "START GAME");
    GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 100 * h_s, 240 * w_s, 80 * h_s }, "SCOREBOARD");
    GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 200 * h_s, 240 * w_s, 80 * h_s }, "EXIT");

}

void ui::draw_stats(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    int size = (int)(20 * sqrt((width*width + height*height)/(3200000)));

    string text = format("SCORE    :    {}    POINTS", Player.score);
    DrawTextEx(gamefont1,text.c_str(),{(float)0.5*width - MeasureTextEx(gamefont1,text.c_str(), size,0).x / 2 , (float)0.05 * height},size,0,WHITE);

    DrawTextEx(gamefont1,"SASUKE",{(float)0.2*width,(float)0.09*height},size,0,WHITE);

    for(int i=0;i<until_death - Player.get_death();i++){
        Rectangle life_rect = {0,0,Player.life.width,Player.life.height};
        Rectangle pos_rect = {width*0.2f + MeasureTextEx(gamefont1,"SASUKE     ",size,0).x + i*(MeasureTextEx(gamefont1,"SASUKE",size,0).y + 5 * w_s),0.09f * height,MeasureTextEx(gamefont1,"SASUKE",size,0).y,MeasureTextEx(gamefont1,"SASUKE",size,0).y};
        DrawTexturePro(Player.life,life_rect,pos_rect,{0,0},0,WHITE);
    }

    Rectangle player_health_bar = {0.2*width,0.13*height,0.15*width*Player.get_health()/100,0.025*height};
    DrawRectangleRec(player_health_bar,RED);
    if(Boss.active){
        DrawTextEx(gamefont1,"9    TAILED    NARUTO",{(float)0.65*width,(float)0.09*height},size,0,WHITE);
        Rectangle boss_health_bar = {0.65*width,0.13*height,0.15*width*Boss.get_health()/100,0.025*height};
        DrawRectangleRec(boss_health_bar,RED);
    }
}


void ui::draw_gameplay(){
    ClearBackground(WHITE);
    DrawTexturePro(gamescreen,{0,0,(float)gamescreen.width,(float)gamescreen.height},{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},{0,0},0,WHITE);
    draw_stats();
    map.draw();
    Player.draw();
    Enemy1.draw();
    Enemy2.draw();
    Boss.draw();
    for(int i=0;i<MAX_PROJECTILE;i++) Player_projectile[i].draw();
    for(int i=0;i<BOSS_MAX_PROJECTILE;i++) Boss_projectile[i].draw();
}

void ui::draw_final(){
    draw_gameplay();
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    int size = (int)(60 * sqrt((width*width + height*height)/(3200000)));

    DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),(Color){0,0,0,180});
    DrawTextEx(gamefont1,"YOU'VE    DEFEATED    NARUTO", {width / 2 - MeasureTextEx(gamefont1,"YOU'VE    DEFEATED    NARUTO", size,0).x / 2, height / 4 + 100 * h_s}, size,0, WHITE);
    GuiButton((Rectangle){ width/2 - 120 * w_s, height/2 , 240 * w_s, 80 * h_s }, "CONTINUE");
    GuiButton((Rectangle){ width/2 - 120*w_s, height/2 + 100 * h_s, 240 * w_s, 80 * h_s }, "MAIN MENU");
    GuiButton((Rectangle){ width/2 - 120 * w_s, height/2 + 200 * h_s, 240 * w_s, 80 * h_s}, "EXIT");
}


void ui::draw_try_again(){
    draw_gameplay();
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    int size = (int)(60 * sqrt((width*width + height*height)/(3200000)));

    DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),(Color){0,0,0,180});
    DrawTextEx(gamefont1,"YOU    DIED", {width / 2 - MeasureTextEx(gamefont1,"YOU    DIED", size,0).x / 2, height / 4 + 100 * h_s}, size,0, WHITE);
    GuiButton((Rectangle){ width/2 - 120 * w_s, height/2 , 240 * w_s, 80 * h_s }, "TRY AGAIN");
    GuiButton((Rectangle){ width/2 - 120 * w_s, height/2 + 100 * h_s, 240 * w_s, 80 * h_s }, "MAIN MENU");
    GuiButton((Rectangle){ width/2 - 120 * w_s, height/2 + 200 * h_s, 240 * w_s, 80 * h_s}, "EXIT");
}

void ui::draw_escape_menu(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;

    ClearBackground(WHITE);
    DrawTexturePro(homescreen,{0,0,(float)homescreen.width,(float)homescreen.height},{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},{0,0},0,WHITE);

    int size = 40 * sqrt((width*width + height*height)/(3200000));
    DrawTextEx(gamefont2,"SHINOBI    CHRONICLES", {width / 2 - MeasureTextEx(gamefont2,"SHINOBI   CHRONICLES", size*2,0).x / 2, height * 0.1f },size*2,0, BLACK);

    GuiButton((Rectangle){ width/2 - 120*w_s , height/3, 240 * w_s, 80 * h_s }, "RESUME");
    GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 100 * h_s, 240 * w_s, 80 * h_s }, "SCOREBOARD");
    GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 200 * h_s, 240 * w_s, 80 * h_s }, "MAIN MENU");
    GuiButton((Rectangle){ width/2 - 120*w_s , height/3 + 300 * h_s, 240 * w_s, 80 * h_s }, "QUIT GAME");
}

void ui::draw_set_name(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    int size = 70 * sqrt((width*width + height*height)/(3200000));

    ClearBackground(WHITE);
    DrawTexturePro(set_name_background,{0,0,(float)set_name_background.width,(float)set_name_background.height},{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},{0,0},0,WHITE);

    DrawTextEx(gamefont3,"ENTER NAME : ",{ width / 5, height /3}, size,0, BLACK);
    Rectangle rect = {width/5 + MeasureTextEx(gamefont3,"ENTER NAME : ", size,0).x,height/3,width/3,MeasureTextEx(gamefont3,"ENTER NAME : ", size,0).y};
    
    if(GuiTextBox(rect,player_name,30,edit_mode)){
        edit_mode = !edit_mode;
    }
    GuiButton({width * 0.75f,height * 0.75f,200 * w_s,80 * h_s },"BACK");

}

void ui::draw_scoreboard(){
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    float w_s = width/1600;
    float h_s = height/800;
    int size = 50 * sqrt((width*width + height*height)/(3200000));

    ClearBackground(WHITE);
    DrawTexturePro(board_background,{0,0,(float)board_background.width,(float)board_background.height},{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()},{0,0},0,WHITE);
    
    DrawTextEx(gamefont4,"SCOREBOARD",{width * 0.5f - MeasureTextEx(gamefont4,"SCOREBOARD",size*3,0).x/2,height *0.08f},size * 3,0,BLACK);
    int limit = min((int)(score_list.size()),10);
    for(int i=0;i<limit;i++){
        string text = score_list[i].first;
        string text2 = " - " + to_string(score_list[i].second);
        DrawTextEx(gamefont4,text.c_str(),{width * 0.3f ,height/4 + i* (MeasureTextEx(gamefont4,text.c_str(),size,0).y+7 * h_s)},size,0,BLACK);
        DrawTextEx(gamefont4,text2.c_str(),{width * 0.6f ,height/4 + i* (MeasureTextEx(gamefont4,text2.c_str(),size,0).y+7 * h_s)},size,0,BLACK);
    }
    GuiButton({width * 0.75f,height * 0.75f,200 * w_s,80 * h_s },"BACK");

}