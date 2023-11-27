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

#include "../include/General/LevelLoader.h"

#include "fmod.hpp"
#include "fmod_studio.h"
#include "fmod_errors.h"
#include "rapidjson.h"
#include "document.h"

#define LOAD_PATH "../Assets/LevelFiles/SaveFile1.gplevel"
#define SAVE_PATH "../Assets/LevelFiles/SaveFile.gplevel"

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
	if (!mRenderer->init())
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
}

void Game::shutdown()
{
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
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                // this->mGameState = EQuit;
                break;
            case SDL_KEYDOWN:
                if (!event.key.repeat)
                {
                    if (this->mGameState == EGamePlay)
                    {
                        this->handleKey(event.key.keysym.sym);
                    }
                    else if (!this->mUIStack.empty())
                    {
                        this->mUIStack.back()->handleKeyPress(event.key.keysym.sym);
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (this->mGameState == EGamePlay)
                {
                    this->handleKey(event.button.button);
                }
                else if (!this->mUIStack.empty())
                {
                    this->mUIStack.back()->handleKeyPress(event.button.button);
                }
                std::cout << "[Game] Button down..." << std::endl;
                break;
            default:
                break;
        }
    }

    // 获取键盘状态
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
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
    }
    else if (!this->mUIStack.empty())
    {
        this->mUIStack.back()->processInput(keyboard_state);
    }
    // mShip->processInput(keyboard_state);
}

void Game::updateGame(string name)
{
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
    
    // 更新ui对象
    for (auto ui : this->mUIStack)
    {
        if (ui->getState() == UIScreen::EActive)
        {
            ui->update(dt);
        }
    }

    for (auto iter = this->mUIStack.begin(); iter != this->mUIStack.end();)
    {
        if ((*iter)->getState() == UIScreen::EClosing)
        {
            // delete (*iter);
            auto temp = (*iter);

            SDL_Log("[UIScreen] Closing...");
            
            iter = this->mUIStack.erase(iter);
            if (temp)
            {
                delete temp;
                temp = nullptr;
            }

            // Sleep(10000);
            // system("pause");
        }          
        else
        {
            ++iter;
        }
    }
}

void Game::generateOutput()
{
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

    // 创建物理碰撞检测对象
    this->mPhysWorld = new PhysWorld(this);

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

    // 开启平面显示器
    this->mHUD = new HUD(this);
    
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
        new PauseMenu(this);
        break;
    }
    case SDLK_TAB:
    {
        new Setting(this);
        break;
    }
    case SDL_BUTTON_LEFT:
    {
        // this->mFPSActor->shoot();
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
        // this->mFPSActor->setFootstepSurface(0.0);
        this->mFollowActor->setFootstepSurface(0.0f);
        SDL_Log("Set default footstep surface...");
        break;
    }
    case '2':
    {
        // 设置草地的脚步表面
        // this->mCameraActor->setFootstepSurface(0.5);
        // this->mFPSActor->setFootstepSurface(0.5);
        this->mFollowActor->setFootstepSurface(0.5f);
        SDL_Log("Set grass footstep surface...");
        break;
    }
    default:
        break;
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
    Actor* a = nullptr;
    for (auto act : this->mActors)
    {
        if (act->getType() == Actor::EFollowActor)
        {
            a = act;
            break;
        }
    }

    // return this->mFPSActor;
    return a;
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

FreeTypeFont* Game::getFreeTypeFont(const std::string& name)
{
    return nullptr;
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

class HUD* Game::getHUD()
{

    return this->mHUD;
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