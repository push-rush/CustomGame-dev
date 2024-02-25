#include "../../include/Renderers/Setting.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/Button.h"
#include "../../include/Audios/AudioSystem.h"
#include "../../include/Renderers/Shader.h"
#include "../../include/Renderers/VertexArray.h"
#include "../../include/Renderers/FreeTypeFont.h"

Setting::Setting(Game* game)
: UIScreen(game)
{
    this->setUIType(ESetting);

    // 暂停游戏
    // game->setGameState(Game::EPaused);

    this->setBGPos(Vector2(0.0f, 0.0f));
    this->setTitlePos(Vector2(0.0f, 200.0f));
    // this->setNextButtonPos(Vector2(0.0f, 0.0f));
    
    // 设置背景
    // Texture* bg = game->getRenderer()->getTexture("../Assets/Images/BG1.png");
    // this->addBackground(bg);
    // bg->setScale(Vector2{1.0f, 0.75});

    // 设置菜单标题
    // this->setTitleTexture(L"游戏设置", Vector3{0.1, 0.1, 0.1}, 42);
    
    // // 添加音量调节滑动条
    // this->addButton(
    //     SoundSlider,
    //     Vector2{0.0f, this->getBGPos().y + bg->getHeight() * 0.15},
    //     Vector2{0.75, 1.0f}
    // );

    // auto buttonTexs = this->getButtonTexs();
    // auto iter = buttonTexs.find(SoundSlider);
    // if (iter != buttonTexs.end())
    // {
    //     auto slider = (*iter).second;

    //     iter = buttonTexs.find(SoundSliderOn);
    //     auto sliderOn = (*iter).second;

    //     this->mSliderCoordMax = 0.0f + slider->getWidth() * 0.5 - sliderOn->getWidth() * 0.25;
    //     this->mSliderCoordMin = 0.0f - slider->getWidth() * 0.5 + sliderOn->getWidth() * 0.25;
    // }
    
    // 添加音量图标
    // iter = buttonTexs.find(VolumeOn);
    // if (iter != buttonTexs.end())
    // {
    //     auto volumeIcon = (*iter).second;
    //     this->addButton(
    //         VolumeOn,
    //         Vector2{this->mSliderCoordMin - volumeIcon->getWidth() * 0.25, this->getBGPos().y + bg->getHeight() * 0.15},
    //         [this](){
    //             for (auto b : this->getButtons())
    //             {
    //                 if (b->getButtonType() == VolumeOn)
    //                 {
    //                     this->getGame()->getAudioSystem()->setBusVolume("bus:/", 0.0f);
    //                     b->setButtonTex(b->getButtonOff(), b->getButtonOn());
                        
    //                     b->setButtonType(VolumeOFF);
    //                 }
    //                 else if (b->getButtonType() == VolumeOFF)
    //                 {
    //                     this->getGame()->getAudioSystem()->setBusVolume("bus:/", 1.0f);
    //                     b->setButtonTex(b->getButtonOff(), b->getButtonOn());
                        
    //                     b->setButtonType(VolumeOn);
    //                 }
    //             }
    //         }
    //     );
    // }

    // float volume = game->getAudioSystem()->getBusVolume("bus:/");
    // Vector2 pos = Vector2{volume * (this->mSliderCoordMax - this->mSliderCoordMin) + this->mSliderCoordMin, 
    //                 this->getBGPos().y + bg->getHeight() * 0.15};
    // std::string str = std::to_string(int(volume * 100));
    // std::wstring w_str(str.begin(), str.end());

    // std::cout << "[Setting] Volumn: " << volume << std::endl;
    // this->addButton(
    //     SoundSliderOn,
    //     w_str,
    //     Vector3{1.0f, 1.0f, 1.0f},
    //     pos, 
    //     [this](){
    //         std::cout << "[Setting] SoundSliderOn" << std::endl;
    //         this->setGameVolume();
    //     }
    // );

    // // 添加关闭窗口按钮
    // Texture* cross = nullptr;
    // iter = buttonTexs.find(CrossOn);
    // if (iter != buttonTexs.end())
    // {
    //     cross = (*iter).second;
    //     this->addButton(CrossOn,
    //         Vector2{this->getBGPos().x + (bg->getWidth() * 0.5 - cross->getWidth()) * bg->getScale().x, 
    //             this->getBGPos().y + (bg->getHeight() * 0.5 - cross->getHeight()) * bg->getScale().y},
    //         [this](){
    //             this->close();
    //         }
    //     );
    // }

    // // 添加取消按钮
    // this->addButton(RectButtonON, L"取消", Vector3{0.5, 0.25, 0.25}, 
    //     Vector2{this->getBGPos().x - bg->getWidth() * 0.25, 
    //         this->getBGPos().y - bg->getHeight() * 0.32}, [this](){
    //     this->close();
    // });

    // // 添加确定按钮
    // this->addButton(RectButtonON, L"确定", Vector3{0.5, 0.25, 0.25}, 
    //     Vector2{this->getBGPos().x + bg->getWidth() * 0.25, 
    //         this->getBGPos().y - bg->getHeight() * 0.32}, [this](){
    //     this->close();
    // });


    // 暂停时禁用相对鼠标模式
    // this->setRelativeMouseMode(false);
}

