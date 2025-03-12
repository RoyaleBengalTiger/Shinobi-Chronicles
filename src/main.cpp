#include "UI/UI.h"

using namespace std;

int main()
{
    InitAudioDevice();
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(0,0, "Shinobi Chronicles");
    SetExitKey(0);
    ui UI;
    UI.load_game();
    UI.play_music();
    SetTargetFPS(60);
    while(!WindowShouldClose() && UI.get_game() != game_end)
    {
        UI.update_music();
        UI.update_game();
        UI.draw_game();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
