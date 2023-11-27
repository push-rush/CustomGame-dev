#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>
// #include <wchar.h>

#include <glew.h>
#include <glfw3.h>

// glm
#include "../../External/glm/glm/glm.hpp"
#include "../../External/glm/glm/gtc/matrix_transform.hpp"

#include "../include/General/Cmath.h"
#include "../include/Renderers/Texture.h"

struct Character
{
    GLuint TextureID;   // 字形纹理ID
    glm::ivec2 Size;    // 字形大小
    glm::ivec2 Bearing; // 从基准线到字形左部/顶部的距离
    GLint64 Advance;    // 原点距下一个字形原点的距离
    GLubyte Height;     // 字体大小
};

class FreeTypeFont
{
private:
    class Game* mGame;
    FT_Face* mFontFace;
    std::unordered_map<wchar_t, Character*> mCharacters;
    std::string mFontFile;
    FT_Library mFT_Library;

public:
    FreeTypeFont(class Game* game);
    ~FreeTypeFont();

    void unLoad();
    void load(const std::string& fontFile);

    Character* loadWChar(const wchar_t& wch, const GLubyte& fontSize = 30);
    static void createFromSurface(FT_Face* surface, GLuint& texture_id);
    Vector2 renderText(std::vector<class Texture*>& textures, const wchar_t* text, Vector3 color = Color::Red, int pointSize = 30);
};