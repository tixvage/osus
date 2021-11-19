#include "GameManager.h"

int main(){
    GameManager *gameManager = GameManager::getInstance();
    gameManager->loadGame("padoru.osu");
    gameManager->run();
    return 0;
}

/*


x  ,y  ,time ,type,hitSound,curveType|curvePoints,slides,length,edgeSounds,edgeSets,hitSample
256,192,11000,21  ,2
204,127,41984,6   ,0       ,L        |187:297    ,1     ,160
177,242,611  ,6   ,0       ,L        |291:237    ,1     ,10,   ,6|2       ,1:2|0:0 ,0:0:0:0:



*/