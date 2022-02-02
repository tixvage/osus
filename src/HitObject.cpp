#include <HitObject.h>
#include "GameManager.h"
#include <cmath>
#include <algorithm>


float GetT( float t, float alpha, const Vector2& p0, const Vector2& p1 ){
    Vector2 d  = Vector2Subtract(p1, p0);
    float a = d.x * 2 + d.y * 2; // Dot product
    float b = std::pow( a, alpha*.5f );
    return (b + t);
}

Vector2 CatmullRom( const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t, float alpha=.5f){
    float t0 = 0.0f;
    float t1 = GetT( t0, alpha, p0, p1 );
    float t2 = GetT( t1, alpha, p1, p2 );
    float t3 = GetT( t2, alpha, p2, p3 );
    t = t1 + t * (t2 - t1);
    Vector2 A1 = Vector2Add(Vector2Scale(p0 , ( t1-t )/( t1-t0 )) , Vector2Scale(p1 , ( t-t0 )/( t1-t0 )));
    Vector2 A2 = Vector2Add(Vector2Scale(p1 , ( t2-t )/( t2-t1 )) , Vector2Scale(p2 , ( t-t1 )/( t2-t1 )));
    Vector2 A3 = Vector2Add(Vector2Scale(p2 , ( t3-t )/( t3-t2 )) , Vector2Scale(p3 , ( t-t2 )/( t3-t2 )));
    Vector2 B1 = Vector2Add(Vector2Scale(A1 , ( t2-t )/( t2-t0 )) , Vector2Scale(A2 , ( t-t0 )/( t2-t0 )));
    Vector2 B2 = Vector2Add(Vector2Scale(A2 , ( t3-t )/( t3-t1 )) , Vector2Scale(A3 , ( t-t1 )/( t3-t1 )));
    Vector2 C  = Vector2Add(Vector2Scale(B1 , ( t2-t )/( t2-t1 )) , Vector2Scale(B2 , ( t-t1 )/( t2-t1 )));
    return C;
}

//checks the perfect circle slider's orientation
int orientation(Vector2 p1, Vector2 p2, Vector2 p3){
    int val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
    return (val > 0)? false: true;
}

//gets the points needed for a bezier curve, we can also define a resolution for it
Vector2 getBezierPoint(std::vector<Vector2> points, int numPoints, float t){
    Vector2* tmp = new Vector2[numPoints];
    for(int i = 0; i < points.size(); i++){
        tmp[i] = points[i];
    }
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

//gets the center point and the size for the perfect circle sliders
std::pair<Vector2, int> getPerfectCircle(Vector2 p1, Vector2 p2, Vector2 p3){
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;
    int x3 = p3.x;
    int y3 = p3.y;
    int a = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;
    int b = (x1 * x1 + y1 * y1) * (y3 - y2) + (x2 * x2 + y2 * y2) * (y1 - y3) + (x3 * x3 + y3 * y3) * (y2 - y1);
    int c = (x1 * x1 + y1 * y1) * (x2 - x3) + (x2 * x2 + y2 * y2) * (x3 - x1) + (x3 * x3 + y3 * y3) * (x1 - x2);
    int x = -b / (2 * a);
    int y = -c / (2 * a);
    return std::make_pair(Vector2{(float)x,(float)y}, sqrt((x - x1) * (x - x1) + (y - y1) *(y - y1)));
}

//creates a circle
Circle::Circle(HitObjectData data){
    this->data = data;
    init();
}

//initilizes a circle
void Circle::init(){
    GameManager* gm = GameManager::getInstance();
}

//the main code that runs for every circle on screen, the collision and point manager is in the GamerManager
void Circle::update(){
    GameManager* gm = GameManager::getInstance();
    //the circle is not clickable after some time so we check that
    if(gm->currentTime*1000 > data.time + gm->gameFile.p50Final){
        //this is needed for dead_update to work, maybe there is a smarter way to do that
        data.time = gm->currentTime*1000;
        data.point = 0;
        //resets the combo
        gm->clickCombo = 0;
        gm->destroyHitObject();
    }
}

//renders the Circle
void Circle::render(){
    GameManager* gm = GameManager::getInstance();
    //calculates the size and opacity of the approach circle, can change a bit
    float approachScale = 3*(1-(gm->currentTime*1000 - data.time + gm->gameFile.preempt)/gm->gameFile.preempt)+1;
    if (approachScale <= 1)
        approachScale = 1;
    float clampedFade = (gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in;
    //draws the hitcircle texture, the code line here is pretty long but gotta deal with that (also there is a fallback protection)
    if(data.colour.size() > 2)
        DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(Color{(unsigned char)data.colour[0],(unsigned char)data.colour[1],(unsigned char)data.colour[2]}, clampedFade));
    else
        DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(WHITE, clampedFade));
    //renders the combo number
    render_combo();
    //renders the overlay, that one doesnt change colors thankfully
    DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, clampedFade));
    //finally renders the approach circle, the same deal as the hitcircle itself
    if(data.colour.size() > 2)
        DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(Color{(unsigned char)data.colour[0],(unsigned char)data.colour[1],(unsigned char)data.colour[2]}, clampedFade));
    else
        DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(WHITE, clampedFade));
}

