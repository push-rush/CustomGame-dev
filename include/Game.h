#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
// #include <windows.h>

#include "SDL.h"

#include "./Audios/SoundEvent.h"

class Game
{
public:
    Game(/* args */);
    Game(const Game&);
    Game& operator=(const Game&);
    static Game& getInstance();
    ~Game();

    enum GameState
    {
        EGamePlay,
        EPaused,
        EQuit
    };

    bool init();
    void runLoop();
    void shutdown();

    void loadData();
    void unLoadData();
    void processInput(const uint8_t* keyboard_state);
    void handleKey(const uint8_t key);
    void loadText(const std::string& name);

    // getters
    std::vector<class Asteroid*>& getAsteroids();
    class Renderer* getRenderer();
    class AudioSystem* getAudioSystem();
    class PhysWorld* getPhysWorld();
    std::vector<class PlaneActor*>& getPlanes();
    class Actor* getPlayer() const;
    class Font* getFont(const std::string& name);
    const GameState& getGameState() const;

    std::string getText(const std::string& key);
    class Skeleton* getSkeleton(const std::string& fileName);
    class Animation* getAnimation(const std::string& fileName);
    const std::vector<class Actor*>& getActors() const;

    class HUD* getHUD();
    class PauseMenu* getPauseMenu();
    class Setting* getSetting();
    class DialogBox* getDialogBox();
    class ResourceMenu* getResourceMenu();
    class PropertyMenu* getPropertyMenu();
    class Console* getConsole();

    class ResourceManager* getResourceManager() const;

    // 返回ui栈的引用
    const std::vector<class UIScreen*>& getUIStack();
    // 将屏幕ui类添加到栈中
    void pushUI(class UIScreen* screen);

    // setters
    void setRunningState(bool);
    void setGameState(GameState);
    
    // add or remove
    void removeActor(class Actor*);
    void addActor(class Actor*);
    
    void addAsteroid(class Asteroid*);
    void addLaser(class Laser*);

    void addPlane(class PlaneActor*);
    void removePlane(class PlaneActor*);

private:
    // 输入处理/游戏更新/画面输出
    void processInput();
    void updateGame(std::string name = "PlanetaryWars");
    void generateOutput();

private:
    const int thickness = 15;
    bool mIsRunning;
    Uint32 mTicksCount;
    Uint32 mTimeCounter;

    float mCreateAsteroidTime;
    bool mUpdatingActors;

    class Ship* mShip;
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;
    std::vector<class Asteroid*> mAsteroids;
    std::vector<class Laser*> mLasers;
    std::vector<class PlaneActor*> mPlanes;
    std::unordered_map<std::string, class Font*> mFonts;
    std::unordered_map<std::string, std::string> mText;
    std::unordered_map<std::string, std::string> mTextMap;
    std::unordered_map<std::string, class Skeleton*> mSkeletons;
    std::unordered_map<std::string, class Animation*> mAnimations;

    // ui栈
    std::vector<class UIScreen*> mUIStack;
    class HUD* mHUD;
    class PauseMenu* mPauseMenu;
    class Setting* mSetting;
    class DialogBox* mDialogBox;
    class ResourceMenu* mResourceMenu;
    class Console* mConsole;
    class PropertyMenu* mPropertyMenu;

    // 游戏状态
    GameState mGameState;

    class CameraActor* mCameraActor;
    class Renderer* mRenderer;
    class AudioSystem* mAudioSystem;
    class SoundEvent mMusicEvent;
    class SoundEvent mReverbSnap;
    class SpriteComponent* mCrosshair;
    class FPSActor* mFPSActor;
    class FollowActor* mFollowActor;
    class OrbitActor* mOrbitActor;
    class SplineActor* mSplineActor;
    class PhysWorld* mPhysWorld;

    // 资源管理类
    class ResourceManager* mResourceManager;
    
public:
    // 字体类
    // static class FreeTypeFont* mFreeTypeFont;
    // static class FreeTypeFont* getFreeTypeFont();

    class FreeTypeFont* mFreeTypeFont;
    class FreeTypeFont* getFreeTypeFont();
};
