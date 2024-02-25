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

#include "../General/Cmath.h"

#include "./VertexArray.h"

// namespace Renderer2D
// {
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
            ESetting,
            EResourceMenu,
            EConsole,
            EPropertyMenu
        };

        enum UIState
        {
            EActive,
            EClosing,
            EInvisible,
            EActiviting,
            EVisible
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
            OpenMenuLayer3_1,
            OpenChildren,
            UIRename,
            UICreateCollection
        };

        virtual void update(float dt);
        // virtual void draw(
        //     class Shader* spriteShader, class Shader* textShader, 
        //     class EmptySprite* elem = nullptr, 
        //     const Vector2& offset = Vector2{0.0f, 0.0f}
        // );

        virtual void draw(
            class Shader* basicShader, 
            class Shader* spriteShader, 
            class Shader* fontShader,
            class EmptySprite* elem
        );

        virtual void processInput(const uint8_t* keys);
        virtual void handleKeyPress(int key);
        virtual void handleMouseWheel(const int& mouse_wheel);
        virtual void close();

        Vector2 mousePosRemapping(const Vector2& pos);

        // setters
        void setFont(class Font* font);
        void setTitlePos(Vector2);
        void setUIState(const UIState& state);
        void setBGPos(Vector2);
        // void setNextButtonPos(Vector2);
        void setRelativeMouseMode(bool relative);
        void setTitleTexture(const std::wstring& text, 
            const Vector3& color = Color::White, int pointSize = 40);
        void setTextColor(const Vector3& color);
        // void setButtonPos(const Vector2& pos);
        void setUIType(UIType type);

        void setBindFrameBuffer(const unsigned int& buffer);
        void setBindTexture(class Texture* tex);
        void setUIPosOffset(const Vector2& offset);
        void setUIViewScale(const Vector2& scale);
        void setUIBGColor(const Vector3& color);
        void setUIBufferPos(const Vector2& pos);
        void setUIBufferViewSize(const Vector2& size);
        void setUITranslation(const Vector2& trans);
        void setPosOffsetCoeff(const float& coeff);

        // getters
        UIState getState() const;
        UIType getUIType() const;
        class Game* getGame() const;
        class Font* getFont() const;
        Vector2 getTitlePos() const;
        Vector2 getBGPos() const;
        // Vector2 getNextButtonPos() const;
        UIState getUIState() const;
        Vector3 getTextColor() const;

        // Vector2 getButtonPos() const;
        std::vector<class EmptySprite*>& getUIElements();
        std::unordered_map<std::string, class Texture*>& getUITextures();
        class VertexArray* getSpriteVerts() const;
        
        unsigned int getBindFrameBuffer() const;
        class Texture* getBindTexture() const;
        Vector2 getUIPosOffset() const;
        Vector2 getUIViewScale() const;
        Vector3 getUIBGColor() const;
        Vector2 getUIBufferPos() const;
        Vector2 getUIBufferViewSize() const;
        Vector2 getUITranslation() const;
        float getPosOffsetCoeff() const;

        // std::vector<class Texture*>& getBackgrounds();
        // std::vector<class Button*>& getButtons();
        // std::vector<class Texture*>& getTexture();
        // std::unordered_map<UIButtonType, class Texture*>& getButtonTexs();

        // add or remove
        // void addBackground(class Texture* tex);
        // void addButton(const std::string& type, const Vector2& pos, const Vector2& scale = Vector2{1.0f, 1.0f});
        template <typename T>
        T* addButton(
            const std::string& name, const std::string& type, const std::map<std::string, 
            std::string>& bindTex, const Vector2& pos, const UIBindEvent& bind_event, 
            std::function<void()> onClick, const bool& addIn = true, const Vector2& scale = Vector2{1.0f, 1.0f});

        template <typename T>
        T* addButton(
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
        virtual void bindEvent(const UIBindEvent& event, class Button* b);

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

        // ui绑定的帧缓存序号
        unsigned int mBindFrameBuffer;
        class Texture* mBindTexture;
        Vector2 mUIPosOffset;
        Vector2 mUIViewScale;
        Vector3 mUIBGColor;
        Vector2 mUIBufferPos;
        Vector2 mUIBufferViewSize;
        Vector2 mUITranslation;
        float mPosOffsetCoeff;
    };
    
// }