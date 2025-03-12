#include "projectile.h"

void projectile::sprite_update(){
    current_time += GetFrameTime();
    if(current_time >= frame_time[0]){
        current_time = 0;
        frame[0]++;
        if(frame[0] >= frame_count[0]) frame[0] = 0;
    }
}