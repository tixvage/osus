#include "GameManager.h"

int main(){
    GameManager *gameManager = GameManager::getInstance();
    gameManager->loadGame("swing.osu");
    gameManager->run();
    return 0;
}