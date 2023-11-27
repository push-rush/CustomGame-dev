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
    game->setGameState(Game::EPaused);

    this->setBGPos(Vector2(0.0f, 0.0f));
    this->setTitlePos(Vector2(0.0f, 200.0f));
    this->setNextButtonPos(Vector2(0.0f, 0.0f));
    
    // 设置背景
    Texture* bg = game->getRenderer()->getTexture("../Assets/Images/BG1.png");
    this->addBackground(bg);
    bg->setScale(Vector2{1.0f, 0.75});

    // 设置菜单标题
    this->setTitleTexture(L"游戏设置", Vector3{0.1, 0.1, 0.1}, 42);
    
    // 添加音量调节滑动条
    this->addButton(
        SoundSlider,
        Vector2{0.0f, this->getBGPos().y + bg->getHeight() * 0.15},
        Vector2{0.75, 1.0f}
    );

    auto buttonTexs = this->getButtonTexs();
    auto iter = buttonTexs.find(SoundSlider);
    if (iter != buttonTexs.end())
    {
        auto slider = (*iter).second;

        iter = buttonTexs.find(SoundSliderOn);
        auto sliderOn = (*iter).second;

        this->mSliderCoordMax = 0.0f + slider->getWidth() * 0.5 - sliderOn->getWidth() * 0.25;
        this->mSliderCoordMin = 0.0f - slider->getWidth() * 0.5 + sliderOn->getWidth() * 0.25;
    }
    
    float volume = game->getAudioSystem()->getBusVolume("bus:/");
    Vector2 pos = Vector2{volume * (this->mSliderCoordMax - this->mSliderCoordMin) + this->mSliderCoordMin, 
                    this->getBGPos().y + bg->getHeight() * 0.15};
    std::string str = std::to_string(int(volume * 100));
    std::wstring w_str(str.begin(), str.end());

    std::cout << "[Setting] Volumn: " << volume << std::endl;
    this->addButton(
        SoundSliderOn,
        w_str,
        Vector3{1.0f, 1.0f, 1.0f},
        pos, 
        [this](){
            std::cout << "[Setting] SoundSliderOn" << std::endl;
            this->setGameVolume();
        }
    );

    // 添加关闭窗口按钮
    Texture* cross = nullptr;
    iter = buttonTexs.find(CrossOn);
    if (iter != buttonTexs.end())
    {
        cross = (*iter).second;
        this->addButton(CrossOn,
            Vector2{this->getBGPos().x + (bg->getWidth() * 0.5 - cross->getWidth()) * bg->getScale().x, 
            this->getBGPos().y + (bg->getHeight() * 0.5 - cross->getHeight()) * bg->getScale().y}
        );
    }

    // 添加取消按钮
    this->addButton(RectButtonON, L"取消", Vector3{0.5, 0.25, 0.25}, 
        Vector2{this->getBGPos().x - bg->getWidth() * 0.25, 
            this->getBGPos().y - bg->getHeight() * 0.32}, [this](){
        this->close();
    });

    // 添加确定按钮
    this->addButton(RectButtonON, L"确定", Vector3{0.5, 0.25, 0.25}, 
        Vector2{this->getBGPos().x + bg->getWidth() * 0.25, 
            this->getBGPos().y - bg->getHeight() * 0.32}, [this](){
        this->close();
    });


    // 暂停时禁用相对鼠标模式
    this->setRelativeMouseMode(false);
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

    if (key == SDLK_ESCAPE)
    {
        this->close();
    }
}

void Setting::setGameVolume()
{
    this->setRelativeMouseMode(true);

    int x = 0, y = 0;
    
    Button* bn = nullptr;
    Button* bn_off = nullptr;
    for (auto b : this->getButtons())
    {
        if (b->getButtonType() == SoundSliderOn)
        {
            bn = b;
        }
    }

    auto iter = this->getButtonTexs().find(SoundSliderOn);
    Texture* tex = nullptr;
    Shader* shader = nullptr;
    SDL_Window* window = nullptr;
    VertexArray* spriteVerts = nullptr;
    if (iter != this->getButtonTexs().end())
    {
        tex = (*iter).second;
        shader = this->getGame()->getRenderer()->getSpriteShader();
        window = this->getGame()->getRenderer()->getRenderWindow();
        spriteVerts = this->getGame()->getRenderer()->getSpriteVerts();
    }
    
    shader->setActive();
    spriteVerts->setActive();

    SDL_Event e;
    SDL_PollEvent(&e);
    while (!(e.type == SDL_MOUSEBUTTONUP))
    {
        Uint32 buttons = SDL_GetMouseState(&x, &y);
        Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
        // mousePos.x = e.button.x;
        // mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);
        // std::cout << "x1:" << mousePos.x << std::endl;

        // mousePos.x = float(x);
        // mousePos.y = this->getGame()->getRenderer()->getScreenHeight() * 0.5f - mousePos.y;
        mousePos.x -= (this->getGame()->getRenderer()->getScreenWidth() * 0.5f);

        // std::cout << "x2:" << mousePos.x << std::endl;
        if (bn)
        {
            if (mousePos.x > this->mSliderCoordMax)
            {
                mousePos.x = this->mSliderCoordMax;
            }
            else if (mousePos.x < this->mSliderCoordMin)
            {
                mousePos.x = this->mSliderCoordMin;
            }

            // std::cout << "Sound db: " << (mousePos.x - this->mSliderCoordMin) <<
            //     " LIMIT:" << mSliderCoordMax << " " << mSliderCoordMin << std::endl;

            // 设置音量
            float volume = this->getGame()->getAudioSystem()->getBusVolume("bus:/");
            volume = 1.0f / (this->mSliderCoordMax - this->mSliderCoordMin) * (mousePos.x - this->mSliderCoordMin);
            this->getGame()->getAudioSystem()->setBusVolume("bus:/", volume);

            // bn->setButtonPos(Vector2{x, bn->getPosition().y});
            bn->setButtonPos(Vector2{mousePos.x, bn->getPosition().y});
            // std::cout << "[Setting] Setted game volume..." << std::endl;

            std::string str = std::to_string(int(volume * 100));
            std::wstring w_str(str.begin(), str.end());
            Vector2 size = bn->setNameTexture(w_str);
            bn->setTextSize(size);

            // std::cout << "[Setting] Text size: {" << size.x << ", " << size.y << std::endl;

            // std::cout << "[Setting] Setted game volume..." << std::endl;
            
            this->getGame()->getRenderer()->drawUI(this->getGame()->getRenderer()->getUIBuffer());
            SDL_GL_SwapWindow(window);
            // if (tex && shader)
            // {
            //     this->drawTexture(shader, tex, bn->getPosition(), bn->getButtonScale().x, bn->getButtonScale().y);

            //     std::cout << "[Setting] flush window..." << std::endl;
            // }
            // else
            // {
            //     break;
            // }
        }
        else
        {
            break;
        }

        SDL_PollEvent(&e);
    }

    this->setRelativeMouseMode(false);
}