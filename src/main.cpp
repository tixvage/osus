#include "GameManager.h"

int main(){
    GameManager *gameManager = GameManager::getInstance();
    gameManager->loadGame("gravitymakesthingsfall.osu");
    gameManager->run();
    return 0;
}