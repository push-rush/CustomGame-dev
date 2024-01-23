#pragma once

#include <iostream>

#include <SDL.h>

#include "./Button.h"



class TextButton : public Button
{
private:
    Vector3 mBoxColor;
    Vector3 mBGColor;

    int mMaxTextLength;
    std::wstring mModText;
    std::vector<class Texture*> mModTextTextures;
    Vector2 mModTextSize;

    Uint32 mCurTimestamp;

public:
    TextButton(class UIScreen* ui, 
        const std::string& name, 
        const std::string& type, 
        const std::map<std::string, std::string>& bindTexName, 
        const std::wstring& text, const Vector3& color, const int& fontSize,
        const int& bind_event,
        std::function<void()> onClick, 
        const Vector2& pos, 
        const Vector2& dims, 
        const Vector2& scale,
        const bool& addIn = true
    );
    ~TextButton();

    void Rename(class SDL_Window* window, class Shader* spriteShader, class Shader* fontShader);
    void setBgColor(const Vector3& color);
    void setBoxColor(const Vector3& color);
    void setTextLength(const int& len);
};