Setting::~Setting()
{
    // 将游戏状态重新设置回“进行中”状态
    this->getGame()->setGameState(Game::EGamePlay);
    this->setRelativeMouseMode(true);
}

void Setting::handleKeyPress(int key)
{
    UIScreen::handleKeyPress(key);

    // if (key == SDLK_ESCAPE)
    // {
    //     this->close();
    // }
}

void Setting::setGameVolume()
{
    // this->setRelativeMouseMode(true);

    // int x = 0, y = 0;
    
    // Button* volume_bn = nullptr;

    auto elements = this->getUIElements();
    for (auto elem : elements)
    {
        // if (!strcmp(elem->getType().c_str(), "button") && 
        //     !strcmp(elem->getSpriteName().c_str(), "button3"))
        // {
        //     volume_bn = (Button*)elem;
        //     break;
        // }
        
        if (!strcmp(elem->getSpriteName().c_str(), "slider0"))
        {
            auto w = (*this->getUITextures().find((*elem->getBindTexName().begin()).second)).second->getWidth();
            this->mSliderCoordMax = this->getBGPos().x + w * 0.5f;
            this->mSliderCoordMin = this->getBGPos().x - w * 0.5f;

            break;
        }
    }

    // auto iter = volume_bn->getBindTexName().find("on");
    // Texture* tex = nullptr;
    // Shader* shader = nullptr;
    // SDL_Window* window = nullptr;
    // VertexArray* spriteVerts = nullptr;
    // if (volume_bn && 
    //     iter != volume_bn->getBindTexName().end()
    // )
    // {
    //     auto it = this->getUITextures().find((*iter).second);
    //     if (it != this->getUITextures().end())
    //     {
    //         tex = (*it).second;
    //         shader = this->getGame()->getRenderer()->getSpriteShader();
    //         window = this->getGame()->getRenderer()->getRenderWindow();
    //         spriteVerts = this->getGame()->getRenderer()->getSpriteVerts();
    //     }
    // }
    
    // shader->setActive();
    // spriteVerts->setActive();

    // SDL_Event e;
    // SDL_PollEvent(&e);
    // while (!(e.type == SDL_MOUSEBUTTONUP))
    // {
    //     Uint32 buttons = SDL_GetMouseState(&x, &y);
    //     Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
    //     // mousePos.x = e.button.x;
    //     // mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);
    //     // std::cout << "x1:" << mousePos.x << std::endl;

    //     // mousePos.x = float(x);
    //     // mousePos.y = this->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;
    //     mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);
    //     if (mousePos.x >= this->mSliderCoordMax)
    //     {
    //         mousePos.x = this->mSliderCoordMax;
    //     }
    //     else if(mousePos.x <= this->mSliderCoordMin)
    //     {
    //         mousePos.x = this->mSliderCoordMin;
    //     }

    //     // std::cout << "x2:" << mousePos.x << std::endl;
    //     if (volume_bn)
    //     {
    //         // if (mousePos.x > this->mSliderCoordMax)
    //         // {
    //         //     mousePos.x = this->mSliderCoordMax;
    //         // }
    //         // else if (mousePos.x < this->mSliderCoordMin)
    //         // {
    //         //     mousePos.x = this->mSliderCoordMin;
    //         // }

    //         // std::cout << "Sound db: " << (mousePos.x - this->mSliderCoordMin) <<
    //         //     " LIMIT:" << mSliderCoordMax << " " << mSliderCoordMin << std::endl;

    //         // 设置音量
    //         float volume = this->getGame()->getAudioSystem()->getBusVolume("bus:/");
    //         volume = (mousePos.x - this->mSliderCoordMin) / (this->mSliderCoordMax - this->mSliderCoordMin);
    //         this->getGame()->getAudioSystem()->setBusVolume("bus:/", volume);

    //         // bn->setButtonPos(Vector2{x, bn->getPosition().y});
    //         volume_bn->setButtonPos(Vector2{mousePos.x, volume_bn->getPosition().y});
    //         // std::cout << "[Setting] Setted game volume..." << std::endl;

    //         std::string str = std::to_string(int(volume * 100));
    //         std::wstring w_str(str.begin(), str.end());
    //         Vector2 size = volume_bn->setNameTexture(w_str);
    //         volume_bn->setTextSize(size);

    //         // std::cout << "[Setting] Text size: {" << size.x << ", " << size.y << std::endl;

    //         // std::cout << "[Setting] Setted game volume..." << std::endl;
    //         auto renderer = this->getGame()->getRenderer();
    //         renderer->drawUI(this, this->getBindFrameBuffer());
    //         renderer->draw(this->getBindTexture(), Vector2{0.0f, 0.0f}, Vector2{0.8f, 0.8f});
            
    //         SDL_GL_SwapWindow(window);
            
    //         // if (tex && shader)
    //         // {
    //         //     this->drawTexture(shader, tex, bn->getPosition(), bn->getButtonScale().x, bn->getButtonScale().y);

    //         //     std::cout << "[Setting] flush window..." << std::endl;
    //         // }
    //         // else
    //         // {
    //         //     break;
    //         // }
    //     }
    //     else
    //     {
    //         break;
    //     }

    //     SDL_PollEvent(&e);
    // }

    // this->setRelativeMouseMode(false);
}

