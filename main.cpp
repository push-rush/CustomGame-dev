#include <iostream>

#include "./include/Game.h"

using namespace std;

int main(int argc, char** argv)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // ofstream fout("../Log.txt");
    // auto old_cout = cout.rdbuf(fout.rdbuf());

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