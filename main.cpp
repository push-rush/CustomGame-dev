#include <iostream>

#include "./include/Game.h"

using namespace std;

int WinMain(int argc, char** argv)
{
    ofstream fout("../Log.txt");
    auto old_cout = cout.rdbuf(fout.rdbuf());

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