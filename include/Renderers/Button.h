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
        const bool& addIn = true
    );

    Button(class UIScreen* ui, 
        const std::string& name, 
        const std::string& type, 
        const std::map<std::string, std::string>& bindTexName,
        const Vector2& bindTexOffset,
        const int& bind_event,
        std::function<void()> onClick, 
        const Vector2& pos, 
        const Vector2& dims, 
        const Vector2& scale,
        const bool& addIn = true
    );
        
    Button(class UIScreen* ui, 
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

    Button(class UIScreen* ui, 
        const std::string& name, 
        const std::string& type, 
        const std::map<std::string, std::string>& bindTexName,
        const Vector2& bindTexOffset,
        const std::wstring& text, const Vector3& color, const int& fontSize,
        const Vector2& bindTextOffset,
        const int& bind_event,
        std::function<void()> onClick, 
        const Vector2& pos,
        const Vector2& dims, 
        const Vector2& scale, 
        const bool& addIn = true
    );

    Button(class UIScreen* ui,
        const std::string& name, const std::string& type,
        const Vector2& pos, const Vector2& dims, const Vector2& scale, 
        const Vector3& bgColor, const Vector3& boxColor,
        const std::wstring& text, const Vector3& color, const int& fontSize, const Vector2& bindTextOffset,
        const std::map<std::string, std::string>& bindTexName, const Vector2& bindTexOffset,
        const int& bind_event,
        std::function<void()> onClick,
        const bool& addIn = true
    );

    ~Button();

    // setters
    void setHighlighted(bool);
    // void setButtonTex(class Texture* buttonOn, class Texture* buttonOff);
    // void setButtonType(int type);
    void setButtonState(bool is_click);
    void setButtonScale(const Vector2& scale);
    void setButtonPos(const Vector2& pos);
    void setBindEventID(const int& event);

    Vector3 setBindBgColor(const Vector3& color);
    Vector3 setBindBoxColor(const Vector3& color);

    // getters
    // float getHeight() const;
    // float getWidth() const;
    bool getHighlighted() const;
    const int& getBindEventID() const;
    bool getButtonState() const;
    Vector2 getButtonScale() const;

    Vector3 getBindBgColor() const;
    Vector3 getBindBoxColor() const;

    // 判断点击点在按钮范围内
    bool containsPoint(const Vector2& ps) const;

    // 按钮被点击时回调函数
    void onClick();

private:
    std::function<void()> mOnClick;

    int mBindEventID;
    bool mHighlighted;
    bool mIsClickable;
    bool mContainText;

    Vector3 mBindBgColor;
    Vector3 mBindBoxColor;
};