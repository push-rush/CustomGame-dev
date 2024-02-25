#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <functional>
#include <vector>
#include <string>
#include <map>

#include "../General/Cmath.h"

class EmptySprite
{
public:
    enum SpriteState
    {
        EDisplay,
        EUninvisible,
        EUnselected,
        ESelected,
        EActivited
    };

private:
    class UIScreen* mUI;
    int mUpdateOrder;
    std::string mName;
    std::string mType;
    std::map<std::string, std::string> mBindTexName;
    Vector2 mPosition;
    Vector2 mScale;
    Vector2 mDimension;
    SpriteState mSpriteState;

    // ui文本信息
    bool mContainText;
    std::wstring mText;
    Vector2 mTextSize;
    Vector3 mTextColor;
    int mFontSize;
    std::vector<class Texture*> mNameTextures;

    Vector2 mBindTexOffset;
    Vector2 mBindTextOffset;

public:
    EmptySprite(
        class UIScreen* ui, 
        const std::string& name, 
        const std::string& type,
        const std::map<std::string, std::string>& bindTexName, 
        const bool& addIn = true,
        const Vector2& pos = Vector2{0.0f, 0.0f}, 
        const Vector2& scale = Vector2{1.0f, 1.0f},
        const int& mUpdateOrder = 100,
        const Vector2& dimension = Vector2{0.0f, 0.0f},
        const bool& contain_text = false,
        const std::wstring& text = L"default",
        const Vector3& color = Vector3{0.25, 0.25, 0.25},
        const int& fontSize = 35
    );
    ~EmptySprite();

    // getters
    const std::string& getSpriteName() const;
    const std::string& getType() const;
    const std::map<std::string, std::string>& getBindTexName() const;
    const int& getUpdateOrder() const;
    class UIScreen* getUIScreen() const;
    
    const Vector2& getPosition() const;
    const Vector2& getScale() const;
    const Vector2& getDimension() const;

    std::vector<class Texture*>& getNameTextures();
    Vector2 getTextSize() const;
    Vector3 getTextColor() const;
    std::wstring getText() const;
    int getFontSize() const;

    SpriteState getSpriteState() const;

    Vector2 getBindTexOffset() const;
    Vector2 getBindTextOffset() const;

    // setters
    void setSpriteName(const std::string& name);
    void setType(const std::string& type);
    void setBindTexName(const std::map<std::string, std::string>& bindTexName);
    void setPosition(const Vector2& pos);
    void setScale(const Vector2& scale);
    void setDimension(const Vector2& dim);
    void setUpdateOrder(const int& updateOrder);

    void setSpriteState(const SpriteState& state);
    
    void setText(std::wstring text);
    void setTextColor(const Vector3&);
    void setTextSize(const Vector2&);
    Vector2 setNameTexture(const std::wstring& name, const int& font_size = 35);
    void updateTextTexture(const std::vector<class Texture*>& texs);

    void setBindTexOffset(const Vector2& offset);
    void setBindTextOffset(const Vector2& offset);

    bool containText();
};