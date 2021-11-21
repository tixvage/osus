#include "GameManager.h"

int main(){
    GameManager *gameManager = GameManager::getInstance();
    gameManager->loadGame("linearcurve.osu");
    gameManager->run();
    return 0;
}