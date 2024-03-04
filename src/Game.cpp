#include "../include/Game.h"

#include "../include/Actors/Asteroid.h"
#include "../include/Actors/Ship.h"
#include "../include/Actors/Laser.h"
#include "../include/Actors/Actor.h"
#include "../include/Actors/FPSActor.h"
#include "../include/Actors/FollowActor.h"
#include "../include/Actors/OrbitActor.h"
#include "../include/Actors/SplineActor.h"
#include "../include/Actors/TargetActor.h"
#include "../include/Actors/PlaneActor.h"
#include "../include/Actors/CameraActor.h"

#include "../include/Components/MeshComponent.h"
#include "../include/Components/SpriteComponent.h"
#include "../include/Components/PointLightComponent.h"
#include "../include/Components/AudioComponent.h"
#include "../include/Audios/AudioSystem.h"

#include "../include/Renderers/Renderer.h"
#include "../include/Renderers/Font.h"
#include "../include/Renderers/FreeTypeFont.h"
#include "../include/Renderers/Skeleton.h"
#include "../include/Renderers/Animation.h"
#include "../include/Renderers/PhysWorld.h"
#include "../include/Renderers/PauseMenu.h"
#include "../include/Renderers/HUD.h"
#include "../include/Renderers/Setting.h"
#include "../include/Renderers/DialogBox.h"
#include "../include/Renderers/ResourceMenu.h"
#include "../../include/Renderers/Console.h"
#include "../../include/Renderers/Button.h"
#include "../../include/Renderers/PropertyMenu.h"
#include "../../include/Renderers/Collision.h"

#include "../include/General/LevelLoader.h"
#include "../include/General/ResourceManager.h"

#include "fmod.hpp"
#include "fmod_studio.h"
#include "fmod_errors.h"
#include "rapidjson.h"
#include "document.h"

#define LOAD_PATH "../Assets/LevelFiles/SaveFile1.json"
#define SAVE_PATH "../Assets/LevelFiles/SaveFile0.json"

#define LOAD_HUD_PATH "../Assets/LevelFiles/HUD_save0.json"
#define SAVE_HUD_PATH "../Assets/LevelFiles/HUD_save1.json"

#define LOAD_UITREE_PATH "../Assets/LevelFiles/UITree_menu1.json"

#define LOAD_RESOURCE_TREE "../Assets/LevelFiles/ResourceTree.json"

// 加载字体文本
// FreeTypeFont* Game::mFreeTypeFont = new FreeTypeFont("../Assets/Texts/Deng.ttf");

Game::Game()
{
    this->mRenderer = nullptr;
    this->mAudioSystem = nullptr;
    this->mUpdatingActors = false;
    this->mIsRunning = true;
    this->mCreateAsteroidTime = 0.0f;
    this->mCameraActor = nullptr;
    this->mCrosshair = nullptr;
    this->mFPSActor = nullptr;
    this->mGameState = EGamePlay;
    this->mTimeCounter = 0;
    this->mTicksCount = SDL_GetTicks();

    this->mHUD = nullptr;
    this->mDialogBox = nullptr;
    this->mPauseMenu = nullptr;
    this->mSetting = nullptr;
    this->mConsole = nullptr;
}

Game::Game(const Game& game)
{
}

Game& Game::operator=(const Game& other)
{
    if (this == &other) {
        return *this;
    }
    this->mRenderer = other.mRenderer;
    return (*this);
}

Game::~Game()
{   
    // 释放所有资源
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    while (!mPendingActors.empty())
    {
        delete mPendingActors.back();
    }

    // while (!mUIStack.empty())
    // {
    //     delete mUIStack.back();
    // }
}   

Game& Game::getInstance()
{
    static Game instance;
    return instance;
}

