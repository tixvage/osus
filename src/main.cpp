#include "GameManager.h"

int main(){
    GameManager *gameManager = GameManager::getInstance();
    gameManager->loadGame("chekibreki.osu");
    gameManager->run();
    return 0;
}