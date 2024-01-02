#pragma once

#include <wchar.h>
#include "./EmptySprite.h"

class Button : public EmptySprite
{
public:
    Button(class UIScreen* ui, 
        const std::string& name, 
        const std::string& type, 
        const std::map<std::string, std::string>& bindTexName, 
        const int& bind_event,
        std::function<void()> onClick, 
        const Vector2& pos, 
        const Vector2& dims, 
        const Vector2& scale,
        const bool& addIn = true);
        
    Button(class UIScreen* ui, 
        const std::string& name, 
        const std::string& type, 
        const std::map<std::string, std::string>& bindTexName,
        const std::wstring& text, const Vector3& color, const int& fontSize,
        const int& bind_event,
        std::function<void()> onClick, 
        const Vector2& pos, const Vector2& dims, const Vector2& scale, const bool& addIn = true);
    
    // Button(const int& type, const std::wstring& name, class Font* font, const Vector3& color, 
    //     std::function<void()> onClick, const Vector2& pos, const Vector2& dims);
    ~Button();

    // setters
    // Vector2 setNameTexture(const std::wstring& name);
    // void setTextColor(const Vector3&);
    // void setTextSize(const Vector2&);
    // std::vector<class Texture*>& getNameTextures();
    // Vector2 getTextSize() const;
    // Vector3 getTextColor() const;
    // std::wstring getText() const;
    
    void setHighlighted(bool);
    // void setButtonTex(class Texture* buttonOn, class Texture* buttonOff);
    // void setButtonType(int type);
    void setButtonState(bool is_click);
    void setButtonScale(const Vector2& scale);
    void setButtonPos(const Vector2& pos);
    void setBindEventID(const int& event);

    // getters
    // float getHeight() const;
    // float getWidth() const;
    bool getHighlighted() const;
    const int& getBindEventID() const;

    // const Vector2& getPosition() const override;
    // const Vector2& getDimension() const override;
    // const Vector2& getScale() const override;

    // class Texture* getButtonOn() const;
    // class Texture* getButtonOff() const;
    // int getButtonType() const;

    bool getButtonState() const;
    Vector2 getButtonScale() const;

    // 判断点击点在按钮范围内
    bool containsPoint(const Vector2& ps) const;

    // 按钮被点击时回调函数
    void onClick();

private:
    std::function<void()> mOnClick;

    // std::wstring mText;
    // Vector2 mTextSize;
    // Vector3 mTextColor;
    // std::vector<class Texture*> mNameTextures;
    
    // class Font* mFont;
    // Vector2 mPosition;
    // Vector2 mDimensions;
    // Vector2 mScale;
    
    // int mButtonType;
    // class Texture* mButtonOn;
    // class Texture* mButtonOff;
    
    int mBindEventID;
    bool mHighlighted;
    bool mIsClickable;
    bool mContainText;
};
