#pragma once
#include "Parser.h"
#include <raylib.h>

class GameManager{
	public:
		static GameManager* getInstance();
		GameManager();
		void run();
		void loadGame(std::string filename);
		void destroyHitObject();
		void destroyDeadHitObject();

		float windowScale = 2.0f;
		int skip = 2;
		Texture2D hitCircle;
		Texture2D hitCircleOverlay;
		Texture2D approachCircle;
		Texture2D cursor;
		Texture2D selectCircle;
		Texture2D hit0;
		Texture2D hit50;
		Texture2D hit100;
		Texture2D hit300;
		Texture2D sliderb;
		
		Texture2D numbers[10];

		Music backgroundMusic;

		double currentTime;
		int combo = 1;
		int clickCombo = 0;
		Color comboColour;
		int score = 0;
		float difficultyMultiplier = 0;
		int currentComboIndex = 0;

		int time;
		int meter;
		float beatLength;
		int sampleSet;
		int sampleIndex;
		int volume;
		bool uninherited;
		int effects;
		float sliderSpeed = 1;
		float sliderSpeedOverride = 1;
		bool pressed = false;
		bool down = false;
		Vector2 MousePosition;

		GameFile gameFile;
		Parser parser;
		std::vector<HitObject*> objects;
		std::vector<HitObject*> dead_objects;
		float clip(float value, float min, float max);
	private:
		static GameManager* inst_;
		void init();
		void render();
		void update();
		void spawnHitObject(HitObjectData data);
		void render_points();
		void render_combo();
		
};