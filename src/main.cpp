#include "GameManager.h"

int main(){
    GameManager *gameManager = GameManager::getInstance();
    gameManager->loadGame("padoru.osu");
    gameManager->run();
    return 0;
}