#include "../../include/Game.h"

#include "../../include/Renderers/Button.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Font.h"
#include "../../include/Renderers/FreeTypeFont.h"
#include "../../include/Renderers/UIScreen.h"

Button::Button(class UIScreen* ui,
    const std::string& name, 
    const std::string& type, 
    const std::map<std::string, std::string>& bindTexName,
    const int& bind_event,
    std::function<void()> onClick, 
    const Vector2& pos, const Vector2& dims, const Vector2& scale,
    const bool& addIn
) : EmptySprite(ui, name, type, bindTexName, addIn, 
        pos, scale, 100, dims),
    mBindEventID(int(bind_event))
{
    this->mOnClick = onClick;
    this->mHighlighted = false;
    this->mIsClickable = true;
    this->mContainText = false;
}

Button::Button(class UIScreen* ui,
    const std::string& name, 
    const std::string& type, 
    const std::map<std::string, std::string>& bindTexName, 
    const std::wstring& text, const Vector3& color, const int& fontSize, 
    const int& bind_event,
    std::function<void()> onClick, 
    const Vector2& pos, const Vector2& dims, const Vector2& scale,
    const bool& addIn
) : EmptySprite(ui, name, type, bindTexName, addIn, 
        pos, scale, 100, dims, true, text, color, fontSize),
    mBindEventID(int(bind_event))
{
    this->mOnClick = onClick;
    this->mHighlighted = false;
    this->mIsClickable = true;
    this->mContainText = true;
}

Button::Button(class UIScreen* ui, 
    const std::string& name, 
    const std::string& type, 
    const std::map<std::string, std::string>& bindTexName,
    const Vector2& bindTexOffset,
    const int& bind_event,
    std::function<void()> onClick, 
    const Vector2& pos, 
    const Vector2& dims, 
    const Vector2& scale,
    const bool& addIn
) : EmptySprite(ui, name, type, bindTexName, addIn, 
        pos, scale, 100, dims),
    mBindEventID(int(bind_event))
{
    this->mOnClick = onClick;
    this->mHighlighted = false;
    this->mIsClickable = true;
    this->mContainText = false;

    this->setBindTexOffset(bindTexOffset);
}

Button::Button(class UIScreen* ui, 
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
    const bool& addIn
) : EmptySprite(ui, name, type, bindTexName, addIn, 
        pos, scale, 100, dims, true, text, color, fontSize),
    mBindEventID(int(bind_event))
{
    this->mOnClick = onClick;
    this->mHighlighted = false;
    this->mIsClickable = true;
    this->mContainText = true;

    this->setBindTexOffset(bindTexOffset);
    this->setBindTextOffset(bindTextOffset);
}

Button::Button(class UIScreen* ui,
        const std::string& name, const std::string& type,
        const Vector2& pos, const Vector2& dims, const Vector2& scale, 
        const Vector3& bgColor, const Vector3& boxColor,
        const std::wstring& text, const Vector3& color, const int& fontSize, const Vector2& bindTextOffset,
        const std::map<std::string, std::string>& bindTexName, const Vector2& bindTexOffset,
        const int& bind_event,
        std::function<void()> onClick,
        const bool& addIn
) : EmptySprite(ui, name, type, bindTexName, addIn, 
        pos, scale, 100, dims, true, text, color, fontSize),
    mBindEventID(int(bind_event))
{
    this->mOnClick = onClick;
    this->mHighlighted = false;
    this->mIsClickable = true;
    this->mContainText = true;

    this->mBindBgColor = bgColor;
    this->mBindBoxColor = boxColor;

    this->setBindTexOffset(bindTexOffset);
    this->setBindTextOffset(bindTextOffset);
}

Button::~Button()
{

}

bool Button::containsPoint(const Vector2& ps) const
{
    bool no = (
        ps.x < (this->getPosition().x - this->getDimension().x * 0.5f) ||
        ps.x > (this->getPosition().x + this->getDimension().x * 0.5f) ||
        ps.y < (this->getPosition().y - this->getDimension().y * 0.5f) || 
        ps.y > (this->getPosition().y + this->getDimension().y * 0.5f)
    );

    // SDL_Log("pos: (%.2f, %.2f) size: (%.2f, %.2f)", 
    //     this->getPosition().x, this->getPosition().y, 
    //     this->getDimension().x, this->getDimension().y
    // );
    return !no;
}

void Button::onClick()
{
    if (this->mOnClick)
    {
        this->mOnClick();
    }
}

// float Button::getHeight() const
// {
//     return this->mDimensions.x;
// }

// float Button::getWidth() const
// {
//     return this->mDimensions.y;
// }

bool Button::getHighlighted() const
{
    return this->mHighlighted;
}

// const Vector2& Button::getPosition() const
// {
//     // return this->mPosition;
// }

// const Vector2& Button::getDimension() const
// {
//     // return this->mDimensions;
// }

// const Vector2& Button::getScale() const
// {

// }

// std::vector<Texture*>& Button::getNameTextures()
// {
//     return this->mNameTextures;
// }

void Button::setHighlighted(bool value)
{
    this->mHighlighted = value;
}

// Vector2 Button::getTextSize() const
// {
//     return this->mTextSize;
// }

// Vector3 Button::getTextColor() const
// {
//     return this->mTextColor;
// }

// void Button::setTextColor(const Vector3& color)
// {
//     this->mTextColor = color;
// }

// void Button::setButtonTex(Texture* buttonOn, Texture* buttonOff)
// {
//     this->mButtonOn = buttonOn;
//     this->mButtonOff = buttonOff;
// }

// Texture* Button::getButtonOn() const
// {
//     return this->mButtonOn;
// }

// Texture* Button::getButtonOff() const
// {
//     return this->mButtonOff;
// }

// void Button::setButtonType(int type)
// {
//     this->mButtonType = type;
// }

// int Button::getButtonType() const
// {
//     return this->mButtonType;
// }

void Button::setButtonState(bool is_click)
{
    this->mIsClickable = is_click;
}

bool Button::getButtonState() const
{
    return this->mIsClickable;
}

void Button::setButtonScale(const Vector2& scale)
{
    // this->mScale = scale;
    this->setScale(scale);
}

Vector2 Button::getButtonScale() const
{
    // return this->mScale;
    return this->getScale();
}

void Button::setButtonPos(const Vector2& pos)
{
    // this->mPosition = pos;
    this->setPosition(pos);
}

// void Button::setTextSize(const Vector2& size)
// {
//     this->mTextSize = size;
// }

// std::wstring Button::getText() const
// {
//     return this->mText;
// }

const int& Button::getBindEventID() const
{
    return this->mBindEventID;
}

void Button::setBindEventID(const int& event)
{
    this->mBindEventID = event;
}

Vector3 Button::setBindBgColor(const Vector3& color)
{
    this->mBindBgColor = color;
}

Vector3 Button::setBindBoxColor(const Vector3& color)
{
    this->mBindBoxColor = color;
}

Vector3 Button::getBindBgColor() const
{
    return this->mBindBgColor;
}

Vector3 Button::getBindBoxColor() const
{
    return this->mBindBoxColor;
}