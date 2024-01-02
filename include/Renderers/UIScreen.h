#pragma once

#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <stack>

#include "./General/Cmath.h"

// namespace Renderer2D
// {
    class UIScreen
    {
    public:
        UIScreen(class Game* game);
        virtual ~UIScreen();

        // struct UIElement
        // {
        //     int mUpdateOrder;
        //     std::vector<class Texture*> mTextures;
        //     std::wstring* mBindText;
        //     class Button* mBindButton;

        //     UIElement()
        //     {
        //         this->mUpdateOrder = 100;
        //         this->mBindButton = nullptr;
        //     }

        //     UIElement(int updateOrder, 
        //             const std::vector<class Texture*>& tex, 
        //             class Button* bindBn = nullptr, 
        //             std::wstring* bindText = nullptr
        //     )
        //     {
        //         this->mUpdateOrder = updateOrder;
        //         this->mBindButton = bindBn;
        //         this->mBindText = bindText;
        //         for (auto t : tex)
        //         {
        //             if (t)
        //             {
        //                 this->mTextures.emplace_back(t);
        //             }
        //         }    
        //     }
        // };

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

        enum UIBindEvent
        {
            UICancel,
            UIConfirm,
            UIClose,
            UIQuit,
            UISetVolume,
            OpenMenuLayer1,
            OpenMenuLayer2_1,
            OpenMenuLayer2_2,
            OpenMenuLayer3_1
        };

        // enum UIButtonType
        // {
        //     RectButtonON,
        //     RectButtonOFF,
        //     HookButtonON,
        //     HookButtonOFF,
        //     SoundSliderOn,
        //     SoundSliderOFF,
        //     CrossOn,
        //     CrossOFF,
        //     VolumeOn,
        //     VolumeOFF,
        //     SoundSlider,
        //     NUM_Buttons
        // };

        virtual void update(float dt);
        virtual void draw(class Shader* spriteShader, class Shader* textShader, class EmptySprite* elem = nullptr);
        virtual void processInput(const uint8_t* keys);
        virtual void handleKeyPress(int key);
        virtual void close();

        // setters
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
        Vector2 getTitlePos() const;
        Vector2 getBGPos() const;
        Vector2 getNextButtonPos() const;
        UIState getUIState() const;
        Vector3 getTextColor() const;
        Vector2 getButtonPos() const;
        std::vector<class EmptySprite*>& getUIElements();
        std::unordered_map<std::string, class Texture*>& getUITextures();
        class VertexArray* getSpriteVerts() const;

        // std::vector<class Texture*>& getBackgrounds();
        // std::vector<class Button*>& getButtons();
        // std::vector<class Texture*>& getTexture();
        // std::unordered_map<UIButtonType, class Texture*>& getButtonTexs();

        // add or remove
        // void addBackground(class Texture* tex);
        // void addButton(const std::string& type, const Vector2& pos, const Vector2& scale = Vector2{1.0f, 1.0f});

        class Button* addButton(
            const std::string& name, const std::string& type, const std::map<std::string, 
            std::string>& bindTex, const Vector2& pos, const UIBindEvent& bind_event, 
            std::function<void()> onClick, const bool& addIn = true, const Vector2& scale = Vector2{1.0f, 1.0f});

        class Button* addButton(
            const std::string& name, const std::string& type, 
            const std::map<std::string, std::string>& bindTex,
            const std::wstring& text, const Vector3& color, const int& fontSize,
            const Vector2& pos, const UIBindEvent& bind_event, 
            std::function<void()> onClick, const bool& addIn = true, const Vector2& scale = Vector2{1.0f, 1.0f});

        // void addButton(const std::string& type, const std::wstring& name, const Vector3& color, 
        //                 std::function<void()> onClick, const Vector2& scale = Vector2{1.0f, 1.0f});
        // void addButton(const std::string& type, const std::wstring& name, const Vector3& color, 
        //                 const Vector2& pos, std::function<void()> onClick, const Vector2& scale = Vector2{1.0f, 1.0f});
        
        void addUIElement(class EmptySprite* sprite);
        void removeElement(class EmptySprite* sprite);
        void addUITexture(const std::string& name, class Texture* tex);
        void removeTexture(const std::string& name);

        // 屏幕ui绘制
        // void drawScreen();

        // 绘制纹理
        void drawTexture(class Shader* shader, class Texture* texture,
                            const Vector2& offset = Vector2::Zero, float scaleX = 1.0f, 
                            float scaleY = 1.0f, bool flipY = false, bool drawFont = false);

        // 绘制文本纹理
        void drawTextures(class Shader* shader, std::vector<class Texture*>& textures,
                            const Vector2& offset = Vector2::Zero, float scaleX = 1.0f, 
                            float scaleY = 1.0f, bool flipY = false, bool drawFont = false);

        // 创建字形字体顶点布局
        class VertexArray* createFontVerts(bool draw_font, const Vector2& pos, const float& w, const float& h);

        // 事件绑定
        virtual void bindEvent(const UIBindEvent& event);

    private:
        UIState mState;
        UIType mUIType;
        class Game* mGame;
        class Font* mFont;
        // class FreeTypeFont* mFreeTypeFont;
        std::vector<class Texture*> mTitleTextures;
        class VertexArray* mFontVerts;
        class VertexArray* mSpriteVerts;
        Vector2 mTextSize;
        Vector3 mTextColor;

        // class Texture* mButtonOn;
        // class Texture* mButtonOff;
        // class Texture* mSoundSlider;
        // class Texture* mSoundSliderOn;
        // class Texture* mSoundSliderOff;
        // class Texture* mHookButtonOff;
        // class Texture* mHookButtonOn;
        // class Texture* mCrossOff; 
        // class Texture* mCrossOn;
        // class Texture* mVolumeOff;
        // class Texture* mVolumeOn;

        Vector2 mBGPos;
        Vector2 mTitlePos;
        Vector2 mNextButtonPos;

        // std::vector<class Texture*> mBackgrounds;
        // std::vector<class Button*> mButtons;
        // std::unordered_map<UIButtonType, class Texture*> mButtonTexs;

        // ui元素
        std::vector<class EmptySprite*> mUIElements;
        // std::multimap<std::string, UIElement*> mUIElements;

        // ui纹理集合
        std::unordered_map<std::string, class Texture*> mUITextures;

    };
// }