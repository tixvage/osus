#include "Game.h"



void Game::init(){
	InitWindow(1280,960,"osus?");
	SetTargetFPS(60);
	HideCursor();
	hitCircle = LoadTexture("../skin/hitcircle.png");
    hitCircleOverlay = LoadTexture("../skin/hitcircleoverlay.png");
    approachCircle = LoadTexture("../skin/approachcircle.png");
    cursor = LoadTexture("../skin/Extra Cursors/cursor.png");
}

void Game::update(){
	
}

void Game::render(){
	BeginDrawing();
	ClearBackground(BLACK);
	//DrawCircle(GetMouseX(), GetMouseY(), 10, RED);
	float scale = 0.6f;
	float approachScale = scale*5.0f;
	DrawTextureEx(cursor, Vector2{GetMouseX()-cursor.width*scale*0.5f,GetMouseY()-cursor.height*scale*0.5f},0,scale, WHITE);
	DrawTextureEx(hitCircle, Vector2{640-hitCircle.width*0.5f,480-hitCircle.height*0.5f},0,1, GREEN);
	DrawTextureEx(hitCircleOverlay, Vector2{640-hitCircleOverlay.width*0.5f,480-hitCircleOverlay.height*0.5f},0,1, WHITE);
	DrawTextureEx(approachCircle, Vector2{640-approachCircle.width*approachScale*0.5f,480-approachCircle.height*approachScale*0.5f},0,approachScale, Fade(WHITE, 0.2f));
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