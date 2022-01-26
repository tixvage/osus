#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <raylib.h>
#include <climits>

struct HitObjectData{
    int x;
    int y;
    int time;
    int type;
    int hitSound;

    int point = 0;

    char curveType;
    std::vector<std::pair<int,int> > curvePoints;
    int slides;
    float length;
    std::vector<int> edgeSounds;
    std::vector<std::pair<int, int> > edgeSets;

    int endTime;

    int normalSet;
    int additionSet;
    int index;
    int volume;
    std::string filename;
    bool startingACombo;
    int skipComboColours = 0;
    int comboNumber;
    bool useDefaultHitSound;
    std::vector<int> colour;
};

class HitObject{
    public:
        virtual void init(){}

        virtual void render(){}
        virtual void render_combo(){}
        virtual void dead_render(){}

        virtual void update(){}
        virtual void dead_update(){}
        

        HitObjectData data;
        float x,y;
};

class Circle : public HitObject{
    public:
        Circle(HitObjectData data);
        void init() override; 

        void render() override;
        void render_combo() override;
        void dead_render() override;

        void update() override;
        void dead_update() override;
};

class Slider : public HitObject{
    public:
        Slider(HitObjectData data);
        void init() override; 
        
        void render() override;
        void render_combo() override;
        void dead_render() override;

        void update() override;
        void dead_update() override;
        
        bool is_hit_at_first = false;
        float position = 0;

        bool state = true;
        std::vector<Vector2> renderPoints; 
        
    private:
        float timer;

        std::vector<Vector2> edgePoints; 
        RenderTexture2D sliderTexture;
        
        float minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
};