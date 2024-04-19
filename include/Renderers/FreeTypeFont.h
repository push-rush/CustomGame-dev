/*
 * @Author: push-rush 1441488761@qq.com
 * @Date: 2024-04-19 11:38:25
 * @LastEditors: push-rush 1441488761@qq.com
 * @LastEditTime: 2024-04-19 20:11:18
 * @FilePath: /CustomGame-dev/include/Renderers/FreeTypeFont.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>

#include <freetype/freetype.h>

#include <GL/glew.h>

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
    FreeTypeFont(const std::string& path);
    ~FreeTypeFont();

    void unLoad();
    void load(const std::string& fontFile);

    static void createFromSurface(FT_Face* surface, GLuint& texture_id);
    Character* loadWChar(const wchar_t& wch, const GLubyte& fontSize = 30);
    Character* renderFont(const wchar_t& wch, const Vector3& color = Color::White, const int& size = 30);
    class Texture* getFontTexture(class Character* ch, const Vector2& offset);
    Vector2 renderText(std::vector<class Texture*>& textures, const wchar_t* text, Vector3 color = Color::Red, int pointSize = 30);
};