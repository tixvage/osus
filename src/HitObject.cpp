#include <HitObject.h>
#include "GameManager.h"
#include <math.h>
#include <algorithm>



int orientation(Vector2 p1, Vector2 p2, Vector2 p3)
{
    int val = (p2.y - p1.y) * (p3.x - p2.x) -
              (p2.x - p1.x) * (p3.y - p2.y);

 
    return (val > 0)? false: true; // clock or counterclock wise
}

Vector2 getBezierPoint( Vector2* points, int numPoints, float t ) {
    Vector2* tmp = new Vector2[numPoints];
    memcpy(tmp, points, numPoints * sizeof(Vector2));
    int i = numPoints - 1;
    while (i > 0) {
        for (int k = 0; k < i; k++)
            tmp[k] = Vector2{tmp[k].x + t *(tmp[k+1].x - tmp[k].x),tmp[k].y + t *(tmp[k+1].y - tmp[k].y)};
        i--;
    }
    Vector2 answer = tmp[0];
    delete[] tmp;
    return answer;
}

std::pair<Vector2, int> getPerfectCircle(Vector2 p1, Vector2 p2, Vector2 p3) {
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;
    int x3 = p3.x;
    int y3 = p3.y;

    int a = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;

    int b = (x1 * x1 + y1 * y1) * (y3 - y2) 
            + (x2 * x2 + y2 * y2) * (y1 - y3)
            + (x3 * x3 + y3 * y3) * (y2 - y1);

    int c = (x1 * x1 + y1 * y1) * (x2 - x3) 
            + (x2 * x2 + y2 * y2) * (x3 - x1) 
            + (x3 * x3 + y3 * y3) * (x1 - x2);

    int x = -b / (2 * a);
    int y = -c / (2 * a);

    return std::make_pair(Vector2{x,y}, sqrt((x - x1) * (x - x1) + (y - y1) *(y - y1)));
}

Circle::Circle(HitObjectData data){
    this->data = data;
    init();
}

void Circle::init(){
    GameManager* gm = GameManager::getInstance();

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
    if(data.colour.size() > 2) DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, clampedFade));
    else DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(WHITE, clampedFade));
    //DrawTextEx(GetFontDefault(),(std::to_string(data.comboNumber)).c_str(), Vector2{data.x*2,data.y*2},20,3,Fade(WHITE, clampedFade));
    render_combo();
    DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, clampedFade));
    if(data.colour.size() > 2) DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, clampedFade));
    else DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(WHITE, clampedFade));
}

