#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <functional>
#include <vector>
#include <string>
#include <wchar.h>

#include "../include/General/Cmath.h"

class Button
{
public:
    Button(const int& type, const Vector2& pos, 
        const Vector2& dims, const Vector2& scale);

    Button(const int& type, std::function<void()> onClick, 
        const Vector2& pos, const Vector2& dims, const Vector2& scale);
        
    Button(const int& type, const std::wstring& name, class FreeTypeFont* font, 
        const Vector3& color, std::function<void()> onClick, const Vector2& pos, const Vector2& dims, const Vector2& scale);
    
    // Button(const int& type, const std::wstring& name, class Font* font, const Vector3& color, 
    //     std::function<void()> onClick, const Vector2& pos, const Vector2& dims);
    ~Button();

    // setters
    Vector2 setNameTexture(const std::wstring& name);
    void setHighlighted(bool);
    void setTextColor(const Vector3&);
    void setButtonTex(class Texture* buttonOn, class Texture* buttonOff);
    void setButtonType(int type);
    void setButtonState(bool is_click);
    void setButtonScale(const Vector2& scale);
    void setButtonPos(const Vector2& pos);
    void setTextSize(const Vector2&);

    // getters
    // float getHeight() const;
    // float getWidth() const;
    bool getHighlighted() const;
    Vector2 getPosition() const;
    Vector2 getDimensions() const; 
    std::vector<class Texture*>& getNameTextures();
    Vector2 getTextSize() const;
    Vector3 getTextColor() const;
    class Texture* getButtonOn() const;
    class Texture* getButtonOff() const;
    int getButtonType() const;
    bool getButtonState() const;
    Vector2 getButtonScale() const;
    std::wstring getText() const;

    // 判断点击点在按钮范围内
    bool containsPoint(const Vector2& ps) const;

    // 按钮被点击时回调函数
    void onClick();

private:
    std::function<void()> mOnClick;

    std::wstring mName;
    std::vector<class Texture*> mNameTextures;
    class Font* mFont;
    class FreeTypeFont* mFreeTypeFont;
    Vector2 mTextSize;
    Vector3 mTextColor;
    
    Vector2 mPosition;
    Vector2 mDimensions;
    Vector2 mScale;
    
    int mButtonType;
    class Texture* mButtonOn;
    class Texture* mButtonOff;
    
    bool mHighlighted;
    bool mIsClickable;
    bool mContainText;
};
