#pragma once

#include "./UIScreen.h"

class PauseMenu : public UIScreen
{
public:
    PauseMenu(class Game* game);
    ~PauseMenu();

    void handleKeyPress(int key);

private:
    
    
};