//renders the "dead" Circle
void Circle::dead_render(){
    GameManager* gm = GameManager::getInstance();
    //calculates opacity and size based on the time left, MAKE IT DEPENDANT TO APPROACH RATE
    float scale = (gm->currentTime*1000 + 400 - data.time )/400;
    float fadeAnimation = 0.3*(1-((gm->currentTime*1000 + 200 - data.time )/200-1));
    //the points move up and they fade so here is some fancy code for that too, MAKE IT DEPENDANT TO APPROACH RATE
    float fadePoint = (1-((gm->currentTime*1000 + 400 - data.time )/400-1));
    float movePoint = (((gm->currentTime*1000 + 400 - data.time )/400-1))*20;
    //we draw the overlay instead of the hitcircle, maybe you can modify it to do it both but it looks cooler this way
    if(data.colour.size() > 2)
        DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*scale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*scale*0.5f*gm->windowScale/2},0,scale*gm->windowScale/2, Fade(Color{(unsigned char)data.colour[0],(unsigned char)data.colour[1],(unsigned char)data.colour[2]}, fadeAnimation));
    else
        DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*scale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*scale*0.5f*gm->windowScale/2},0,scale*gm->windowScale/2, Fade(WHITE, fadeAnimation));
    //renders the point numbers
    if(data.point == 0)
        DrawTextureEx(gm->hit0, Vector2{data.x*gm->windowScale-gm->hit0.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit0.height*1*0.5f*gm->windowScale/2},(1-fadePoint)*15,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 1)
        DrawTextureEx(gm->hit50, Vector2{data.x*gm->windowScale-gm->hit50.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit50.height*1*0.5f*gm->windowScale/2 },0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 2)
        DrawTextureEx(gm->hit100, Vector2{data.x*gm->windowScale-gm->hit100.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit100.height*1*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 3)
        DrawTextureEx(gm->hit300, Vector2{data.x*gm->windowScale-gm->hit300.width*1*0.5f*gm->windowScale/2 ,data.y*gm->windowScale-gm->hit300.height*1*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
}

//just gives more time to render the "dead" Circle 
void Circle::dead_update(){
    GameManager* gm = GameManager::getInstance();
    //gives 400ms for the animation to play, MAKE IT DEPENDANT TO APPROACH RATE
    if (data.time+400 < gm->currentTime*1000)
        gm->destroyDeadHitObject();
}

//renders the combo number on top of the circle, NEED TO FIX THE DIGITS
void Circle::render_combo(){
    GameManager* gm = GameManager::getInstance();
    //calculates the opacity
    float clampedFade = (gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in;
    //checks how many digits there are, dont do it like this please
    int digits = 1;
    if
        (data.comboNumber >= 1000) digits = 4;
    else if
        (data.comboNumber >= 100) digits = 3;
    else if
        (data.comboNumber >= 10) digits = 2;
    //the textures are drawn from the top left so we need to find that point
    int origin = (gm->numbers[0].width + (digits - 3) * (gm->numbers[0].width - 150)) / 2;
    //again, not the smartest code but we render each digit individually
    for(int i = digits; i >= 1 ; i--){
        int number = data.comboNumber;
        if(i == 1)
            number = number % 10;
        else if(i == 2)
            number = (number % 100 - number % 10)/10;
        else if(i == 3)
            number = (number % 1000 - number % 100)/100;
        else if(i == 4)
            number = (number % 10000 - number % 1000)/1000;
        //renders the digit as a texture
        DrawTextureEx(gm->numbers[number], Vector2{(float)data.x*gm->windowScale - origin*gm->windowScale/2 + (digits - i - 1) * (gm->numbers[0].width - 150)*gm->windowScale/2, (float)data.y*gm->windowScale - gm->numbers[0].width*gm->windowScale/2 / 2 },0,gm->windowScale / 2, Fade(WHITE, clampedFade));
    }
}

//creates a Slider
Slider::Slider(HitObjectData data){
    this->data = data;
    init();
}

//initilizes a Slider, all the curve stuff and the texture creation happens here
void Slider::init(){
    GameManager* gm = GameManager::getInstance();
    //these is the points that we get from the beatmap file
    edgePoints.push_back(Vector2{(float)data.x, (float)data.y});
    //the resolution is the number of total points
    float resolution = data.length;
    float currentResolution = 0;
    float lengthScale, totalLength = 0;
    //add every point from the beatmap 
    for(int i = 0; i < data.curvePoints.size(); i++)
        edgePoints.push_back(Vector2{(float)data.curvePoints[i].first, (float)data.curvePoints[i].second});
    //if the "curve" is linear calculate the points needed to render the slider
    if(data.curveType == 'L'){
        //a linear "curve" consists of different sized lines so we calculate them here
        std::vector<float> lineLengths;
        for(int i = 0; i < edgePoints.size()-1; i++)
            lineLengths.push_back(std::sqrt(std::pow(std::abs(edgePoints[i].x - edgePoints[i+1].x),2)+std::pow(std::abs(edgePoints[i].y - edgePoints[i+1].y),2)));
        //gets the total length of the calculated lines
        for(int i = 0; i < lineLengths.size(); i++)
            totalLength+=lineLengths[i];
        //the calculated length is pretty different from the beatmap so we scale the calculations for that
        lengthScale = totalLength/data.length;
        //add the render points to a vector
        for(int i = 0; i < edgePoints.size()-1; i++)
            for(float j = 0; j < lineLengths[i]; j += lengthScale)
                renderPoints.push_back(Vector2{edgePoints[i].x + (edgePoints[i+1].x - edgePoints[i].x)*j/lineLengths[i], edgePoints[i].y + (edgePoints[i+1].y - edgePoints[i].y)*j/lineLengths[i]});
        renderPoints.push_back(edgePoints[edgePoints.size()-1]);
        //this is an inside joke, but yeah if we add more points than needed, we just delete them (doesn't happen that much)
        while(!false){
            if(renderPoints.size()-1 <= data.length) break;
            renderPoints.pop_back();
        }
    }
    else if(data.curveType == 'B'){
        //for the bezier curves we do the calculations in another function
        Vector2 edges[edgePoints.size()];
        for(int i = 0; i < edgePoints.size(); i++)
            edges[i] = edgePoints[i];
        std::vector<Vector2> tempEdges;
        std::vector<Vector2> tempRender;
        std::vector<float> curveLengths;
        float totalCalculatedLength = 0;
        for(int i = 0; i < edgePoints.size(); i++){
            tempEdges.push_back(edgePoints[i]);
            if((edgePoints[i].x == edgePoints[i+1].x && edgePoints[i].y == edgePoints[i+1].y) || i == edgePoints.size()-1){
                currentResolution = 0;
                for(float j = 0; j < 1; j += 1.0f / 100.0f){
                    if(currentResolution > 100) break;
                    currentResolution++;
                    Vector2 tmp = getBezierPoint(tempEdges, tempEdges.size(), j);
                    tempRender.push_back(tmp);
                }
                float tempLength = 0;
                for(int i = 1; i < tempRender.size(); i += 1)
                    tempLength += std::sqrt(std::pow(std::abs(tempRender[i-1].x - tempRender[i].x),2) + std::pow(std::abs(tempRender[i-1].y - tempRender[i].y),2));
                curveLengths.push_back(tempLength);
                totalCalculatedLength += tempLength;
                tempEdges.clear();
                tempRender.clear();
            }
        }
        tempEdges.clear();
        tempRender.clear();
        int curveIndex = 0;
        for(int i = 0; i < edgePoints.size(); i++){
            tempEdges.push_back(edgePoints[i]);
            if((edgePoints[i].x == edgePoints[i+1].x && edgePoints[i].y == edgePoints[i+1].y) || i == edgePoints.size()-1){
                currentResolution = 0;
                float tempResolution = resolution / (totalCalculatedLength / curveLengths[curveIndex]);
                for(float j = 0; j < 1; j += 1.0f / tempResolution){
                    if(currentResolution > tempResolution) break;
                    currentResolution++;
                    Vector2 tmp = getBezierPoint(tempEdges, tempEdges.size(), j);
                    renderPoints.push_back(tmp);
                }
                if(i != edgePoints.size()-1)
                    renderPoints.pop_back();
                tempEdges.clear();
                curveIndex++;
            }
        }
        std::cout << "----------------------------------------------------------------------------"<< std::endl;
        std::cout << "given slider length: " << data.length << "  calculated slider length: " << renderPoints.size() << std::endl;
        std::cout << "----------------------------------------------------------------------------"<< std::endl;
        /*
        for(float i = 0; i < 1; i += 1.0f / resolution){
            if(currentResolution > resolution) break;
            currentResolution++;
            Vector2 tmp = getBezierPoint(edges, edgePoints.size(), i);
            renderPoints.push_back(tmp);
        }
        std::cout << "----------------------------------------------------------------"<< std::endl;
        std::cout << renderPoints.size() << " " << data.length << std::endl;
        float tempLength = 0;
        for(int i = 1; i < renderPoints.size(); i += 1){
            tempLength += std::sqrt(std::pow(std::abs(renderPoints[i-1].x - renderPoints[i].x),2) + std::pow(std::abs(renderPoints[i-1].y - renderPoints[i].y),2));
        }
        std::cout << "calculated slider length: " << tempLength << "  given slider length: " << resolution << std::endl;
        */

        while(!false){
            if(renderPoints.size()-1 <= data.length) break;
            renderPoints.pop_back();
        }
    }
    else if(data.curveType == 'P'){
        //for the perfect circle we just use trigonometry to calculate points on the circle
        std::pair<Vector2, int> circleData = getPerfectCircle(edgePoints[0], edgePoints[1], edgePoints[2]);
        Vector2 center = circleData.first;
        int radius = circleData.second;
        float degree1 = atan2(edgePoints[0].y - center.y , edgePoints[0].x - center.x) * RAD2DEG;
        float degree2 = atan2(edgePoints[1].y - center.y , edgePoints[1].x - center.x) * RAD2DEG;
        float degree3 = atan2(edgePoints[2].y - center.y , edgePoints[2].x - center.x) * RAD2DEG;
        degree1 = degree1 < 0 ? degree1 + 360 : degree1;
        degree2 = degree2 < 0 ? degree2 + 360 : degree2;
        degree3 = degree3 < 0 ? degree3 + 360 : degree3;
        //the orientation is really important... oh those days of debugging
        bool clockwise = !orientation(edgePoints[0], edgePoints[1], edgePoints[2]);
        if(clockwise){
            degree1 = degree1 < degree3 ? degree1 + 360 : degree1;
            degree2 = degree2 < degree3 ? degree2 + 360 : degree2;
            for(float i = degree1; i > degree3 - (degree1-degree3)/resolution; i-= (degree1-degree3)/resolution){
                if(currentResolution > resolution) break;
                currentResolution++;
                Vector2 tempPoint = Vector2{center.x + cos(i / RAD2DEG) * radius, center.y + sin(i / RAD2DEG) * radius};
                renderPoints.push_back(tempPoint);
            }
        }
        else{
            degree2 = degree2 < degree1 ? degree2 + 360 : degree2;
            degree3 = degree3 < degree1 ? degree3 + 360 : degree3;
            for(float i = degree3; i > degree1 - (degree3-degree1)/resolution; i -= (degree3-degree1)/resolution){
                if(currentResolution > resolution) break;
                currentResolution++;
                Vector2 tempPoint = Vector2{center.x + cos(i / RAD2DEG) * radius, center.y + sin(i / RAD2DEG) * radius};
                renderPoints.push_back(tempPoint);
            }
            //reverse the point because the orientation is reversed
            std::reverse(renderPoints.begin(), renderPoints.end());
        }
    }
    else if(data.curveType == 'C'){
        //OTUR AGLA
        std::__throw_invalid_argument("Haha Eren is too dumb to calculate this");
    }
    else{
        //there shouldnt be a different slider type so this wont be triggered
        std::__throw_invalid_argument("Invalid Slider type!");
    }
    /*the sliders are drawn in an interesting way, it really ressembles the early days of the original osu!,
    just spam circles until there is a curve, but doing this every frame is very cpu/gpu heavy so here, we 
    just sacrifice some ram and vram to create a texture using this technique and then rendering the texture
    when we need the slider, the downside of this is that the sliders look very crude because they are just
    drawn using basic circles with no antialiasing, i can fix that by plotting textures instead of circles but
    to be honest, they look fine just now so i probably wont fix this working code for a while*/ 
    //calculate the texture size using the max and min cords of the points that need to be rendered
    for(int i = 0; i < renderPoints.size(); i++){
        minX = std::min(minX, renderPoints[i].x);
        minY = std::min(minY, renderPoints[i].y);
        maxX = std::max(maxX, renderPoints[i].x);
        maxY = std::max(maxY, renderPoints[i].y);
    }
    //create a texture that can be rendered on, thanks raylib, very cool
    sliderTexture = LoadRenderTexture((maxX-minX+(float)gm->hitCircle.height/2)*gm->windowScale, (maxY-minY+(float)gm->hitCircle.height/2)*gm->windowScale);
    //start to draw on the texture
    BeginTextureMode(sliderTexture);
    //thanks to "tixvage" for reporting this, some gpus dont clear the background automatically for some reason
    ClearBackground(BLANK);
    //just to gain some performance we can skip some of the points (determined by gm->skip)
    for(int i = 0; i < renderPoints.size(); i+=gm->skip)
        DrawCircle((renderPoints[i].x-minX+(float)gm->hitCircle.height/4)*gm->windowScale, sliderTexture.texture.height - (renderPoints[i].y-minY+(float)gm->hitCircle.height/4)*gm->windowScale, (gm->hitCircle.height/4-2)*gm->windowScale,Color{170,170,170,255});
    //draws the last circle because for loops...
    DrawCircle((renderPoints[renderPoints.size()-1].x-minX+(float)gm->hitCircle.height/4)*gm->windowScale, sliderTexture.texture.height - (renderPoints[renderPoints.size()-1].y-minY+(float)gm->hitCircle.height/4)*gm->windowScale, (gm->hitCircle.height/4-2)*gm->windowScale,Color{170,170,170,255});
    //the same deal as above, but for the insides
    for(int i = 0; i < renderPoints.size(); i+=gm->skip)
        DrawCircle((renderPoints[i].x-minX+(float)gm->hitCircle.height/4)*gm->windowScale, sliderTexture.texture.height - (renderPoints[i].y-minY+(float)gm->hitCircle.height/4)*gm->windowScale, (gm->hitCircle.height/4-5)*gm->windowScale,Color{12,12,12,255});
    DrawCircle((renderPoints[renderPoints.size()-1].x-minX+(float)gm->hitCircle.height/4)*gm->windowScale, sliderTexture.texture.height - (renderPoints[renderPoints.size()-1].y-minY+(float)gm->hitCircle.height/4)*gm->windowScale, (gm->hitCircle.height/4-5)*gm->windowScale,Color{12,12,12,255});
    //finalizes the texture
    EndTextureMode();
}

//main code that runs for every slider on screen
void Slider::update(){
    GameManager* gm = GameManager::getInstance();
    //calculates the position of the circle that you need to follow
    position = (gm->currentTime * 1000.f - (float)data.time) / (gm->beatLength) * gm->sliderSpeed * gm->sliderSpeedOverride;// / std::stof(gm->gameFile.configDifficulty["SliderMultiplier"]));
    position *= 100;
    if(gm->currentTime*1000 - data.time > 0){
        //the slider can also run backwards, ADD BEGINNING AND END REVERSING VARIABLES
        if ((int)((gm->currentTime*1000 - data.time) /((data.length/100) * (gm->beatLength) / (gm->sliderSpeed* gm->sliderSpeedOverride))) % 2 == 1)
            position = (int)data.length - ((int)position % (int)data.length + 1); 
        else
            position = (int)position % (int)data.length + 1; 
    }
    //clamps the position variable just in case
    position = std::max(0.f,position);
    //determines if the slider was hit at the beginning, ADD MORE TIMING CONTROL FOR EARLY CLICKS
    if (is_hit_at_first || gm->currentTime*1000 > data.time + gm->gameFile.p100Final)
        state = false;
    //ends the slider
    if(gm->currentTime*1000 > data.time + (data.length/100) * (gm->beatLength) / (gm->sliderSpeed * gm->sliderSpeedOverride) * data.slides){
        data.time = gm->currentTime*1000;
        data.point = 0;
        gm->clickCombo = 0;
        gm->destroyHitObject();
    }
}

//the slider renderer, NEED TO ADD THE TICKS AND MORE ANIMATION STUFF
void Slider::render(){
    GameManager* gm = GameManager::getInstance();
    if(data.curveType == 'L' || data.curveType == 'B' || data.curveType == 'P'){
        if(renderPoints.size() > 0){
            //calculate the opacity
            float clampedFade = gm->clip((gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in,0.0f,0.75f);
            //check if the slider is actually valid
            if(renderPoints.size() > 0)
                DrawTextureEx(sliderTexture.texture, Vector2{(minX-(float)gm->hitCircle.height/4)*gm->windowScale,(minY-(float)gm->hitCircle.height/4)*gm->windowScale},0,1, Fade(WHITE,clampedFade));
            float angle = 0;
            if(renderPoints.size() >= 2){
                angle = atan2(renderPoints[0].y- renderPoints[1].y, renderPoints[0].x - renderPoints[1].x);
                angle = angle * 180 / PI + 180;
            }
            /*DrawTextureEx(gm->reverseArrow, Vector2{
                renderPoints[0].x*gm->windowScale-cos((angle-45)*PI/180)*((gm->reverseArrow.width*0.5f*gm->windowScale/2)/cos(45*PI/180)),
                renderPoints[0].y*gm->windowScale-sin((angle-45)*PI/180)*((gm->reverseArrow.height*0.5f*gm->windowScale/2)/sin(45*PI/180))},
                angle+180,
                gm->windowScale/2,
                Fade(WHITE, clampedFade));*/
            DrawTexturePro(gm->reverseArrow, Rectangle{0,0,gm->reverseArrow.width,gm->reverseArrow.height}, Rectangle{renderPoints[0].x*gm->windowScale,renderPoints[0].y*gm->windowScale,gm->reverseArrow.width*0.5f*gm->windowScale,gm->reverseArrow.height*0.5f*gm->windowScale}, Vector2{gm->reverseArrow.width*0.5f*gm->windowScale/2, gm->reverseArrow.height*0.5f*gm->windowScale/2}, angle, Fade(WHITE, clampedFade));
            //calculate the position, and clamp it
            int calPos = position;
            calPos = std::min(calPos, static_cast<int>(renderPoints.size()-1));
            //draw the follow circle according to the calculated position
            if(gm->currentTime*1000 - data.time > 0 or !state){
                if(data.colour.size() > 2)
                    DrawTextureEx(gm->sliderb, Vector2{renderPoints[calPos].x*gm->windowScale-gm->sliderb.width*0.5f*gm->windowScale/2,renderPoints[calPos].y*gm->windowScale-gm->sliderb.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(Color{(unsigned char)data.colour[0],(unsigned char)data.colour[1],(unsigned char)data.colour[2]}, clampedFade));
                else
                    DrawTextureEx(gm->sliderb, Vector2{renderPoints[calPos].x*gm->windowScale-gm->sliderb.width*0.5f*gm->windowScale/2,renderPoints[calPos].y*gm->windowScale-gm->sliderb.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(WHITE, clampedFade));      
            }
        }
    }
    else{
        //fall back to circle drawing
        float approachScale = 3*(1-(gm->currentTime*1000 - data.time + gm->gameFile.preempt)/gm->gameFile.preempt)+1;
        if (approachScale <= 1)
            approachScale = 1;
        float clampedFade = (gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in;
        if(data.colour.size() > 2)
            DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(Color{(unsigned char)data.colour[0],(unsigned char)data.colour[1],(unsigned char)data.colour[2]}, clampedFade));
        else
            DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(WHITE, clampedFade));
        render_combo();
        DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, clampedFade));
        if(data.colour.size() > 2)
            DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(Color{(unsigned char)data.colour[0],(unsigned char)data.colour[1],(unsigned char)data.colour[2]}, clampedFade));
        else
            DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(WHITE, clampedFade));
    }
    //the "state" determines which circle type must be drawn, if it is "true" it draws the hitcircle
    if(state){
        float approachScale = 3*(1-(gm->currentTime*1000 - data.time + gm->gameFile.preempt)/gm->gameFile.preempt)+1;
        if (approachScale <= 1)
            approachScale = 1;
        float clampedFade = (gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in;
        if(data.colour.size() > 2)
            DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(Color{(unsigned char)data.colour[0],(unsigned char)data.colour[1],(unsigned char)data.colour[2]}, clampedFade));
        else
            DrawTextureEx(gm->hitCircle, Vector2{data.x*gm->windowScale-gm->hitCircle.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircle.height*0.5f*gm->windowScale/2},0,gm->windowScale/2, Fade(WHITE, clampedFade));
        DrawTextureEx(gm->hitCircleOverlay, Vector2{data.x*gm->windowScale-gm->hitCircleOverlay.width*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->hitCircleOverlay.height*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, clampedFade));
        render_combo();
        if(data.colour.size() > 2)
            DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(Color{(unsigned char)data.colour[0],(unsigned char)data.colour[1],(unsigned char)data.colour[2]}, clampedFade));
        else
            DrawTextureEx(gm->approachCircle, Vector2{data.x*gm->windowScale-gm->approachCircle.width*approachScale*0.5f*gm->windowScale/2,data.y*gm->windowScale-gm->approachCircle.height*approachScale*0.5f*gm->windowScale/2},0,approachScale*gm->windowScale/2, Fade(WHITE, clampedFade));
    }
}

