#include <HitObject.h>
#include "GameManager.h"

Circle::Circle(HitObjectData data){
    this->data = data;
    init();
}

void Circle::init(){
    GameManager* gm = GameManager::getInstance();
    x = data.x*2-gm->hitCircle.width*0.5f;
    y = data.y*2-gm->hitCircle.width*0.5f;
}

void Circle::update(){
    GameManager* gm = GameManager::getInstance();

    if(gm->currentTime*1000 > data.time){
        gm->destroyHitObject();
    }
}

void Circle::render(){
    GameManager* gm = GameManager::getInstance();
    float approachScale = 3*(1-(gm->currentTime*1000 - data.time + gm->gameFile.preempt)/gm->gameFile.preempt)+1;
    float clampedFade = (gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in;
    DrawTextureEx(gm->hitCircle, Vector2{x,y},0,1, Fade(PINK, clampedFade));
    DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*2-gm->hitCircleOverlay.width*0.5f,data.y*2-gm->hitCircleOverlay.height*0.5f},0,1, Fade(WHITE, clampedFade));
    DrawTextureEx(gm->approachCircle, Vector2{data.x*2-gm->approachCircle.width*approachScale*0.5f,data.y*2-gm->approachCircle.height*approachScale*0.5f},0,approachScale, Fade(WHITE, clampedFade));
}

void Circle::dead_render(){
    //DrawRectangle(data.x*2, data.y*2, 5,5, WHITE);
    GameManager* gm = GameManager::getInstance();
    float scale = (gm->currentTime*1000 + 200 - data.time )/200;
    float fadeAnimation = 0.3*(1-((gm->currentTime*1000 + 200 - data.time )/200-1));
    float fadePoint = (1-((gm->currentTime*1000 + 200 - data.time )/200-1));
    float movePoint = (((gm->currentTime*1000 + 200 - data.time )/200-1))*20;
    DrawTextureEx(gm->selectCircle, Vector2{data.x*2-gm->selectCircle.width*scale*0.5f,data.y*2-gm->selectCircle.height*scale*0.5f},0,scale, Fade(WHITE, fadeAnimation));
    if(data.point == 0) DrawTextureEx(gm->hit0, Vector2{data.x*2-gm->hit0.width*1*0.5f ,data.y*2-gm->hit0.height*1*0.5f - movePoint},0,1, Fade(WHITE, fadePoint));
    else if(data.point == 3) DrawTextureEx(gm->hit300, Vector2{data.x*2-gm->hit0.width*1*0.5f ,data.y*2-gm->hit300.height*1*0.5f - movePoint},0,1, Fade(WHITE, fadePoint));
}

void Circle::dead_update(){
    GameManager* gm = GameManager::getInstance();
    if (data.time+200 < gm->currentTime*1000){
        gm->destroyDeadHitObject();
    }
}