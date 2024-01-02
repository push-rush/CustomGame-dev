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
    // this->mButtonType = type;

    // this->mPosition = pos;
    // this->mDimensions = dims;
    // this->mScale = scale;

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
    // this->mButtonType = type;
    // this->mFreeTypeFont = freeTypeFont;

    // this->mName = name;
    // this->mTextColor = color;

    // this->mPosition = pos;
    // this->mDimensions = dims;
    // this->mScale = scale;
    
    // this->mTextSize = this->setNameTexture(text);
    this->mOnClick = onClick;

    this->mHighlighted = false;
    this->mIsClickable = true;
    this->mContainText = true;
}

// Button::Button(const int& type,
//     const std::wstring& name, class Font* font, const Vector3& color,
//     std::function<void()> onClick, const Vector2& pos, const Vector2& dims)
// {
//     this->mButtonType = type;
//     this->mName = name;
//     this->mFont = font;
//     this->mTextColor = color;
//     this->mPosition = pos;
//     this->mDimensions = dims;
//     this->mHighlighted = false;
//     this->mOnClick = onClick;
    
//     this->mTextSize = this->setNameTexture(name);

//     this->mButtonOff = nullptr;
//     this->mButtonOn = nullptr;
// }

Button::~Button()
{
    // for (auto tex : this->mNameTextures)
    // {
    //     if (tex)
    //     {
    //         tex->unLoad();
    //         delete tex;
    //         tex = nullptr;
    //     }
    // }
    // this->mNameTextures.clear();
}

// Vector2 Button::setNameTexture(const std::wstring& name)
// {
//     this->mText = name;
//     // std::wcout << "name1: " << name << std::endl;

//     for (auto tex : this->mNameTextures)
//     {
//         if (tex)
//         {
//             tex->unLoad();
//             delete tex;
//             tex = nullptr;
//         }
//     }
//     this->mNameTextures.clear();

//     // std::cout << "set tex..." << std::endl;
//     // this->mNameTexture = this->mFont->renderText(name);
//     // wchar_t text[name.length()];
//     // memcpy(text, name.c_str(), sizeof(char) * name.length());

//     // std::wcout << "name2: " << name << std::endl;

//     return this->getUIScreen()->getGame()->getFreeTypeFont()->renderText(this->mNameTextures, name.c_str(), Color::Red, 35);
// }

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