//renders the "dead" Slider
void Slider::dead_render(){
    GameManager* gm = GameManager::getInstance();
    //calculates the fading and the size just like the hitCircle, THIS HERE MUST BE DIFFERENT
    float scale = (gm->currentTime*1000 + 400 - data.time )/400;
    float fadeAnimation = 0.3*(1-((gm->currentTime*1000 + 200 - data.time )/200-1));
    float fadePoint = (1-((gm->currentTime*1000 + 400 - data.time )/400-1));
    float movePoint = (((gm->currentTime*1000 + 400 - data.time )/400-1))*20;
    //fades the slider texture, ALSO NEED TO FADE THE OTHER PARTS AND BITS
    DrawTextureEx(sliderTexture.texture, Vector2{(minX-(float)gm->hitCircle.height/4)*gm->windowScale,(minY-(float)gm->hitCircle.height/4)*gm->windowScale},0,1, Fade(WHITE,fadeAnimation));
    //renders the points
    if(data.point == 0)
        DrawTextureEx(gm->hit0, Vector2{renderPoints[position].x*gm->windowScale-gm->hit0.width*1*0.5f*gm->windowScale/2 ,renderPoints[position].y*gm->windowScale-gm->hit0.height*1*0.5f*gm->windowScale/2},(1-fadePoint)*15,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 1)
        DrawTextureEx(gm->hit50, Vector2{(renderPoints[position].x)*gm->windowScale-gm->hit50.width*1*0.5f*gm->windowScale/2 ,(renderPoints[position].y)*gm->windowScale-gm->hit50.height*1*0.5f*gm->windowScale/2 },0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 2)
        DrawTextureEx(gm->hit100, Vector2{renderPoints[position].x*gm->windowScale-gm->hit100.width*1*0.5f*gm->windowScale/2 ,renderPoints[position].y*gm->windowScale-gm->hit100.height*1*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
    else if(data.point == 3)
        DrawTextureEx(gm->hit300, Vector2{renderPoints[position].x*gm->windowScale-gm->hit300.width*1*0.5f*gm->windowScale/2 ,renderPoints[position].y*gm->windowScale-gm->hit300.height*1*0.5f*gm->windowScale/2},0,1*gm->windowScale/2, Fade(WHITE, fadePoint));
}

//gives time to render the "dead" slider
void Slider::dead_update(){
    GameManager* gm = GameManager::getInstance();
    if (data.time+400 < gm->currentTime*1000){
        //prevent memory leak
        UnloadRenderTexture(sliderTexture);
        gm->destroyDeadHitObject();
    }
}

//renders the combo on top of the slider
void Slider::render_combo(){
    GameManager* gm = GameManager::getInstance();
    //same garbage as the hitcircle combo, NEEDS FIXING
    float clampedFade = (gm->currentTime*1000 - data.time  + gm->gameFile.fade_in) / gm->gameFile.fade_in;
    int digits = 1;
    if(data.comboNumber >= 1000)
        digits = 4;
    else if(data.comboNumber >= 100)
        digits = 3;
    else if(data.comboNumber >= 10)
        digits = 2;
    int origin = (gm->numbers[0].width + (digits - 3) * (gm->numbers[0].width - 150)) / 2;
    for(int i = digits; i >= 1 ; i--){
        int number = data.comboNumber;
        if(i == 1)
            number = number % 10;
        else if(i == 2)
            number = (number % 100 - number % 10)/10;
        else if(i == 3)
            number = (number % 1000 - number % 100)/100;
        else if(i == 4)
            number = (number % 10000 - number % 1000)/1000;
        int calPos = position;
        calPos = std::min(calPos, static_cast<int>(renderPoints.size()-1));
        DrawTextureEx(gm->numbers[number], Vector2{renderPoints[0].x*gm->windowScale - origin*gm->windowScale/2 + (digits - i - 1) * (gm->numbers[0].width - 150)*gm->windowScale/2, renderPoints[0].y*gm->windowScale - gm->numbers[0].width*gm->windowScale/2 / 2 },0,gm->windowScale / 2, Fade(WHITE, clampedFade));
    }
}
