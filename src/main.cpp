#include "GameManager.h"

int main(){
    GameManager *gameManager = GameManager::getInstance();
    gameManager->loadGame("llvl.osu");
    gameManager->run();
    return 0;
}