#include "GameManager.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>


GameManager* GameManager::inst_ = NULL;

GameManager* GameManager::getInstance() {
   if (inst_ == NULL) {
      inst_ = new GameManager();
   }
   return(inst_);
}

GameManager::GameManager(){
	init();
}

void GameManager::init(){
	InitWindow(1280,960,"osus?");
	SetTargetFPS(60);
	HideCursor();
	
	hitCircle = LoadTexture("../skin/hitcircle.png");
    hitCircleOverlay = LoadTexture("../skin/hitcircleoverlay.png");
    approachCircle = LoadTexture("../skin/approachcircle.png");
    cursor = LoadTexture("../skin/Extra Cursors/cursor.png");
}

void GameManager::update(){
	currentTime = GetTime();

	int size = gameFile.hitObjects.size();
	for(int i = size-1; i >= 0; i--){
		if(gameFile.hitObjects[i].time - gameFile.preempt <= currentTime*1000){
			//spawn Circle
			spawnHitObject(gameFile.hitObjects[i]);
			gameFile.hitObjects.pop_back();
		}
		//else break;
	}
	bool pressed = IsMouseButtonPressed(0); 
	for(int i = 0; i < objects.size(); i++){
		if(pressed && i == 0){

			if (CheckCollisionPointCircle(Vector2{(float)GetMouseX(), (float)GetMouseY()},Vector2{(float)objects[i]->data.x*2,(float)objects[i]->data.y*2}, 56) && pressed){
				destroyHitObject();
			}
		}else{
			objects[i]->update();
		}
	}

	//dead animations
	for(int i = 0; i < dead_objects.size(); i++){
		dead_objects[i]->dead_update();
	}
}

void GameManager::render(){
	BeginDrawing();
	ClearBackground(BLACK);
	DrawFPS(10, 10);

	float scale = 0.6f;

	DrawTextureEx(cursor, Vector2{GetMouseX()-cursor.width*scale*0.5f,GetMouseY()-cursor.height*scale*0.5f},0,scale, WHITE);
	
	for(int i = objects.size() - 1; i >= 0; i--){
		objects[i]->render();
	}

	//dead animations
	for(int i = dead_objects.size() - 1; i >= 0; i--){
		dead_objects[i]->dead_render();
	}
	//DrawTexture(cursor,GetMouseX(), GetMouseY(), RED);
	EndDrawing();
}

void GameManager::run(){
	while(!WindowShouldClose()){
		update();
		render();
	}

	CloseWindow();
}

void GameManager::loadGame(std::string filename){
	Parser parser = Parser();
	gameFile = parser.parse(filename);

	std::reverse(gameFile.hitObjects.begin(),gameFile.hitObjects.end());

	if(std::stoi(gameFile.configDifficulty["ApproachRate"]) < 5){
		gameFile.preempt = 1200 + 600 * (5 - std::stoi(gameFile.configDifficulty["ApproachRate"])) / 5;
		gameFile.fade_in = 800 + 400 * (5 - std::stoi(gameFile.configDifficulty["ApproachRate"])) / 5;
	}
	else if(std::stoi(gameFile.configDifficulty["ApproachRate"]) < 5){
		gameFile.preempt = 1200 - 750 * (std::stoi(gameFile.configDifficulty["ApproachRate"]) - 5) / 5;
		gameFile.fade_in = 800 - 500 * (std::stoi(gameFile.configDifficulty["ApproachRate"]) - 5) / 5;
	}
	else{
		gameFile.preempt = 1200;
		gameFile.fade_in = 800;
	}
}

void GameManager::spawnHitObject(HitObjectData data){
	Circle *temp = new Circle(data);
	objects.push_back(temp);
}

void GameManager::destroyHitObject(){
	dead_objects.push_back(objects[0]);
	objects.erase(objects.begin());
}

void GameManager::destroyDeadHitObject(){
	delete dead_objects[0];
	dead_objects.erase(dead_objects.begin());
}