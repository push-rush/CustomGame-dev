#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <functional>
#include <unordered_map>

#include <rapidjson.h>
#include <document.h>

#include <SDL.h>

#include "./Cmath.h"

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

    static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;
    static std::unordered_map<std::string, ComponentFunc> sComponentFactoryMap;
private:
    static bool loadJSON(const std::string& fileName, rapidjson::Document& outDoc);
    static void loadGlobalProperties(class Game* game, const rapidjson::Value& inObject);
    static void loadActors(class Game* game, const rapidjson::Value& inArray);
    static void loadComponents(class Actor* owner, const rapidjson::Value& inArray);

    static bool saveGlobalProperties(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inObj);
    static bool saveActors(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inArray);
    static bool saveComponents(rapidjson::Document::AllocatorType& alloc, class Actor* actor, rapidjson::Value& inArray);
};

class JsonHelper
{
private:
    
public:
    JsonHelper();
    ~JsonHelper();

    static bool getInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
    static bool getVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector3);
    static bool getString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString);
    static bool getRotation(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat);
    static bool getScale(const rapidjson::Value& inObject, const char* inProperty, float& outScale);

    static bool addInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, int value);
    static bool addVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, Vector3 value);
    static bool addString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, std::string value);
    static bool addRotation(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, Quaternion value);
    static bool addScale(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, float value);
};