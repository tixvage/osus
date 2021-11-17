#include "Game.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>

void Game::init(GameFile gf){
	InitWindow(1280,960,"osus?");
	SetTargetFPS(60);
	HideCursor();

	gameFile = gf;
	std::reverse(gameFile.hitObjects.begin(),gameFile.hitObjects.end());
	hitCircle = LoadTexture("../skin/hitcircle.png");
    hitCircleOverlay = LoadTexture("../skin/hitcircleoverlay.png");
    approachCircle = LoadTexture("../skin/approachcircle.png");
    cursor = LoadTexture("../skin/Extra Cursors/cursor.png");
}

void Game::update(){
	currentTime = GetTime();
}

void Game::render(){
	BeginDrawing();
	ClearBackground(BLACK);
	DrawFPS(10, 10);
	//DrawCircle(GetMouseX(), GetMouseY(), 10, RED);
	float scale = 0.6f;
	
	int preempt;
	int fade_in;
	
	if(std::stoi(gameFile.configDifficulty["ApproachRate"]) < 5){
		preempt = 1200 + 600 * (5 - std::stoi(gameFile.configDifficulty["ApproachRate"])) / 5;
		fade_in = 800 + 400 * (5 - std::stoi(gameFile.configDifficulty["ApproachRate"])) / 5;
	}
	else if(std::stoi(gameFile.configDifficulty["ApproachRate"]) < 5){
		preempt = 1200 - 750 * (std::stoi(gameFile.configDifficulty["ApproachRate"]) - 5) / 5;
		fade_in = 800 - 500 * (std::stoi(gameFile.configDifficulty["ApproachRate"]) - 5) / 5;
	}
	else{
		preempt = 1200;
		fade_in = 800;
	}

	DrawTextureEx(cursor, Vector2{GetMouseX()-cursor.width*scale*0.5f,GetMouseY()-cursor.height*scale*0.5f},0,scale, WHITE);
	int size = gameFile.hitObjects.size();
	for(int i = size-1; i >= 0; i--){
		if(gameFile.hitObjects[i].time - preempt <= currentTime*1000 and gameFile.hitObjects[i].time > currentTime*1000){
			float approachScale = 3*(1-(currentTime*1000 - gameFile.hitObjects[i].time + preempt)/preempt)+1;
			float clampedFade = (currentTime*1000 - gameFile.hitObjects[i].time + fade_in) / fade_in;
			DrawTextureEx(hitCircle, Vector2{gameFile.hitObjects[i].x*2-hitCircle.width*0.5f,gameFile.hitObjects[i].y*2-hitCircle.height*0.5f},0,1, Fade(PINK, clampedFade));
			DrawTextureEx(hitCircleOverlay, Vector2{gameFile.hitObjects[i].x*2-hitCircleOverlay.width*0.5f,gameFile.hitObjects[i].y*2-hitCircleOverlay.height*0.5f},0,1, Fade(WHITE, clampedFade));
			DrawTextureEx(approachCircle, Vector2{gameFile.hitObjects[i].x*2-approachCircle.width*approachScale*0.5f,gameFile.hitObjects[i].y*2-approachCircle.height*approachScale*0.5f},0,approachScale, Fade(WHITE, clampedFade));
		}
		//else break;
	}
	
	//DrawTexture(cursor,GetMouseX(), GetMouseY(), RED);
	EndDrawing();
}

void Game::run(){
	while(!WindowShouldClose()){
		update();
		render();
	}

	CloseWindow();
}