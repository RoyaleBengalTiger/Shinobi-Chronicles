
#include "surface.h"

void surface::load(){
    platform_texture = {
        (LoadTexture("assets/platform2.png")),
        (LoadTexture("assets/platform.png")),
        (LoadTexture("assets/platform.png")),
        (LoadTexture("assets/platform.png"))    
    };
    active = false;
}

void surface::create(){
    float width = 1.0*GetScreenWidth()/100;
    float height = 1.0*GetScreenHeight()/100;
    platform.push_back( (Rectangle){(width*0),(height*94),(width*100),(height*12)}); // bottom level
    platform.push_back( (Rectangle){(width*35),(height*65),(width*30),(height*5)}); // mid level
    platform.push_back( (Rectangle){(width*-7),(height*40),(width*30),(height*5)}); // top left level
    platform.push_back( (Rectangle){(width*77),(height*40),(width*30),(height*5)}); // top right level


    wall.push_back((Rectangle){0,0,width*0.5,height*100});
    wall.push_back((Rectangle){width*99.5,0,width*2,height*100});

}

void surface::update(){
    float scale_x = GetScreenWidth()/GetMonitorWidth(0);
    float scale_y = GetScreenHeight()/GetMonitorHeight(0);
    for(int i=0;i<platform.size();i++) platform[i] = (Rectangle){platform[i].x*scale_x,platform[i].y*scale_y,platform[i].width*scale_x,platform[i].height*scale_x};
    for(int i=0;i<wall.size();i++) wall[i] = (Rectangle){wall[i].x*scale_x,wall[i].y*scale_y,wall[i].width*scale_x,wall[i].height*scale_x};

}

void surface::draw(){
    for(int i=0;i<platform.size();i++){
        DrawTexturePro(platform_texture[i],(Rectangle){0,0,platform_texture[i].width,platform_texture[i].height},platform[i],{0,0},0,WHITE);
    }
}