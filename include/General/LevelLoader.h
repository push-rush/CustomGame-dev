#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <functional>
#include <unordered_map>
#include <codecvt>
#include <string>
#include <locale>
#include <wchar.h>

#include <rapidjson.h>
#include <document.h>

#include <SDL.h>

#include "./Cmath.h"


union String2WString
{
    std::string uString;
    std::wstring uWString;
    
    String2WString()
    {

    }

    ~String2WString()
    {

    }
};

class LevelLoader
{
private:
    typedef std::function<class Actor*(class Game*, const rapidjson::Value&)> ActorFunc;
    typedef std::function<class Component*(class Actor*, const rapidjson::Value&)> ComponentFunc;

public:
    LevelLoader(/* args */);
    ~LevelLoader();


    static bool loadLevel(class Game* game, const std::string& fileName);
    static bool saveLevel(class Game* game, const std::string& fileName);

    static bool loadUIElements(class Game* game, const std::string& fileName);
    static bool saveUIElements(class Game* game, const std::string& fileName);

    static bool loadUITrees(class Game* game, const std::string& fileName);

    static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;
    static std::unordered_map<std::string, ComponentFunc> sComponentFactoryMap;
private:
    static bool loadJSON(const std::string& fileName, rapidjson::Document& outDoc);
    static void loadGlobalProperties(class Game* game, const rapidjson::Value& inObject);
    static void loadActors(class Game* game, const rapidjson::Value& inArray);
    static void loadComponents(class Actor* owner, const rapidjson::Value& inArray);

    static void loadHUD(class Game* game, const rapidjson::Value& inObject, class UIScreen* ui);
    // static void loadPauseMenu(class Game* game, const rapidjson::Value& inObject);
    // static void loadDialogBox(class Game* game, const rapidjson::Value& inObject);
    // static void loadSetting(class Game* game, const rapidjson::Value& inObject);

    static bool saveGlobalProperties(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inObj);
    static bool saveActors(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inArray);
    static bool saveComponents(rapidjson::Document::AllocatorType& alloc, class Actor* actor, rapidjson::Value& inArray);

    static bool saveHUD(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inArray, class UIScreen* ui);

    static bool loadUITree(class Game* game, const rapidjson::Value& inObject, class UIScreen* ui);
};

class JsonHelper
{
private:
    
public:
    JsonHelper();
    ~JsonHelper();

    static bool getInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
    static bool getVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector3);
    static bool getVector2(const rapidjson::Value& inObject, const char* inProperty, Vector2& outVector2);

    static bool getString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString);
    static bool getRotation(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat);
    static bool getScale(const rapidjson::Value& inObject, const char* inProperty, float& outScale);

    static bool addInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, int value);
    static bool addVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, Vector3 value);
    static bool addVector2(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, Vector2 value);

    static bool addString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, std::string value);
    static bool addString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const std::string& name, std::string value);
    
    static bool addRotation(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, Quaternion value);
    static bool addScale(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, float value);
};