void Circle::dead_render(){
    //DrawRectangle(data.x*2, data.y*2, 5,5, WHITE);
    GameManager* gm = GameManager::getInstance();
    float scale = (gm->currentTime*1000 + 400 - data.time )/400;
    float fadeAnimation = 0.3*(1-((gm->currentTime*1000 + 200 - data.time )/200-1));
    float fadePoint = (1-((gm->currentTime*1000 + 400 - data.time )/400-1));
    float movePoint = (((gm->currentTime*1000 + 400 - data.time )/400-1))*20;
    if(data.colour.size() > 2) DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*scale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*scale*0.5f*gm->windowScale/2},0,scale*gm->windowScale/2, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, fadeAnimation));
    else DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*scale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*scale*0.5f*gm->windowScale/2},0,scale*gm->windowScale/2, Fade(WHITE, fadeAnimation));
    if(data.point == 0) DrawTextureEx(gm->hit0, Vector2{data.x*gm->windowScale-gm->hit0.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit0.height*1*0.5f*gm->windowScale/2},(1-fadePoint)*15,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 1) DrawTextureEx(gm->hit50, Vector2{data.x*gm->windowScale-gm->hit50.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit50.height*1*0.5f*gm->windowScale/2 },0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 2) DrawTextureEx(gm->hit100, Vector2{data.x*gm->windowScale-gm->hit100.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit100.height*1*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 3) DrawTextureEx(gm->hit300, Vector2{data.x*gm->windowScale-gm->hit300.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit300.height*1*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
}

void Circle::dead_update(){
    GameManager* gm = GameManager::getInstance();
    if (data.time+400 < gm->currentTime*1000){
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
        DrawTextureEx(gm->numbers[number], Vector2{(float)data.x*gm->windowScale - origin*gm->windowScale/2 + (digits - i - 1) * (gm->numbers[0].width - 150)*gm->windowScale/2, (float)data.y*gm->windowScale - gm->numbers[0].width*gm->windowScale/2 / 2 },0,gm->windowScale / 2, Fade(WHITE, clampedFade));
    }
}

Slider::Slider(HitObjectData data){
    this->data = data;
    init();
}

void Slider::init(){
    GameManager* gm = GameManager::getInstance();

    edgePoints.push_back(Vector2{data.x, data.y});

    float resolution = data.length;

    float currentResolution = 0;

    float lengthScale, totalLength = 0;

    for(int i = 0; i < data.curvePoints.size(); i++){
        edgePoints.push_back(Vector2{data.curvePoints[i].first, data.curvePoints[i].second});
    }
    if(data.curveType == 'L'){
        std::vector<float> lineLengths;

        for(int i = 0; i < edgePoints.size()-1; i++){
            lineLengths.push_back(std::sqrt(std::pow(std::abs(edgePoints[i].x - edgePoints[i+1].x),2)+std::pow(std::abs(edgePoints[i].y - edgePoints[i+1].y),2)));
        }

        for(int i = 0; i < lineLengths.size(); i++){
            //std::cout << lineLengths[i] << " ";
            totalLength+=lineLengths[i];
        }

        lengthScale = totalLength/data.length;

        //std::cout << " -> " << lengthScale << " " << totalLength / lengthScale << std::endl;

        for(int i = 0; i < edgePoints.size()-1; i++){
            //renderPoints.push_back(edgePoints[i]);
            for(float j = 0; j < lineLengths[i]; j += lengthScale){
                
                renderPoints.push_back(Vector2{edgePoints[i].x + (edgePoints[i+1].x - edgePoints[i].x)*j/lineLengths[i], edgePoints[i].y + (edgePoints[i+1].y - edgePoints[i].y)*j/lineLengths[i]});
                //std::cout << (edgePoints[i+1].x - edgePoints[i].x)*j/lineLengths[i] << " " << (edgePoints[i+1].y - edgePoints[i].y)*j/lineLengths[i] << std::endl;
            }
        }
        renderPoints.push_back(edgePoints[edgePoints.size()-1]);

        while(!false){
            if(renderPoints.size()-1 <= data.length) break;
            renderPoints.pop_back();
        }

    }else if(data.curveType == 'B'){
        Vector2 edges[edgePoints.size()];

        for(int i = 0; i < edgePoints.size(); i++){
            edges[i] = edgePoints[i];
        }

        for(float i = 0; i < 1 + 1.0f / resolution; i += 1.0f / resolution){
            if(currentResolution > resolution) break;
            currentResolution++;
            Vector2 tmp = getBezierPoint(edges, edgePoints.size(), i);
            renderPoints.push_back(tmp);
        }
        
    }else if(data.curveType == 'P'){

        /*
            I literally took a fucking 3 hour trigonometry lesson to get this shit working
            UPDATE : THIS SHIT ISNT WORKING
        */

        std::pair<Vector2, int> circleData = getPerfectCircle(edgePoints[0], edgePoints[1], edgePoints[2]);

        Vector2 center = circleData.first;

        int radius = circleData.second;

        // Some math shit
        float degree1 = atan2(edgePoints[0].y - center.y , edgePoints[0].x - center.x) * RAD2DEG;
        float degree2 = atan2(edgePoints[1].y - center.y , edgePoints[1].x - center.x) * RAD2DEG;
        float degree3 = atan2(edgePoints[2].y - center.y , edgePoints[2].x - center.x) * RAD2DEG;

        degree1 = degree1 < 0 ? degree1 + 360 : degree1;
        degree2 = degree2 < 0 ? degree2 + 360 : degree2;
        degree3 = degree3 < 0 ? degree3 + 360 : degree3;

        bool clockwise = !orientation(edgePoints[0], edgePoints[1], edgePoints[2]);
        
        //std::cout << !orientation(edgePoints[0], edgePoints[1], edgePoints[2]) << " " << degree1 << " " << degree3 << " ";

        //eren pls fix this failure of a code...

        //eren sen cidden bunun icin aglion mu
        if(clockwise){

            degree1 = degree1 < degree3 ? degree1 + 360 : degree1;
            degree2 = degree2 < degree3 ? degree2 + 360 : degree2;

            for(float i = degree1; i > degree3 - (degree1-degree3)/resolution; i-= (degree1-degree3)/resolution){
                if(currentResolution > resolution) break;
                currentResolution++;
                Vector2 tempPoint = Vector2{center.x + cos(i / RAD2DEG) * radius, center.y + sin(i / RAD2DEG) * radius};
                renderPoints.push_back(tempPoint);
                //std::cout << "+";
            }

        }else{

            degree2 = degree2 < degree1 ? degree2 + 360 : degree2;
            degree3 = degree3 < degree1 ? degree3 + 360 : degree3;

            for(float i = degree3; i > degree1 - (degree3-degree1)/resolution; i -= (degree3-degree1)/resolution){
                if(currentResolution > resolution) break;
                currentResolution++;
                Vector2 tempPoint = Vector2{center.x + cos(i / RAD2DEG) * radius, center.y + sin(i / RAD2DEG) * radius};
                renderPoints.push_back(tempPoint);
                //std::cout << "-";
            }

            std::reverse(renderPoints.begin(), renderPoints.end());
        }
        

        
    }else if(data.curveType == 'C'){

    }else{
        std::__throw_invalid_argument("What The Fuck? Invalid Slider Type");
    }
    for(int i = 0; i < renderPoints.size(); i++){
        minX = std::min(minX, (int)renderPoints[i].x);
        minY = std::min(minY, (int)renderPoints[i].y);
        maxX = std::max(maxX, (int)renderPoints[i].x);
        maxY = std::max(maxY, (int)renderPoints[i].y);
    }
    sliderTexture = LoadRenderTexture((maxX-minX+56)*gm->windowScale, (maxY-minY+56)*gm->windowScale+gm->windowScale);
    BeginTextureMode(sliderTexture);
    ClearBackground(BLANK);
    for(int i = 0; i < renderPoints.size(); i+=gm->skip){
        DrawCircle((renderPoints[i].x-minX+28)*gm->windowScale, sliderTexture.texture.height + gm->windowScale - (renderPoints[i].y-minY+28)*gm->windowScale, 28*gm->windowScale,Color{200,200,200,255});
        //DrawLineEx(renderPoints[i],renderPoints[i+1], 3, WHITE);
    }
    for(int i = 0; i < renderPoints.size(); i+=gm->skip){
        DrawCircle((renderPoints[i].x-minX+28)*gm->windowScale, sliderTexture.texture.height + gm->windowScale - (renderPoints[i].y-minY+28)*gm->windowScale, 26*gm->windowScale,Color{16,16,16,255});
        //DrawLineEx(renderPoints[i],renderPoints[i+1], 3, i < 2 ? GREEN : RED);
    }
    EndTextureMode();
    //debuggink
}

void Slider::update(){
    GameManager* gm = GameManager::getInstance();

    position = (gm->currentTime * 1000.f - (float)data.time) / (333.33f);// / std::stof(gm->gameFile.configDifficulty["SliderMultiplier"]));
    position *= 100;
    if(gm->currentTime*1000 - data.time > 0){
        if ((int)((gm->currentTime*1000 - data.time) /((data.length/100) * 333.33f)) % 2 == 1) position = (int)data.length - ((int)position % (int)data.length + 1); 
        else position = (int)position % (int)data.length + 1; 
    }
    position = std::max(0.f,position);
    
    if(gm->currentTime*1000 > data.time + (data.length/100) * 333.33f * data.slides){
        data.time = gm->currentTime*1000;
        data.point = 0;
        gm->clickCombo = 0;

        gm->destroyHitObject();
    }
}

void Slider::render(){
    GameManager* gm = GameManager::getInstance();

    if(data.curveType == 'L'){
        float clampedFade = gm->clip((gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in,0.0f,0.75f);
        if(renderPoints.size() > 0){
            DrawTextureEx(sliderTexture.texture, Vector2{(minX-28)*gm->windowScale,(minY-28)*gm->windowScale},0,1, Fade(WHITE,clampedFade));
        }
        int calPos = position;
        calPos = std::min(calPos, static_cast<int>(renderPoints.size()-1));
        
        if(data.colour.size() > 2) DrawTextureEx(gm->sliderb, Vector2{renderPoints[calPos].x*gm->windowScale-gm->sliderb.width*0.5f*gm->windowScale/2,renderPoints[calPos].y*gm->windowScale-gm->sliderb.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, clampedFade));
        else DrawTextureEx(gm->sliderb, Vector2{renderPoints[calPos].x*gm->windowScale-gm->sliderb.width*0.5f*gm->windowScale/2,renderPoints[calPos].y*gm->windowScale-gm->sliderb.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(WHITE, clampedFade));      
        
    }else if(data.curveType == 'B'){
        float clampedFade = gm->clip((gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in,0.0f,0.75f);
        if(renderPoints.size() > 0){
            DrawTextureEx(sliderTexture.texture, Vector2{(minX-28)*gm->windowScale,(minY-28)*gm->windowScale},0,1, Fade(WHITE,clampedFade));
        }
        int calPos = position;
        calPos = std::min(calPos, static_cast<int>(renderPoints.size()-1));
        
        if(data.colour.size() > 2) DrawTextureEx(gm->sliderb, Vector2{renderPoints[calPos].x*gm->windowScale-gm->sliderb.width*0.5f*gm->windowScale/2,renderPoints[calPos].y*gm->windowScale-gm->sliderb.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, clampedFade));
        else DrawTextureEx(gm->sliderb, Vector2{renderPoints[calPos].x*gm->windowScale-gm->sliderb.width*0.5f*gm->windowScale/2,renderPoints[calPos].y*gm->windowScale-gm->sliderb.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(WHITE,clampedFade));      
        
    }else if(data.curveType == 'P'){
        if(renderPoints.size() > 0){
            float clampedFade = gm->clip((gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in,0.0f,0.75f);
            if(renderPoints.size() > 0){
                DrawTextureEx(sliderTexture.texture, Vector2{(minX-28)*gm->windowScale,(minY-28)*gm->windowScale},0,1, Fade(WHITE,clampedFade));
            }

            int calPos = position;
            calPos = std::min(calPos, static_cast<int>/*yes c++*/(renderPoints.size()-1));
        
            if(data.colour.size() > 2) DrawTextureEx(gm->sliderb, Vector2{renderPoints[calPos].x*gm->windowScale-gm->sliderb.width*0.5f*gm->windowScale/2,renderPoints[calPos].y*gm->windowScale-gm->sliderb.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, clampedFade));
            else DrawTextureEx(gm->sliderb, Vector2{renderPoints[calPos].x*gm->windowScale-gm->sliderb.width*0.5f*gm->windowScale/2,renderPoints[calPos].y*gm->windowScale-gm->sliderb.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(WHITE, clampedFade));      
        }
    }else{
        float approachScale = 3*(1-(gm->currentTime*1000 - data.time + gm->gameFile.preempt)/gm->gameFile.preempt)+1;
        if (approachScale <= 1) approachScale = 1;
        float clampedFade = (gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in;
        if(data.colour.size() > 2) DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, clampedFade));
        else DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(WHITE, clampedFade));
        //DrawTextEx(GetFontDefault(),(std::to_string(data.comboNumber)).c_str(), Vector2{data.x*2,data.y*2},20,3,Fade(WHITE, clampedFade));
        render_combo();
        DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, clampedFade));
        if(data.colour.size() > 2) DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, clampedFade));
        else DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(WHITE, clampedFade));
    }
    
}

void Slider::dead_render(){
    //DrawRectangle(data.x*2, data.y*2, 5,5, WHITE);
    GameManager* gm = GameManager::getInstance();
    float scale = (gm->currentTime*1000 + 400 - data.time )/400;
    float fadeAnimation = 0.3*(1-((gm->currentTime*1000 + 200 - data.time )/200-1));
    float fadePoint = (1-((gm->currentTime*1000 + 400 - data.time )/400-1));
    float movePoint = (((gm->currentTime*1000 + 400 - data.time )/400-1))*20;
    if(data.colour.size() > 2) DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*scale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*scale*0.5f*gm->windowScale/2},0,scale*gm->windowScale/2, Fade(Color{data.colour[0],data.colour[1],data.colour[2]}, fadeAnimation));
    else DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*scale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*scale*0.5f*gm->windowScale/2},0,scale*gm->windowScale/2, Fade(WHITE, fadeAnimation));
    if(data.point == 0) DrawTextureEx(gm->hit0, Vector2{data.x*gm->windowScale-gm->hit0.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit0.height*1*0.5f*gm->windowScale/2},(1-fadePoint)*15,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 1) DrawTextureEx(gm->hit50, Vector2{data.x*gm->windowScale-gm->hit50.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit50.height*1*0.5f*gm->windowScale/2 },0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 2) DrawTextureEx(gm->hit100, Vector2{data.x*gm->windowScale-gm->hit100.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit100.height*1*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 3) DrawTextureEx(gm->hit300, Vector2{data.x*gm->windowScale-gm->hit300.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit300.height*1*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
}

void Slider::dead_update(){
    GameManager* gm = GameManager::getInstance();
    if (data.time+400 < gm->currentTime*1000){
        UnloadRenderTexture(sliderTexture);
        gm->destroyDeadHitObject();
    }
}

void Slider::render_combo(){
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
        DrawTextureEx(gm->numbers[number], Vector2{(float)data.x*gm->windowScale - origin*gm->windowScale/2 + (digits - i - 1) * (gm->numbers[0].width - 150)*gm->windowScale/2, (float)data.y*gm->windowScale - gm->numbers[0].width*gm->windowScale/2 / 2 },0,gm->windowScale / 2, Fade(WHITE, clampedFade));
    }
}