#pragma once
#include <GameFile.h>
#include <raylib.h>

class Game{
	public:
		Game(GameFile gf){init(gf);}
		~Game(){};
		void run();
		Texture2D hitCircle;
		Texture2D hitCircleOverlay;
		Texture2D approachCircle;
		Texture2D cursor;
	private:
		void init(GameFile gf);
		void render();
		void update();
		double currentTime;
		GameFile gameFile;
};