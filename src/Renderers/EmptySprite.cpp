#include "../../include/Game.h"

#include "../../include/Renderers/FreeTypeFont.h"
#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/UIScreen.h"
#include "../../include/Renderers/Texture.h"

EmptySprite::EmptySprite(
    class UIScreen* ui,
    const std::string& name, 
    const std::string& type,
    const std::map<std::string, std::string>& bindTexName, 
    const bool& addIn,
    const Vector2& pos,
    const Vector2& scale,
    const int& updateOrder,
    const Vector2& dimension,
    const bool& contain_text,
    const std::wstring& text,
    const Vector3& color,
    const int& fontSize
) : mUI(ui), mName(name), mType(type), mBindTexName(bindTexName), 
    mPosition(pos), mDimension(dimension), mScale(scale), mUpdateOrder(updateOrder),
    mContainText(contain_text), mText(text), mTextColor(color), mFontSize(fontSize),
    mSpriteState(EDisplay)
{
    if (addIn)
        ui->addUIElement(this);

    if (contain_text)
    {
        this->mTextSize = this->setNameTexture(text, fontSize);
    }

    this->mBindTexOffset = Vector2{0.0f, 0.0f};
    this->mBindTextOffset = Vector2{0.0f, 0.0f};
}

EmptySprite::~EmptySprite()
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

const std::string& EmptySprite::getSpriteName() const
{
    return this->mName;
}

const std::map<std::string, std::string>& EmptySprite::getBindTexName() const
{
    return this->mBindTexName;
}

void EmptySprite::setSpriteName(const std::string& name)
{
    this->mName = name;
}

void EmptySprite::setBindTexName(const std::map<std::string, std::string>& bindTexName)
{
    this->mBindTexName = bindTexName;
}

const Vector2& EmptySprite::getPosition() const
{
    return this->mPosition;
}

void EmptySprite::setPosition(const Vector2& pos)
{
    this->mPosition = pos;
}

const Vector2& EmptySprite::getScale() const
{
    return this->mScale;
}   

void EmptySprite::setScale(const Vector2& scale)
{
    this->mScale = scale;
}

void EmptySprite::setDimension(const Vector2& dim)
{
    this->mDimension = dim;
}

const Vector2& EmptySprite::getDimension() const
{
    return this->mDimension;
}

const int& EmptySprite::getUpdateOrder() const
{
    return this->mUpdateOrder;
}

void EmptySprite::setUpdateOrder(const int& updateOrder)
{
    this->mUpdateOrder = updateOrder;
}

void EmptySprite::setType(const std::string& type)
{
    this->mType = type; 
}

const std::string& EmptySprite::getType() const
{
    return this->mType;
}

class UIScreen* EmptySprite::getUIScreen() const
{
    return this->mUI;
}

void EmptySprite::setText(std::wstring text)
{
    this->mText = text;
}

void EmptySprite::setTextSize(const Vector2& size)
{
    this->mTextSize = size;
}

std::wstring EmptySprite::getText() const
{
    return this->mText;
}

Vector2 EmptySprite::getTextSize() const
{
    return this->mTextSize;
}

Vector3 EmptySprite::getTextColor() const
{
    return this->mTextColor;
}

void EmptySprite::setTextColor(const Vector3& color)
{
    this->mTextColor = color;
}

std::vector<Texture*>& EmptySprite::getNameTextures()
{
    return this->mNameTextures;
}

Vector2 EmptySprite::setNameTexture(const std::wstring& name, const int& font_size)
{
    this->mText = name;
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

    return this->getUIScreen()->getGame()->getFreeTypeFont()->renderText(this->mNameTextures, name.c_str(), mTextColor, font_size);
}

bool EmptySprite::containText()
{
    return mContainText;
}

int EmptySprite::getFontSize() const
{
    return this->mFontSize;
}

EmptySprite::SpriteState EmptySprite::getSpriteState() const
{   
    return this->mSpriteState;
}

void EmptySprite::setSpriteState(const SpriteState& state)
{
    this->mSpriteState = state;
}

Vector2 EmptySprite::getBindTexOffset() const
{
    return this->mBindTexOffset;
}

Vector2 EmptySprite::getBindTextOffset() const
{
    return this->mBindTextOffset;
}

void EmptySprite::setBindTexOffset(const Vector2& offset)
{
    this->mBindTexOffset = offset;
}

void EmptySprite::setBindTextOffset(const Vector2& offset)
{
    this->mBindTextOffset = offset;
}