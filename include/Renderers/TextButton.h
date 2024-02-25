#pragma once

#include <iostream>

#include <SDL.h>

#include "./Button.h"

class TextButton : public Button
{
private:
    Vector3 mTextBoxColor;
    Vector3 mTextBGColor;

    int mMaxTextLength;
    std::wstring mModText;
    std::vector<class Texture*> mModTextTextures;
    Vector2 mModTextSize;

    Uint32 mCurTimestamp;

    Vector2 mLinkButtonPos;
    class TextButton* mLinkTextButton;

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

    TextButton(class UIScreen* ui,
        const std::string& name, const std::string& type,
        const Vector2& pos, const Vector2& dims, const Vector2& scale, 
        const Vector3& bgColor, const Vector3& boxColor,
        const std::wstring& text, const Vector3& color, const int& fontSize, const Vector2& bindTextOffset,
        const std::map<std::string, std::string>& bindTexName, const Vector2& bindTexOffset,
        const int& bind_event,
        std::function<void()> onClick,
        const bool& addIn = true
    );

    ~TextButton();

    // setters
    void setTextBgColor(const Vector3& color);
    void setTextBoxColor(const Vector3& color);
    void setTextLength(const int& len);
    void setLinkButtonPos(const Vector2& pos);
    void setLinkTextButton(TextButton* tb);

    // getters
    const Vector2& getLinkButtonPos() const;
    TextButton* getLinkTextButton();

    void Rename(class SDL_Window* window, class Shader* basicShader, class Shader* spriteShader, class Shader* fontShader);
    bool checkModTextValid();
};