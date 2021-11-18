#include "GameManager.h"

int main(){
    GameManager gameManager;
    gameManager.loadGame("BeautifulWorld.osu");
    gameManager.run();
    return 0;
}