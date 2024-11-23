#include "game.h"
#include <algorithm>
int main(){
    Game game;

    while (!game.GetWindow()->IsDone()) {
       //Game loop
        game.Handling();
        game.Update(); //we update twice for some reason, else it will be slow
        game.Render();
        game.RestartClock();
    }

    return 0;
}


