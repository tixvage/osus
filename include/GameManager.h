#pragma once
#include <Parser.h>
#include <raylib.h>

class GameManager{
	public:
		static GameManager* getInstance();
		GameManager();
		void run();
		void loadGame(std::string filename);
		void destroyHitObject();
		void destroyDeadHitObject();
		Texture2D hitCircle;
		Texture2D hitCircleOverlay;
		Texture2D approachCircle;
		Texture2D cursor;
		Texture2D selectCircle;
		Texture2D hit0;
		Texture2D hit50;
		Texture2D hit100;
		Texture2D hit300;
		double currentTime;
		GameFile gameFile;
		Parser parser;
		std::vector<HitObject*> objects;
		std::vector<HitObject*> dead_objects;
	private:
		static GameManager* inst_;
		void init();
		void render();
		void update();
		void spawnHitObject(HitObjectData data);
		
		
};