bool Game::init()
{
    int sdlResult = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    if (sdlResult)
    {
        SDL_log(sdlResult);
        return false;
    }

    this->mAudioSystem = new AudioSystem(this);
    if (!this->mAudioSystem->init())
    {
        SDL_Log("Failed to initialize audio system...");
        delete this->mAudioSystem;
        this->mAudioSystem = nullptr;
        return false;
    }

    mRenderer = new Renderer(this);
	if (!mRenderer->init(200, 130, 1440, 768))
	{
		SDL_Log("Failed to initialize renderer...");
		delete this->mRenderer;
		this->mRenderer = nullptr;
		return false;
	}

    if (TTF_Init() != 0)
    {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

    Random::Init();

    loadData();

    mTicksCount = SDL_GetTicks();

    mIsRunning = true;
    return true;
}

void Game::runLoop()
{
    while (mIsRunning)
    {   
        try
        {
            processInput();
            updateGame();
            generateOutput();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    // 保存当前游戏状态
    LevelLoader::saveLevel(this, SAVE_PATH);

    // 保存游戏ui状态
    LevelLoader::saveUIElements(this, SAVE_HUD_PATH);
}

void Game::shutdown()
{
    std::cout << "Quit game..." << std::endl;

    while (!mActors.empty())
    {
        delete mActors.back();    
    }
    TTF_Quit();
    SDL_Quit();
    this->mAudioSystem->shutdown();
}

void Game::processInput()
{
    for (auto ui : this->mUIStack)
    {
        if (ui->getUIState() == UIScreen::EActiviting)
        {
            ui->setUIState(UIScreen::EActive);
        }
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            SDL_Log("[Game] key num: %d", (int)(event.key.keysym.sym));

            case SDL_QUIT:
            {
                mIsRunning = false;
                // this->mGameState = EQuit;
                break;
            }
            case SDL_KEYDOWN:
            {
                if (!event.key.repeat)
                {
                    if (this->mGameState == EGamePlay)
                    {
                        this->handleKey(event.key.keysym.sym);
                    }
                    else if (!this->mUIStack.empty())
                    {
                        for (auto ui : this->mUIStack)
                        {
                            if (ui->getUIState() == UIScreen::EActive)
                            {
                                ui->handleKeyPress(event.key.keysym.sym);
                            }
                        }
                        // SDL_Log("[UIScreen] type: %d", (int)(this->mUIStack.back()->getUIType()));
                    }
                }
                SDL_Log("[UIScreen] key num: %d", (int)(event.key.keysym.sym));
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                if (this->mGameState == EGamePlay)
                {
                    this->handleKey(event.button.button);
                }
                else
                {
                    if (!this->mUIStack.empty())
                    {
                        for (auto ui : this->mUIStack)
                        {
                            if (ui->getUIState() == UIScreen::EActive)
                            {
                                ui->handleKeyPress(event.button.button);
                                // SDL_Log("[Game] Active ui type: %d STATE-%d addr:%d", ui->getUIType(), ui->getUIState(), (int)ui);
                            }
                        }
                        // this->mUIStack.back()->handleKeyPress(event.button.button);
                        // SDL_Log("[UIScreen] type: %d", (int)(this->mUIStack.back()->getUIType()));
                    }

                    if (!this->mActors.empty())
                    {
                        for (auto act : this->mActors)
                        {
                            act->actorHandleMouse(event.button.button);
                        }
                    }
                    // if (this->mHUD)
                    // {
                    //     this->mHUD->handleKeyPress(event.button.button);
                    //     // SDL_Log("[Game] Update hud button...");
                    // }
                }
                std::cout << "[Game] Button down..." << std::endl;

                std::cout << "x: " << event.button.x << " y:" << event.button.y << std::endl;
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                SDL_MouseWheelEvent mouse = event.wheel;

                if (!this->mUIStack.empty())
                {
                    for (auto ui : this->mUIStack)
                    {
                        if (ui->getUIState() == UIScreen::EActive)
                        {
                            ui->handleMouseWheel(mouse.y);
                            // SDL_Log("[Game] Active ui type: %d STATE-%d addr:%d", ui->getUIType(), ui->getUIState(), (int)ui);
                        }
                    }
                    // this->mUIStack.back()->handleKeyPress(event.button.button);
                    // SDL_Log("[UIScreen] type: %d", (int)(this->mUIStack.back()->getUIType()));
                }

                if (this->mGameState == EGamePlay)
                {
                    mUpdatingActors = true;
                    for (auto actor : mActors)
                    {
                        if (actor != nullptr)
                        {
                            actor->handleMouseWheel(mouse.y);
                        }
                    }
                    mUpdatingActors = false;
                }

                // SDL_Log("[Game] Mouse wheel is scrolling: x: %d y: %d dir: %d preciseX: %.2f preciseY: %.2f", 
                //     mouse.x, mouse.y, mouse.direction, mouse.preciseX, mouse.preciseY
                // );

                // SDL_Log("[Game] Mouse wheel is scrolling: y: %d", mouse.y);

                break;
            }
            default:
            {
                break;
            }
        }
    }

    // 获取键盘状态
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    
    // Uint32 mouse_state = SDL_GetMouseState(nullptr, nullptr);

    // 如果按下Esc键则退出
    // if (keyboard_state[SDL_SCANCODE_ESCAPE])
    // {
    //     // mIsRunning = false;
    //     this->mGameState = EQuit;
    // }

    if (this->mGameState == EGamePlay)
    {
        mUpdatingActors = true;
        for (auto actor : mActors)
        {
            if (actor != nullptr)
            {
                actor->processInput(keyboard_state);
            }
        }
        mUpdatingActors = false;

        // this->mHUD->processInput(keyboard_state);
    }
    else
    {
        if (!this->mUIStack.empty())
        {
            for (auto ui : this->mUIStack)
            {
                if (ui->getUIState() == UIScreen::EActive)
                {
                    ui->processInput(keyboard_state);
                }
            }

            // this->mUIStack.back()->processInput(keyboard_state);
        }

        // if (this->mHUD)
        // {
        //     this->mHUD->processInput(keyboard_state);
        // }
    }
    // mShip->processInput(keyboard_state);

    // SDL_Log("[Game] process input...");
}

void Game::updateGame(string name)
{
    // SDL_Log("[Game] Update game st...");

    float dt = (SDL_GetTicks() - this->mTicksCount) / 1000.0f;
    this->mTicksCount = SDL_GetTicks();
    if (dt > 0.1f)
    {
        dt = 0.1f;
    }

    // 小行星生成
    // this->mCreateAsteroidTime += dt;
    // if (this->mCreateAsteroidTime >= 5.0f)
    // {
    //     new Asteroid(this);
    //     this->mCreateAsteroidTime = 0.0f;
    // }

    if (this->mGameState == EGamePlay)
    {
        // 更新所有游戏对象
        mUpdatingActors = true;
        for (auto actor : mActors)
        {
            if (actor != nullptr)
            {
                actor->update(dt);
            }
        }
        mUpdatingActors = false;

        // 将挂起的角色对象移进活动的角色对象容器
        for (auto pending : mPendingActors)
        {
            pending->computeWorldTransform();
            mActors.emplace_back(pending);
        }
        // 清空当前挂起的角色对象容器
        mPendingActors.clear();

        // SDL_Log("[Game] Update game mid1...");

        std::vector<Actor*> deadActors;
        for (auto actor : mActors)
        {
            if (actor != nullptr)
            {
                if (actor->getState() == Actor::EDead)
                {
                    deadActors.emplace_back(actor);
                }
            }
        }

        // 释放过逝的角色对象并从容器中删除
        for (auto actor : deadActors)
        {
            if (actor)
            {
                SDL_Log("Delete actor...");
                removeActor(actor);
                this->getRenderer()->removeSpriteComponent(actor->getSpriteComponent());
                delete actor;
                actor = nullptr;
            }
        }

        // SDL_Log("[Game] Update game mid2...");
    }
    else if (this->mGameState == EPaused)
    {
        auto temp = (SDL_GetTicks() - this->mTimeCounter) / 1000;
        if (temp > 2.0)
        {
            this->mTimeCounter = SDL_GetTicks();
            SDL_Log("[Game] Game is paused...");
        }
    }
    else if (this->mGameState == EQuit)
    {
        this->mIsRunning = false;
        SDL_Log("[Game] Game is quiting...");
    }

    // 更新音频系统
    this->mAudioSystem->update(dt);
    
    // SDL_Log("[Game] Update game mid3...");
    // 更新ui对象
    for (auto ui : this->mUIStack)
    {
        if (ui->getState() == UIScreen::EActive)
        {
            ui->update(dt);
        }
    }
    // SDL_Log("[Game] Update game mid4...");

    for (auto iter = this->mUIStack.begin(); iter != this->mUIStack.end();)
    {
        if ((*iter)->getState() == UIScreen::EClosing)
        {
            // delete (*iter);
            // auto temp = (*iter);

            SDL_Log("[UIScreen] Closing...");
            
            iter = this->mUIStack.erase(iter);

            // if (temp)
            // {
            //     delete temp;
            //     temp = nullptr;
            // }

            // Sleep(10000);
            // system("pause");
        }          
        else
        {
            ++iter;
        }
    }

    // 更新资源管理器
    // this->mResourceManager->update(false);

    // SDL_Log("[Game] Update game over...");
}

void Game::generateOutput()
{
    // bool flag = false;
    Actor* target = nullptr;

    Ray ray = this->mHUD->getClickRay();
    LineSegment ls = this->mHUD->getClickLine();
    // auto st = ls.mStart;
    // sort(this->mActors.begin(), this->mActors.end(), [st](Actor& a1, Actor& a2){
    //     return (a1.getPosition() - st).LengthSq() < (a2.getPosition() - st).LengthSq();
    // });

    // 测试线段在物理空间中的碰撞
    PhysWorld* phys = this->mPhysWorld;
    CollisionInfo info;
    if (this->mGameState == EPaused)
    {   // 暂停状态下检测鼠标是否选中物体
        if (phys->segmentCast(ls, info))
        {
            // 如果与线段相交，则选中该目标对象
            target = info.mCollActor;
            if (target && target->getState() == Actor::EActive)
            {
                target->setState(Actor::ESelected);

                // flag = true;
            }
            // SDL_Log("[Game] Find seleted actor: %d", (int)target->getType());
        }
        else
        {
            for (auto act : this->mActors)
            {
                if (act->getState() == Actor::EActive)
                {
                    continue;
                }
                else if (act->getState() == Actor::ESelected)
                {
                    act->setState(Actor::EActive);
                }
            }
        }

        if (target)
        {
            for (auto act : this->mActors)
            {
                if (act->getState() == Actor::EActive)
                {
                    continue;
                }
                else if (act != target && act->getState() == Actor::ESelected)
                {
                    act->setState(Actor::EActive);
                }
            }
        }
    }
    else if (this->mGameState == EGamePlay)
    {
        for (auto act : this->mActors)
        {
            act->setState(Actor::EActive);
        }
    }

    // SDL_Log("[Game] Update generate output...");

    this->mRenderer->draw();
}

void Game::addActor(class Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::removeActor(class Actor* actor)
{
    for (auto iter = mActors.begin(); iter != mActors.end(); ++iter)
    {
        if ((*iter) != nullptr)
        {
            if ((*iter) == actor)
            {
                iter_swap(iter, mActors.end() - 1);
                mActors.pop_back();
                break;
            }
        }
    }

    for (auto iter = mPendingActors.begin(); iter != mPendingActors.end(); ++iter)
    {
        if ((*iter) != nullptr)
        {
            if ((*iter) == actor)
            {
                iter_swap(iter, mPendingActors.end() - 1);
                mPendingActors.pop_back();
                break;
            }
        }
    }

    // for (auto iter = this->mLasers.begin(); iter != this->mLasers.end(); ++iter)
    // {
    //     if ((*iter) != nullptr)
    //     {
    //         if ((*iter) == actor)
    //         {
    //             iter_swap(iter, this->mLasers.end() - 1);
    //             this->mLasers.pop_back();
    //             break;
    //         }
    //     }
    // }

    // for (auto iter = this->mAsteroids.begin(); iter != this->mAsteroids.end(); ++iter)
    // {
    //     if ((*iter) != nullptr)
    //     {
    //         if ((*iter) == actor)
    //         {
    //             iter_swap(iter, this->mAsteroids.end() - 1);
    //             this->mAsteroids.pop_back();
    //             break;
    //         }
    //     }
    // }

    // if (!mUpdatingActors)
    // {
    //     for (auto iter = mActors.begin(); iter != mActors.end();)
    //     {
    //         if ((*iter) == actor)
    //         {
    //             iter = mActors.erase(iter);
    //             break;
    //         }
    //         else
    //         {
    //             ++iter;
    //         }
    //     }
    // }
    // else
    // {
    //     for (auto iter = mPendingActors.begin(); iter != mPendingActors.end();)
    //     {
    //         if ((*iter) == actor)
    //         {
    //             iter = mPendingActors.erase(iter);
    //             break;
    //         }
    //         else
    //         {
    //             ++iter;
    //         }
    //     }
    // }
}

void Game::loadData()
{   
    // mShip = new Ship(this);
    // mShip->setPosition(Vector3(0.0f, 0.0f, 0.0f));
    // mShip->setRotation(Quaternion(Vector3::UnitZ, Math::Pi));

    // const int numAsteroids = 5;
    // for (int ii = 0; ii < numAsteroids; ++ii)
    // {
    //     new Asteroid(this);
    // }

    // if (this->mAsteroids.size() == 0)
    // {
    //     cout << "gggGGG" << endl;
    // }

    // 加载文本
    this->loadText("../Assets/Texts/English.gptext");

    // 加载字体文本
    this->mFreeTypeFont = new FreeTypeFont(this);
    this->mFreeTypeFont->load("../Assets/Texts/Deng.ttf");

    // 创建物理碰撞检测对象
    this->mPhysWorld = new PhysWorld(this);

    // 创建资源管理类
    this->mResourceManager = new ResourceManager(this);

    // Actor* a = new Actor(this);
    // a->setPosition(Vector3(200.0f, 75.0f, 0.0f));
    // a->setScale(100.0);
    // Quaternion q(Vector3::UnitY, -Math::PiOver2);
    // q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi * 1.25));
    
    // // std::cout << "q: [" << q.x << q.y << q.z << q.w << "]" << std::endl;
    // a->setRotation(q);
    // MeshComponent* mc = new MeshComponent(a);
    // mc->setMesh(this->mRenderer->getMesh("../Assets/Meshes/Cube.gpmesh"));

    // a = new Actor(this);
    // a->setPosition(Vector3(200.0f, -75.0f, 0.0f));
    // a->setScale(3.0f);
    // mc = new MeshComponent(a);
    // mc->setMesh(this->mRenderer->getMesh("../Assets/Meshes/Sphere.gpmesh"));

    // a = new Actor(this);
    // a->setPosition(Vector3(100.0f, 45.0f, 88.0f));
    // a->setScale(100.0f);
    // mc = new MeshComponent(a);
    // mc->setMesh(this->mRenderer->getMesh("../Assets/Meshes/Fox.gpmesh"));

    // // 设置地板
    // const float start = -1250.0f;
    // const float size = 250.0f;
    // for (int i = 0; i < 10; ++i)
    // {
    //     for (int j = 0; j < 10; ++j)
    //     {
    //         a = new PlaneActor(this);
    //         Vector3 pos = Vector3(start + i * size, start + j * size, -100.0f);
    //         a->setPosition(pos);

    //         // 创建点光源
    //         a = new Actor(this);
    //         pos.z += 100.0f;
    //         a->setPosition(pos);
    //         PointLightComponent* p = new PointLightComponent(a);
    //         Vector3 color;
    //         switch ((i + j) % 5)
    //         {
    //         case 0:
    //             color = Color::Green;
    //             break;
    //         case 1:
    //             color = Color::Blue;
    //             break;
    //         case 2: 
    //             color = Color::Red;
    //             break;
    //         case 3:
    //             color = Color::Yellow;
    //             break;
    //         case 4:
    //             color = Color::LightPink;
    //             break;
    //         default:
    //             break;
    //         }
    //         p->mDiffuseColor = color;
    //         p->mSpecColor = (color * 0.5);
    //         p->mInnerRadius = 100.0f;
    //         p->mOuterRadius = 200.0f;
    //     }
    // }

    // // 左/右墙体
    // q = Quaternion(Vector3::UnitX, Math::PiOver2);
    // for (int i = 0; i < 10; i++)
    // {
    //     a = new PlaneActor(this);
    //     a->setPosition(Vector3(start + i * size, start - size, 0.0f));
    //     a->setRotation(q);

    //     a = new PlaneActor(this);
    //     a->setPosition(Vector3(start + i * size, -start + size, 0.0f));
    //     a->setRotation(q);
    // }

    // // 前/后墙体
    // q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
    // for (int i = 0; i < 10; i++)
    // {
    //     a = new PlaneActor(this);
    //     a->setPosition(Vector3(start - size, start + i * size, 0.0f));
    //     a->setRotation(q);

    //     a = new PlaneActor(this);
    //     a->setPosition(Vector3(-start + size, start + i * size, 0.0f));
    //     a->setRotation(q);
    // }

    unsigned buffer = -1;
    class Texture* tex = nullptr;

    float w = this->getRenderer()->getScreenWidth();
    float h = this->getRenderer()->getScreenHeight();

    // 开启平面显示器
    this->mHUD = new HUD(this);
    if (this->mRenderer->createUIFrameBuffer(buffer, tex, true))
    {
        this->mHUD->setBindFrameBuffer(buffer);
        this->mHUD->setBindTexture(tex);

        this->mHUD->setUIViewScale(Vector2{1.0f, 1.0f});
        this->mHUD->setUIPosOffset(Vector2{
            -this->mHUD->getUIBufferPos().x, 
            h - this->mHUD->getUIBufferPos().y - this->mHUD->getUIBufferViewSize().y
        });
        this->mHUD->setUIBGColor(Vector3{0.1f, 0.25f, 0.25f});
    }
    this->mHUD->setUIState(UIScreen::EActive);
    this->pushUI(this->mHUD);

    buffer = -1;
    tex = nullptr;

    // 添加资源管理菜单
    this->mResourceMenu = new ResourceMenu(this);
    if (this->mRenderer->createUIFrameBuffer(buffer, tex))
    {
        this->mResourceMenu->setBindFrameBuffer(buffer);
        this->mResourceMenu->setBindTexture(tex);

        // auto scale = this->mHUD->getUIViewScale();
        this->mResourceMenu->setUIViewScale(Vector2{0.10f, 0.50f});
        this->mResourceMenu->setUIPosOffset(Vector2{
            w - this->mResourceMenu->getUIBufferPos().x - this->mResourceMenu->getUIBufferViewSize().x, 
            h - this->mResourceMenu->getUIBufferPos().y - this->mResourceMenu->getUIBufferViewSize().y
        });
        this->mResourceMenu->setUIBGColor(Vector3{0.25f, 0.35f, 0.35f});

        Button* left = new Button(
            this->mResourceMenu,
            "left_border", "button",
            Vector2{mResourceMenu->getUIBufferPos().x + mResourceMenu->getUIPosOffset().x - mRenderer->getScreenWidth() * 0.5f, 
                mResourceMenu->getUIBufferPos().y + mResourceMenu->getUIPosOffset().y - mRenderer->getScreenHeight() * 0.5f + this->mResourceMenu->getUIBufferViewSize().y * 0.5f
            }, Vector2{20.0f, this->mResourceMenu->getUIBufferViewSize().y}, Vector2{1.0f, 1.0f},
            Vector3{0.45f, 0.45f, 0.45f}, Color::White,
            {}, Vector2{0.0f, 0.0f}, -1, [this](){
                SDL_Log("[Game] Resource menu left border button...");
            },
            true
        );

        Button* bottom = new Button(
            this->mResourceMenu,
            "bottom_border", "button",
            Vector2{mResourceMenu->getUIBufferPos().x + mResourceMenu->getUIPosOffset().x - mRenderer->getScreenWidth() * 0.5f + this->mResourceMenu->getUIBufferViewSize().x * 0.5f, 
                mResourceMenu->getUIBufferPos().y + mResourceMenu->getUIPosOffset().y - mRenderer->getScreenHeight() * 0.5f
            }, Vector2{this->mResourceMenu->getUIBufferViewSize().x, 20.0f}, Vector2{1.0f, 1.0f},
            Vector3{0.45f, 0.45f, 0.45f}, Color::White,
            {}, Vector2{0.0f, 0.0f}, -1, [this](){
                SDL_Log("[Game] Resource menu bottom border button...");
            }, 
            true
        );

        Button* right = new Button(
            this->mResourceMenu,
            "right_border", "button",
            Vector2{mResourceMenu->getUIBufferPos().x + mResourceMenu->getUIPosOffset().x - mRenderer->getScreenWidth() * 0.5f + this->mResourceMenu->getUIBufferViewSize().x, 
                mResourceMenu->getUIBufferPos().y + mResourceMenu->getUIPosOffset().y - mRenderer->getScreenHeight() * 0.5f + this->mResourceMenu->getUIBufferViewSize().y - this->mResourceMenu->getUIBufferViewSize().y * 0.25f
            }, Vector2{20.0f, this->mResourceMenu->getUIBufferViewSize().y * 0.5f}, Vector2{1.0f, 1.0f}, 
            Vector3{0.45f, 0.45f, 0.45f}, Color::White,
            {}, Vector2{0.0f, 0.0f}, -1, [this](){
                SDL_Log("[Game] Resource menu right border button...");
            },
            true
        );
        
        this->mResourceMenu->setUIBorderPos(left->getPosition(), bottom->getPosition(), right->getPosition());
    }
    this->mResourceMenu->setUIState(UIScreen::EInvisible);
    this->pushUI(this->mResourceMenu);

    buffer = -1;
    tex = nullptr;

    // 添加属性菜单
    this->mPropertyMenu = new PropertyMenu(this);
    if (this->mRenderer->createUIFrameBuffer(buffer, tex))
    {
        this->mPropertyMenu->setBindFrameBuffer(buffer);
        this->mPropertyMenu->setBindTexture(tex);

        // auto scale = this->mHUD->getUIViewScale();
        auto scale = this->mResourceMenu->getUIViewScale();
        this->mPropertyMenu->setUIViewScale(Vector2{scale.x, 1.0f - scale.y});
        this->mPropertyMenu->setUIPosOffset(Vector2{
            w - this->mPropertyMenu->getUIBufferPos().x - this->mPropertyMenu->getUIBufferViewSize().x, 
            -this->mPropertyMenu->getUIBufferPos().y}
        );
        this->mPropertyMenu->setUIBGColor(Vector3{0.20f, 0.20f, 0.20f});

        Button* right = new Button(
            this->mPropertyMenu,
            "right_border", "button",
            Vector2{this->mPropertyMenu->getUIBufferPos().x + this->mPropertyMenu->getUIPosOffset().x - w * 0.5f + this->mPropertyMenu->getUIBufferViewSize().x, 
                this->mPropertyMenu->getUIBufferPos().y + this->mPropertyMenu->getUIPosOffset().y - h * 0.5f + this->mPropertyMenu->getUIBufferViewSize().y * 0.5f
            }, Vector2{20.0f, this->mPropertyMenu->getUIBufferViewSize().y}, Vector2{1.0f, 1.0f}, 
            Vector3{0.45f, 0.45f, 0.45f}, Color::White,
            {}, Vector2{0.0f, 0.0f}, -1, [this](){
                SDL_Log("[Game] Console right border button...");
            }, 
            true
        );
    }
    this->mPropertyMenu->setUIState(UIScreen::EInvisible);
    this->pushUI(this->mPropertyMenu);

    buffer = -1;
    tex = nullptr;
    
    // 添加控制台
    this->mConsole = new Console(this);
    if (this->mRenderer->createUIFrameBuffer(buffer, tex))
    {
        this->mConsole->setBindFrameBuffer(buffer);
        this->mConsole->setBindTexture(tex);

        auto scale = this->mResourceMenu->getUIViewScale();
        // auto scale = this->mHUD->getUIViewScale();
        this->mConsole->setUIViewScale(Vector2{scale.x, 0.10f});
        this->mConsole->setUIPosOffset(Vector2{
            -this->mConsole->getUIBufferPos().x,
            -this->mConsole->getUIBufferPos().y
        });
        this->mConsole->setUIBGColor(Vector3{0.15f, 0.15f, 0.15f});

        Button* up = new Button(
            this->mConsole,
            "up_border", "button",
            Vector2{this->mConsole->getUIBufferPos().x + this->mConsole->getUIPosOffset().x - w * 0.5f + this->mConsole->getUIBufferViewSize().x * 0.5f, 
                this->mConsole->getUIBufferPos().y + this->mConsole->getUIPosOffset().y - h * 0.5f + this->mConsole->getUIBufferViewSize().y
            }, Vector2{this->mConsole->getUIBufferViewSize().x, 20.0f}, Vector2{1.0f, 1.0f},
            Vector3{0.45f, 0.45f, 0.45f}, Color::White,
            {}, Vector2{0.0f, 0.0f}, -1, [this](){
                SDL_Log("[Game] Console up border button...");
            }, 
            true
        );

        Button* right = new Button(
            this->mConsole,
            "right_border", "button",
            Vector2{this->mConsole->getUIBufferPos().x + this->mConsole->getUIPosOffset().x - w * 0.5f + this->mConsole->getUIBufferViewSize().x, 
                this->mConsole->getUIBufferPos().y + this->mConsole->getUIPosOffset().y - h * 0.5f + this->mConsole->getUIBufferViewSize().y * 0.5f
            }, Vector2{20.0f, this->mConsole->getUIBufferViewSize().y}, Vector2{1.0f, 1.0f},
            Vector3{0.45f, 0.45f, 0.45f}, Color::White,
            {}, Vector2{0.0f, 0.0f}, -1, [this](){
                SDL_Log("[Game] Console right border button...");
            }, 
            true
        );
    }
    this->mConsole->setUIState(UIScreen::EInvisible);
    this->pushUI(this->mConsole);

    buffer = -1;
    tex = nullptr;

    // 添加暂停菜单
    this->mPauseMenu = new PauseMenu(this);
    if (this->mRenderer->createUIFrameBuffer(buffer, tex))
    {
        this->mPauseMenu->setBindFrameBuffer(buffer);
        this->mPauseMenu->setBindTexture(tex);

        this->mPauseMenu->setUIViewScale(Vector2{0.75f, 0.75f});
        this->mPauseMenu->setUIBGColor(Vector3{0.1f, 0.5f, 0.5f});
    }
    this->mPauseMenu->setUIState(UIScreen::EInvisible);
    this->pushUI(this->mPauseMenu);

    buffer = -1;
    tex = nullptr;

    // 添加日志菜单
    this->mDialogBox = new DialogBox(this);
    if (this->mRenderer->createUIFrameBuffer(buffer, tex))
    {
        this->mDialogBox->setBindFrameBuffer(buffer);
        this->mDialogBox->setBindTexture(tex);

        this->mDialogBox->setUIViewScale(Vector2{0.75f, 0.75f});
        this->mDialogBox->setUIBGColor(Vector3{0.30f, 0.30f, 0.30f});
    }
    this->mDialogBox->setUIState(UIScreen::EInvisible);
    this->pushUI(this->mDialogBox);

    buffer = -1;
    tex = nullptr;

    // 添加设置菜单
    this->mSetting = new Setting(this);
    if (this->mRenderer->createUIFrameBuffer(buffer, tex))
    {
        this->mSetting->setBindFrameBuffer(buffer);
        this->mSetting->setBindTexture(tex);

        this->mSetting->setUIViewScale(Vector2{0.75f, 0.75f});
        this->mSetting->setUIBGColor(Vector3{0.1f, 0.5f, 0.5f});
    }
    this->mSetting->setUIState(UIScreen::EInvisible);
    this->pushUI(this->mSetting);


    // ui元素
    // a = new Actor(this);
    // a->setPosition(Vector3(-350.0f, -250.0f, 0.0f));
    // SpriteComponent* sc = new SpriteComponent(a);
    // sc->setTexture(this->mRenderer->getTexture("../Assets/Images/HealthBar.png"));

    // 开启音乐
    this->mMusicEvent = this->mAudioSystem->playEvent("event:/Music");
    
    // 设置灯光
    // this->mRenderer->setAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
    // DirectionalLight& dirLight = this->getRenderer()->getDirectionalLight();
    // dirLight.mDirection = Vector3(0.0f, -0.707f, -0.707f);
    // dirLight.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
    // dirLight.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

    LevelLoader::loadLevel(this, LOAD_PATH);
    LevelLoader::loadUIElements(this, LOAD_HUD_PATH);

    // 添加ui树
    LevelLoader::loadUITrees(this, LOAD_UITREE_PATH, mHUD);

    // 添加资源菜单树
    LevelLoader::loadUITrees(this, LOAD_RESOURCE_TREE, mResourceMenu);

    // 相机角色
    // this->mCameraActor = new CameraActor(this);
    // SDL_Log("Camera pos: (%f, %f, %f)", 
    //     this->mCameraActor->getPosition().x, 
    //     this->mCameraActor->getPosition().y, 
    //     this->mCameraActor->getPosition().z
    // );

    // a = new Actor(this);
    // a->setPosition(Vector3(375.0f, -225.0f, 0.0f));
    // a->setScale(0.75f);
    // sc = new SpriteComponent(a);
    // sc->setTexture(this->mRenderer->getTexture("../Assets/Images/Scene.png"));

    // a = new Actor(this);
    // a->setScale(2.0f);
    // this->mCrosshair = new SpriteComponent(a);
    // this->mCrosshair->setTexture(this->mRenderer->getTexture("../Assets/Images/Images/Crosshair.png"));

    // a = new Actor(this);
    // a->setPosition(Vector3(10.0f, 10.0f, -10.0f));
    // a->setScale(0.75f);
    // mc = new MeshComponent(a);
    // mc->setMesh(this->mRenderer->getMesh("../Assets/Rifle.gpmesh"));

    // 创建包含音效组件的球体
    // a = new Actor(this);
    // a->setPosition(Vector3(500.0f, -75.0f, 0.0f));
    // a->setScale(1.0f);
    // mc = new MeshComponent(a);
    // mc->setMesh(this->mRenderer->getMesh("../Assets/Meshes/Sphere.gpmesh"));
    // AudioComponent* ac = new AudioComponent(a);
    // ac->playEvent("event:/FireLoop");

    // 鼠标相对坐标使能
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GetRelativeMouseState(nullptr, nullptr);

    // fps角色
    // this->mFPSActor = new FPSActor(this);

    // 跟拍相机
    // this->mFollowActor = new FollowActor(this);

    // 轨道相机
    // this->mOrbitActor = new OrbitActor(this);

    // 样条曲线相机
    // this->mSplineActor = new SplineActor(this);

    // 创建目标对象
    // a = new TargetActor(this);
    // a->setPosition(Vector3(1450.0f, 0.0f, 100.0f));
    
    // a = new TargetActor(this);
    // a->setPosition(Vector3(1450.0f, 0.0f, 400.0f));

    // a = new TargetActor(this);
    // a->setPosition(Vector3(1450.0f, -500.0f, 200.0f));

    // a = new TargetActor(this);
    // a->setPosition(Vector3(1450.0f, 500.0f, 200.0f));

    // a = new TargetActor(this);
    // a->setPosition(Vector3(0.0f, -1450.0f, 200.0f));
    // a->setRotation(Quaternion(Vector3::UnitZ, Math::PiOver2));

    // a = new TargetActor(this);
    // a->setPosition(Vector3(0.0f, 1450.0f, 200.0f));
    // a->setRotation(Quaternion(Vector3::UnitZ, -Math::PiOver2));

    // auto acts = this->getActors();
    // std::cout << "[Game] Actors Size:" << acts.size() << std::endl;

    for (auto act : this->mActors)
    {
        if (act->getType() == Actor::EFPSActor)
        {
            this->mFPSActor = (FPSActor*)act;
            break;
        }
    }
}

void Game::processInput(const uint8_t* keyboard_state)
{
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        if (actor != nullptr)
        {
            actor->processInput(keyboard_state);
        }
    }
    mUpdatingActors = false;
}

std::vector<class Asteroid*>& Game::getAsteroids()
{
    return this->mAsteroids;
}

void Game::addAsteroid(class Asteroid* ast)
{
    this->mAsteroids.emplace_back(ast);
}

void Game::addLaser(Laser* laser)
{
    this->mLasers.emplace_back(laser);
}

void Game::setRunningState(bool isRunning)
{
    this->mIsRunning = isRunning;
}

Renderer* Game::getRenderer()
{
    if (this->mRenderer)
    {
        return this->mRenderer;
    }
    else
    {
        SDL_Log("[Game] Renderer is null...");
        return nullptr;
    }
}

AudioSystem* Game::getAudioSystem()
{
    return this->mAudioSystem;
}

void Game::handleKey(const uint8_t key)
{
    switch (key)
    {
        case SDLK_ESCAPE:
        {
            if (this->mGameState == EGamePlay)
            {
                // 设置为暂停状态
                this->setGameState(EPaused);

                // new PauseMenu(this);
                if (this->mPauseMenu)
                {
                    // this->mPauseMenu->init();
                    this->mPauseMenu->setUIState(UIScreen::EActive);
                    // this->pushUI(this->mPauseMenu);

                    SDL_Log("[Game] PauseMenu PUSH...");
                }
                else
                {
                    SDL_Log("[Game] PauseMenu is null...");
                }

                // 设置鼠标为相对模式
                this->mPauseMenu->setRelativeMouseMode(false);

                SDL_Log("[Game] Gameplay...");
            }
            else if (this->mGameState == EPaused)
            {
                // for (auto iter = this->mUIStack.begin(); iter != this->mUIStack.end(); ++iter)
                // {
                //     if ((*iter)->getUIType() == UIScreen::EPauseMenu)
                //     {
                //         (*iter)->setUIState(UIScreen::EInvisible);
                //         // iter = this->mUIStack.erase(iter);
                //         break;
                //     }
                // }

                if (this->mPauseMenu)
                {
                    this->mPauseMenu->setUIState(UIScreen::EInvisible);
                }

                // 设置为进行状态
                this->setGameState(EGamePlay);
                this->mPauseMenu->setRelativeMouseMode(true);

                SDL_Log("[Game] Epaused...");
            }

            break;
        }
        case SDLK_TAB:
        {
            if (this->mGameState == EGamePlay)
            {
                // 设置为暂停状态
                this->setGameState(EPaused);

                if (this->mSetting)
                {
                    // new Setting(this);

                    // this->mSetting->init();
                    this->mSetting->setUIState(UIScreen::EActive);
                    // this->pushUI(this->mSetting);
                }
                // 取消鼠标为相对模式
                this->mSetting->setRelativeMouseMode(false);
            }
            break;
        }
        case SDLK_SPACE:
        {
            if (this->mGameState == EGamePlay)
            {
                // 设置为暂停状态
                this->setGameState(EPaused);

                if (this->mResourceMenu)
                {
                    this->mResourceMenu->setUIState(UIScreen::EActive);
                    // this->pushUI(this->mResourceMenu);
                }

                if (this->mPropertyMenu)
                {
                    this->mPropertyMenu->setUIState(UIScreen::EActive);
                }

                if (this->mConsole)
                {
                    this->mConsole->setUIState(UIScreen::EActive);
                }

                // 取消鼠标为相对模式
                this->mHUD->setRelativeMouseMode(false);
            }
            else if (this->mGameState == EPaused)
            {
                // for (auto iter = this->mUIStack.begin(); iter != this->mUIStack.end(); ++iter)
                // {
                //     if ((*iter)->getUIType() == UIScreen::EResourceMenu)
                //     {
                //         iter = this->mUIStack.erase(iter);
                //         break;
                //     }
                // }

                if (this->mResourceMenu)
                {
                    this->mResourceMenu->setUIState(UIScreen::EInvisible);
                    // this->pushUI(this->mResourceMenu);
                }

                if (this->mPropertyMenu)
                {
                    this->mPropertyMenu->setUIState(UIScreen::EInvisible);
                }

                if (this->mConsole)
                {
                    this->mConsole->setUIState(UIScreen::EInvisible);
                }

                // 设置为运行态
                this->setGameState(EGamePlay);

                // 设置鼠标为相对模式
                this->mHUD->setRelativeMouseMode(true);
            }

            break;
        }
        // case SDL_SCANCODE_C:
        // case SDLK_c:
        // {
        //     // 设置为暂停状态
        //     this->setGameState(EPaused);

        //     if (this->mConsole)
        //     {
        //         this->mConsole->setUIState(UIScreen::EActive);
        //     }

        //     // 取消鼠标为相对模式
        //     this->mConsole->setRelativeMouseMode(false);

        //     break;
        // }
        case SDLK_RETURN:
        {
            SDL_Log("[Game] Enter...");

            break;
        }
        case SDLK_BACKSPACE:
        {
            SDL_Log("[Game] Backspace...");

            break;
        }
        case SDLK_w:
        {
            SDL_Log("[Game] press w...");

            break;
        }
        case SDL_BUTTON_LEFT:
        {
            this->mFPSActor->shoot();
            break;
        }
        case '-':
        {
            float volume = this->mAudioSystem->getBusVolume("bus:/");
            volume = Math::Max(0.0f, volume - 0.1f);
            this->mAudioSystem->setBusVolume("bus:/", volume);
            SDL_Log("Volume is decreasing: %.3f", volume);
            break;
        }
        case '=':
        {
            float volume = this->mAudioSystem->getBusVolume("bus:/");
            volume = Math::Min(1.0f, volume + 0.1f);
            this->mAudioSystem->setBusVolume("bus:/", volume);
            SDL_Log("Volume is increasing: %.3f", volume);
            break;
        }
        case 'e':
        {
            // 播放爆破声
            this->mAudioSystem->playEvent("event:/Explosion2D");
            SDL_Log("Play explosion2D music...");
            break;
        }
        case 'm':
        {
            // 触发音乐暂停暂停
            this->mMusicEvent.setPaused(!this->mMusicEvent.getPaused());
            SDL_Log("Stop music...");
            break;
        }
        case 'r':
        {
            // 停止/重启混响快照
            if (!this->mReverbSnap.isValid())
            {
                this->mReverbSnap = this->mAudioSystem->playEvent("snapshot:/WithReverb");
            }
            else
            {
                this->mReverbSnap.stop();
            }
            SDL_Log("Stop or restart reverb snap...");
            break;
        }
        case '1':
        {
            // 设置默认脚步表面
            // this->mCameraActor->setFootstepSurface(0.0);
            this->mFPSActor->setFootstepSurface(0.0);
            // this->mFollowActor->setFootstepSurface(0.0f);
            SDL_Log("Set default footstep surface...");
            break;
        }
        case '2':
        {
            // 设置草地的脚步表面
            // this->mCameraActor->setFootstepSurface(0.5);
            this->mFPSActor->setFootstepSurface(0.5);
            // this->mFollowActor->setFootstepSurface(0.5f);
            SDL_Log("Set grass footstep surface...");
            break;
        }
        default:
        {
            break;
        }
    }
}

PhysWorld* Game::getPhysWorld()
{
    return this->mPhysWorld;
}

std::vector<class PlaneActor*>& Game::getPlanes()
{
    return this->mPlanes;
}

void Game::addPlane(class PlaneActor* plane)
{
    this->mPlanes.emplace_back(plane);
}

void Game::removePlane(class PlaneActor* plane)
{
    for (auto iter = this->mPlanes.begin(); iter != this->mPlanes.end();)
    {
        if ((*iter) == plane)
        {
            iter = this->mPlanes.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }
}

const std::vector<class UIScreen*>& Game::getUIStack()
{
    return this->mUIStack;
}

void Game::pushUI(class UIScreen* screen)
{
    this->mUIStack.emplace_back(screen);
}

void Game::setGameState(GameState state)
{
    this->mGameState = state;
    
    std::cout << "State:" << (state == 0 ? "GamePlay" : state == 1 ? "GamePaused" : "GameQuit") << std::endl;
}

Actor* Game::getPlayer() const
{
    // Actor* a = nullptr;
    // for (auto act : this->mActors)
    // {
    //     if (act->getType() == Actor::EFollowActor)
    //     {
    //         a = act;
    //         break;
    //     }
    // }

    return this->mFPSActor;
    // return a;
}

Font* Game::getFont(const std::string& name)
{
    Font* font = nullptr;
    auto iter = this->mFonts.find(name);
    if (iter != this->mFonts.end())
    {
        font = iter->second;
    }
    else
    {
        font = new Font(this);
        if (font->load(name))
        {
            this->mFonts.emplace(name, font);
        }
        else
        {
            font->unLoad();
            delete font;
            font = nullptr;

            SDL_Log("[Game] Font load error...");
        }
    }
    return font;
}

FreeTypeFont* Game::getFreeTypeFont()
{
    return Game::mFreeTypeFont;
}

void Game::loadText(const std::string& name)
{   
    this->mText.clear();

    std::ifstream file(name);
    if (!file.is_open())
    {
        SDL_Log("Text file %s not found", name.c_str());
        return;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string content = fileStream.str();

    // 以json格式读取文本
    rapidjson::StringStream jsonStr(content.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);
    if (!doc.IsObject())
    {
        SDL_Log("Text file %s is not valid JSON", name.c_str());
        return;
    }

    // 解析文本
    const rapidjson::Value& actions = doc["TextMap"];
    for (rapidjson::Value::ConstMemberIterator iter = actions.MemberBegin(); iter != actions.MemberEnd(); ++iter)
    {
        if (iter->name.IsString() && iter->value.IsString())
        {
            this->mText.emplace(iter->name.GetString(), iter->value.GetString());
        }
        else
        {
            SDL_Log("[Game] %s or %s is not string...", iter->name.GetString(), iter->value.GetString());
        }
    }
}

std::string Game::getText(const std::string& key)
{
    // static std::string errorMsg("**Key not Found!!!**");
    
    std::string text = "Key not Found";
    auto iter = this->mText.find(key);
    if (iter != this->mText.end())
    {
        text = (*iter).second;
    }
    else
    {
        SDL_Log("[Game] No text: %s", text.c_str());
    }

    // SDL_Log("[Game] Text: %s", text.c_str());

    return text;    
}

void Game::unLoadData()
{
    // while (!this->mActors.empty())
    // {
    //     delete this->mActors.back();
    // }

    // while (!this->mUIStack.empty())
    // {
    //     delete this->mUIStack.back();
    // }
    
    if (this->mRenderer)
    {
        this->mRenderer->unLoadData();
    }
    
}


class Skeleton* Game::getSkeleton(const std::string& fileName)
{
    Skeleton* sk = nullptr;
    auto iter = this->mSkeletons.find(fileName);
    if (iter != this->mSkeletons.end())
    {
        sk = (*iter).second;
    }
    else
    {
        sk = new Skeleton();
        if (sk->load(fileName))
        {
            this->mSkeletons.emplace(fileName, sk);
        }
        else
        {
            SDL_Log("[Game] Load Skeleton failed in file %s", fileName.c_str());
            if (sk)
            {
                delete sk;
                sk = nullptr;
            }
        }
    }
    return sk;
}

class Animation* Game::getAnimation(const std::string& fileName)
{
    Animation* anim = nullptr;
    auto iter = this->mAnimations.find(fileName);
    if (iter != this->mAnimations.end())
    {
        anim = (*iter).second;
    }
    else
    {
        anim = new Animation();
        if (anim->load(fileName))
        {
            this->mAnimations.emplace(fileName, anim);
        }
        else
        {
            SDL_Log("[Game] Load Animation failed in file %s", fileName.c_str());
            if (anim)
            {
                delete anim;
                anim = nullptr;
            }
        }
    }
    return anim;
}

const std::vector<class Actor*>& Game::getActors() const
{
    return this->mActors;
}

class HUD* Game::getHUD()
{
    return this->mHUD;
}

class PauseMenu* Game::getPauseMenu()
{
    return this->mPauseMenu;
}

class Setting* Game::getSetting()
{
    return this->mSetting;
}

class DialogBox* Game::getDialogBox()
{
    return this->mDialogBox;
}

const Game::GameState& Game::getGameState() const
{
    return this->mGameState;
}

class ResourceManager* Game::getResourceManager() const
{
    return this->mResourceManager;
}

ResourceMenu* Game::getResourceMenu()
{
    return this->mResourceMenu;
}

class Console* Game::getConsole()
{
    return this->mConsole;
}

class PropertyMenu* Game::getPropertyMenu()
{
    return this->mPropertyMenu;
}