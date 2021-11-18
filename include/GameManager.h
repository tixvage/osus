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
		Texture2D hitCircle;
		Texture2D hitCircleOverlay;
		Texture2D approachCircle;
		Texture2D cursor;

		double currentTime;
		GameFile gameFile;
		Parser parser;
		std::vector<HitObject*> objects;
	private:
		static GameManager* inst_;
		void init();
		void render();
		void update();
		void spawnHitObject(HitObjectData data);
		
		
};