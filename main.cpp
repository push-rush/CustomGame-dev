#include <iostream>

#include "./include/Game.h"

using namespace std;

int WinMain(int argc, char** argv)
{
    Game game;

    // std::cout << "游戏初始化..." << std::endl;
    bool success = game.init();
    if (success)
    {
        game.runLoop();
    }
    else
    {
        cout << "Bugs occurring..." << endl;
    }
    game.shutdown();

    return 0;
}