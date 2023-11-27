#include "../../include/Renderers/Button.h"
#include "../../include/Renderers/Texture.h"

#include "../../include/Renderers/Font.h"
#include "../../include/Renderers/FreeTypeFont.h"

Button::Button(const int& type, const Vector2& pos, 
    const Vector2& dims, const Vector2& scale)
{
    this->mButtonType = type;
    this->mPosition = pos;
    this->mDimensions = dims;
    this->mScale = scale;

    this->mHighlighted = false;
    this->mIsClickable = false;
    this->mContainText = false;
}

Button::Button(const int& type, std::function<void()> onClick, 
    const Vector2& pos, const Vector2& dims, const Vector2& scale)
{
    this->mButtonType = type;
    this->mPosition = pos;
    this->mDimensions = dims;
    this->mScale = scale;

    this->mOnClick = onClick;

    this->mHighlighted = false;
    this->mIsClickable = true;
    this->mContainText = false;
}

Button::Button(const int& type, 
    const std::wstring& name, class FreeTypeFont* freeTypeFont, 
    const Vector3& color, std::function<void()> onClick, 
    const Vector2& pos, const Vector2& dims, const Vector2& scale)
{
    this->mButtonType = type;
    this->mName = name;
    this->mFreeTypeFont = freeTypeFont;
    this->mTextColor = color;
    this->mPosition = pos;
    this->mDimensions = dims;
    this->mScale = scale;
    
    this->mTextSize = this->setNameTexture(name);
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
    for (auto tex : this->mNameTextures)
    {
        if (tex)
        {
            tex->unLoad();
            delete tex;
            tex = nullptr;
        }
    }
    this->mNameTextures.clear();
}

Vector2 Button::setNameTexture(const std::wstring& name)
{
    this->mName = name;
    // std::wcout << "name1: " << name << std::endl;

    for (auto tex : this->mNameTextures)
    {
        if (tex)
        {
            tex->unLoad();
            delete tex;
            tex = nullptr;
        }
    }
    this->mNameTextures.clear();

    // std::cout << "set tex..." << std::endl;
    // this->mNameTexture = this->mFont->renderText(name);
    // wchar_t text[name.length()];
    // memcpy(text, name.c_str(), sizeof(char) * name.length());

    // std::wcout << "name2: " << name << std::endl;

    return this->mFreeTypeFont->renderText(this->mNameTextures, name.c_str(), Color::Red, 35);
}

bool Button::containsPoint(const Vector2& ps) const
{
    bool no = (
        ps.x < (this->mPosition.x - this->mDimensions.x * 0.5f) ||
        ps.x > (this->mPosition.x + this->mDimensions.x * 0.5f) ||
        ps.y < (this->mPosition.y - this->mDimensions.y * 0.5f) || 
        ps.y > (this->mPosition.y + this->mDimensions.y * 0.5f)
    );
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

Vector2 Button::getPosition() const
{
    return this->mPosition;
}

Vector2 Button::getDimensions() const
{
    return this->mDimensions;
}

std::vector<Texture*>& Button::getNameTextures()
{
    return this->mNameTextures;
}

void Button::setHighlighted(bool value)
{
    this->mHighlighted = value;
}

Vector2 Button::getTextSize() const
{
    return this->mTextSize;
}

Vector3 Button::getTextColor() const
{
    return this->mTextColor;
}

void Button::setTextColor(const Vector3& color)
{
    this->mTextColor = color;
}

void Button::setButtonTex(Texture* buttonOn, Texture* buttonOff)
{
    this->mButtonOn = buttonOn;
    this->mButtonOff = buttonOff;
}

Texture* Button::getButtonOn() const
{
    return this->mButtonOn;
}

Texture* Button::getButtonOff() const
{
    return this->mButtonOff;
}

void Button::setButtonType(int type)
{
    this->mButtonType = type;
}

int Button::getButtonType() const
{
    return this->mButtonType;
}

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
    this->mScale = scale;
}

Vector2 Button::getButtonScale() const
{
    return this->mScale;
}

void Button::setButtonPos(const Vector2& pos)
{
    this->mPosition = pos;
}

void Button::setTextSize(const Vector2& size)
{
    this->mTextSize = size;
}

std::wstring Button::getText() const
{
    return this->mName;
}