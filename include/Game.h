#pragma once
#include <GameFile.h>
#include <raylib.h>

class Game{
	public:
		Game(){init();}
		~Game(){};
		void run();
		Texture2D hitCircle;
		Texture2D hitCircleOverlay;
		Texture2D approachCircle;
		Texture2D cursor;
	private:
		void init();
		void render();
		void update();
};