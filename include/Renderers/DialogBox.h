#pragma once

#include "./UIScreen.h"

class DialogBox : public UIScreen
{
public:
    DialogBox(class Game* game, const std::wstring& text, std::function<void()> onOK);
    ~DialogBox();

private:

};
