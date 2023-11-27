#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <functional>
#include <unordered_map>

#include "./General/Cmath.h"

class UIScreen
{
public:
    UIScreen(class Game* game);
    virtual ~UIScreen();

    enum UIType
    {
        EHUD,
        EPauseMenu,
        EDialogBox,
        ESetting
    };

    enum UIState
    {
        EActive,
        EClosing
    };

    enum UIButtonType
    {
        RectButtonON,
        RectButtonOFF,
        HookButtonON,
        HookButtonOFF,
        SoundSliderOn,
        SoundSliderOFF,
        CrossOn,
        CrossOFF,
        SoundSlider,
        NUM_Buttons
    };

    virtual void update(float dt);
    virtual void draw(class Shader* spriteShader, class Shader* textShader);
    virtual void processInput(const uint8_t* keys);
    virtual void handleKeyPress(int key);

    // setters
    void close();
    void setFont(class Font* font);
    void setTitlePos(Vector2);
    void setUIState(UIState);
    void setBGPos(Vector2);
    void setNextButtonPos(Vector2);
    void setRelativeMouseMode(bool relative);
    void setTitleTexture(const std::wstring& text, 
        const Vector3& color = Color::White, int pointSize = 40);
    void setTextColor(const Vector3& color);
    void setButtonPos(const Vector2& pos);
    void setUIType(UIType type);

    // getters
    UIState getState() const;
    UIType getUIType() const;
    class Game* getGame() const;
    class Font* getFont() const;
    std::vector<class Texture*>& getTexture();
    Vector2 getTitlePos() const;
    Vector2 getBGPos() const;
    Vector2 getNextButtonPos() const;
    UIState getUIState() const;
    Vector3 getTextColor() const;
    Vector2 getButtonPos() const;
    std::vector<class Texture*>& getBackgrounds();
    std::unordered_map<UIButtonType, class Texture*>& getButtonTexs();
    std::vector<class Button*>& getButtons();

    // add or remove
    void addBackground(class Texture* tex);
    void addButton(const UIButtonType& type, const Vector2& pos, const Vector2& scale = Vector2{1.0f, 1.0f});
    void addButton(const UIButtonType& type, const Vector2& pos, 
                    std::function<void()> onClick, const Vector2& scale = Vector2{1.0f, 1.0f});
    void addButton(const UIButtonType& type, const std::wstring& name, const Vector3& color, 
                    std::function<void()> onClick, const Vector2& scale = Vector2{1.0f, 1.0f});
    void addButton(const UIButtonType& type, const std::wstring& name, const Vector3& color, 
                    const Vector2& pos, std::function<void()> onClick, const Vector2& scale = Vector2{1.0f, 1.0f});

    // 屏幕ui绘制
    void drawScreen();

    // 绘制纹理
    void drawTexture(class Shader* shader, class Texture* texture,
                        const Vector2& offset = Vector2::Zero, float scaleX = 1.0f, float scaleY = 1.0f, bool flipY = false, bool drawFont = false);

    // 绘制文本纹理
    void drawTextures(class Shader* shader, std::vector<class Texture*>& textures,
                        const Vector2& offset = Vector2::Zero, float scaleX = 1.0f, float scaleY = 1.0f, bool flipY = false, bool drawFont = false);

    // 创建字形字体顶点布局
    class VertexArray* createFontVerts(bool draw_font, const Vector2& pos, const float& w, const float& h);

private:
    UIState mState;
    UIType mUIType;
    class Game* mGame;
    class Font* mFont;
    class FreeTypeFont* mFreeTypeFont;
    std::vector<class Texture*> mTitleTextures;
    class VertexArray* mFontVerts;
    class VertexArray* mSpriteVerts;
    Vector2 mTextSize;
    Vector3 mTextColor;

    class Texture* mButtonOn;
    class Texture* mButtonOff;
    class Texture* mSoundSlider;
    class Texture* mSoundSliderOn;
    class Texture* mSoundSliderOff;
    class Texture* mHookButtonOff;
    class Texture* mHookButtonOn;
    class Texture* mCrossOff; 
    class Texture* mCrossOn;

    Vector2 mBGPos;
    Vector2 mTitlePos;
    Vector2 mNextButtonPos;
    std::vector<class Texture*> mBackgrounds;

    std::vector<class Button*> mButtons;
    std::unordered_map<UIButtonType, class Texture*> mButtonTexs;
};