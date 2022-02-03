#include "GameManager.h"

int main(){
    GameManager *gameManager = GameManager::getInstance();
    //load the beatmap and run it, no questions asked
    std::cout << "PRESS LEFT TO LOAD THE MAP" << std::endl;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(Color{5,0,15,255});
        DrawFPS(10, 10);
        DrawText("Try to close the window to load the game (trust me its gonna work)", 10, 30, 20, BLUE);
        EndDrawing();
    }
    gameManager->loadGame("redo.osu");
    std::cout << "PRESS RIGHT TO START" << std::endl;
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(Color{5,0,15,255});
        DrawFPS(10, 10);
        DrawText("Game Loaded!", 10, 30, 20, GREEN);
        DrawText("Try to close the window again to start the game", 10, 50, 20, BLUE);
        EndDrawing();
    }
    gameManager->run();
    return 0;
}