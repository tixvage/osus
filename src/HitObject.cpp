#include <HitObject.h>
#include "GameManager.h"

Circle::Circle(HitObjectData data){
    this->data = data;
    init();
}

void Circle::init(){

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
    DrawTextureEx(gm->hitCircle, Vector2{data.x*2-gm->hitCircle.width*0.5f,data.y*2-gm->hitCircle.height*0.5f},0,1, Fade(PINK, clampedFade));
    DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*2-gm->hitCircleOverlay.width*0.5f,data.y*2-gm->hitCircleOverlay.height*0.5f},0,1, Fade(WHITE, clampedFade));
    DrawTextureEx(gm->approachCircle, Vector2{data.x*2-gm->approachCircle.width*approachScale*0.5f,data.y*2-gm->approachCircle.height*approachScale*0.5f},0,approachScale, Fade(WHITE, clampedFade));
}

HitObject* Circle::getPointer(){
    return this;
}