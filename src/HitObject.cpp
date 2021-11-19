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

    if(gm->currentTime*1000 > data.time + gm->gameFile.p50Final){
        data.time = gm->currentTime*1000;
        data.point = 0;
        gm->clickCombo = 0;

        gm->destroyHitObject();
    }
}

void Circle::render(){
    GameManager* gm = GameManager::getInstance();
    float approachScale = 3*(1-(gm->currentTime*1000 - data.time + gm->gameFile.preempt)/gm->gameFile.preempt)+1;
    if (approachScale <= 1) approachScale = 1;
    float clampedFade = (gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in;
    if(data.colour.size() > 2) DrawTextureEx(gm->hitCircle, Vector2{x,y},0,1, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, clampedFade));
    else DrawTextureEx(gm->hitCircle, Vector2{x,y},0,1, Fade(PINK, clampedFade));
    //DrawTextEx(GetFontDefault(),(std::to_string(data.comboNumber)).c_str(), Vector2{data.x*2,data.y*2},20,3,Fade(WHITE, clampedFade));
    render_combo();
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
    else if(data.point == 1) DrawTextureEx(gm->hit50, Vector2{data.x*2-gm->hit50.width*1*0.5f ,data.y*2-gm->hit50.height*1*0.5f - movePoint},0,1, Fade(WHITE, fadePoint));
    else if(data.point == 2) DrawTextureEx(gm->hit100, Vector2{data.x*2-gm->hit100.width*1*0.5f ,data.y*2-gm->hit100.height*1*0.5f - movePoint},0,1, Fade(WHITE, fadePoint));
    else if(data.point == 3) DrawTextureEx(gm->hit300, Vector2{data.x*2-gm->hit300.width*1*0.5f ,data.y*2-gm->hit300.height*1*0.5f - movePoint},0,1, Fade(WHITE, fadePoint));
}

void Circle::dead_update(){
    GameManager* gm = GameManager::getInstance();
    if (data.time+200 < gm->currentTime*1000){
        gm->destroyDeadHitObject();
    }
}

void Circle::render_combo(){
    GameManager* gm = GameManager::getInstance();
    float clampedFade = (gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in;
    int digits = 1;
    if(data.comboNumber >= 1000) digits = 4;
    else if(data.comboNumber >= 100) digits = 3;
    else if(data.comboNumber >= 10) digits = 2;
    int origin = (gm->numbers[0].width + (digits - 3) * (gm->numbers[0].width - 150)) / 2;

    for(int i = digits; i >= 1 ; i--){
        int number = data.comboNumber;
        if(i == 1) number = number % 10;
        else if(i == 2) number = (number % 100 - number % 10)/10;
        else if(i == 3) number = (number % 1000 - number % 100)/100;
        else if(i == 4) number = (number % 10000 - number % 1000)/1000;
        DrawTextureEx(gm->numbers[number], Vector2{(float)data.x*2 - origin + (digits - i - 1) * (gm->numbers[0].width - 150), (float)data.y*2 - gm->numbers[0].width / 2},0,1, Fade(WHITE, clampedFade));
    }
}