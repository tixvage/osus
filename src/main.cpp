#include "GameManager.h"

int main(){
    GameManager *gameManager = GameManager::getInstance();
    gameManager->loadGame("megolonavya.osu");
    gameManager->run();
    return 0;
}