void Setting::init()
{

}

void Setting::bindEvent(const UIBindEvent& event, Button* b)
{
    SDL_Log("[Setting] Bind event...");
    UIScreen::bindEvent(event, b);

    switch (event)
    {
        case UISetVolume:
        {
            this->setGameVolume();
            SDL_Log("[Setting] Set game volume...");
            break;
        }
        default:
            break;
    }
}

// void Setting::draw(class Shader* spriteShader, class Shader* fontShader, EmptySprite* elem, const Vector2& offset)
// {
//     std::sort(this->getUIElements().begin(), this->getUIElements().end(), [](EmptySprite* e1, EmptySprite* e2)
//     {
//         return e1->getUpdateOrder() < e2->getUpdateOrder();
//     });

//     for (auto e : this->getUIElements())
//     {
//         UIScreen::draw(spriteShader, fontShader, e);
//     }
// }

void Setting::draw(class Shader* basicShader, class Shader* spriteShader, class Shader* fontShader, class EmptySprite* elem)
{
    std::sort(this->getUIElements().begin(), this->getUIElements().end(), [](EmptySprite* e1, EmptySprite* e2)
    {
        return e1->getUpdateOrder() < e2->getUpdateOrder();
    });

    for (auto e : this->getUIElements())
    {
        UIScreen::draw(nullptr, spriteShader, fontShader, e);
    }
}

void Setting::update(float dt)
{
    UIScreen::update(dt);

    auto elements = this->getUIElements();

    Button* volume_bn = nullptr;
    for (auto elem : elements)
    {
        if (!strcmp(elem->getType().c_str(), "button") && 
            !strcmp(elem->getSpriteName().c_str(), "button3"))
        {
            volume_bn = (Button*)elem;
            break;
        }
    }

    int x = 0, y = 0;
    Uint32 mouse = SDL_GetMouseState(&x, &y);
    if ((mouse & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
        volume_bn->getHighlighted())
    {
        Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
        mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);

        if (mousePos.x >= this->mSliderCoordMax)
        {
            mousePos.x = this->mSliderCoordMax;
        }
        else if(mousePos.x <= this->mSliderCoordMin)
        {
            mousePos.x = this->mSliderCoordMin;
        }

        // 设置音量
        float volume = this->getGame()->getAudioSystem()->getBusVolume("bus:/");
        volume = (mousePos.x - this->mSliderCoordMin) / (this->mSliderCoordMax - this->mSliderCoordMin);
        this->getGame()->getAudioSystem()->setBusVolume("bus:/", volume);

        volume_bn->setButtonPos(Vector2{mousePos.x, volume_bn->getPosition().y});
    }
}