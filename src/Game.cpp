#include "Game.h"
#include <algorithm>
#include <iostream>
#include <vector>

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
	//DrawCircle(GetMouseX(), GetMouseY(), 10, RED);
	float scale = 0.6f;
	float approachScale = scale*5.0f;
	DrawTextureEx(cursor, Vector2{GetMouseX()-cursor.width*scale*0.5f,GetMouseY()-cursor.height*scale*0.5f},0,scale, WHITE);
	int size = gameFile.hitObjects.size();
	for(int i = size-1; i >= 0; i--){
		if(gameFile.hitObjects[i].time<currentTime*1000){
			DrawTextureEx(hitCircle, Vector2{gameFile.hitObjects[i].x-hitCircle.width*0.5f,gameFile.hitObjects[i].y-hitCircle.height*0.5f},0,1, GREEN);
			DrawTextureEx(hitCircleOverlay, Vector2{gameFile.hitObjects[i].x-hitCircleOverlay.width*0.5f,gameFile.hitObjects[i].y-hitCircleOverlay.height*0.5f},0,1, WHITE);
			DrawTextureEx(approachCircle, Vector2{gameFile.hitObjects[i].x-approachCircle.width*approachScale*0.5f,gameFile.hitObjects[i].y-approachCircle.height*approachScale*0.5f},0,approachScale, Fade(WHITE, 0.2f));
		}
		else break;
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