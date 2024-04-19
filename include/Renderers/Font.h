#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>

#include <SDL2/SDL_ttf.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "../include/General/Cmath.h"

class Font
{
private:
    class Game* mGame;
    std::unordered_map<int, TTF_Font*> mFontData;

public:
    Font(class Game* game);
    ~Font();

    bool load(const std::string& fileName);
    void unLoad();

    class Texture* renderText(const std::string& text, Vector3 color = Color::Red, int pointSize = 30);
};