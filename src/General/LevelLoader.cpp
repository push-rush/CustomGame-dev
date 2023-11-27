#include "../../include/General/LevelLoader.h"
#include "../../include/Game.h"
#include "../../include/Renderers/Renderer.h"
#include "../../include/Actors/Actor.h"
#include "../../include/Actors/TargetActor.h"
#include "../../include/Actors/BallActor.h"
#include "../../include/Actors/FollowActor.h"
#include "../../include/Actors/FPSActor.h"
#include "../../include/Actors/PlaneActor.h"
#include "../../include/Actors/CameraActor.h"
#include "../../include/Actors/SplineActor.h"

#include "../../include/Components/Component.h"
#include "../../include/Components/MeshComponent.h"
#include "../../include/Components/BoxComponent.h"
#include "../../include/Components/AudioComponent.h"
#include "../../include/Components/BallMove.h"
#include "../../include/Components/CameraComponent.h"
#include "../../include/Components/CircleComponent.h"
#include "../../include/Components/FollowCamera.h"
#include "../../include/Components/FPSCamera.h"
#include "../../include/Components/InputComponent.h"
#include "../../include/Components/MirrorCamera.h"
#include "../../include/Components/MoveComponent.h"
#include "../../include/Components/OrbitCamera.h"
#include "../../include/Components/PointLightComponent.h"
#include "../../include/Components/SkeletalMeshComponent.h"
#include "../../include/Components/SplineCamera.h"
#include "../../include/Components/SpriteComponent.h"
#include "../../include/Components/TargetComponent.h"

#include <stringbuffer.h>
#include <prettywriter.h>

std::unordered_map<std::string, LevelLoader::ActorFunc> LevelLoader::sActorFactoryMap
{
    {"Actor", &Actor::create<Actor>},
    {"TargetActor", &Actor::create<TargetActor>},
    {"FollowActor", &Actor::create<FollowActor>},
    {"PlaneActor", &Actor::create<PlaneActor>},
    {"BallActor", &Actor::create<BallActor>},
    {"CameraActor", &Actor::create<CameraActor>},
    {"SplineActor", &Actor::create<SplineActor>},
    {"FPSActor", &Actor::create<FPSActor>},
};

std::unordered_map<std::string, LevelLoader::ComponentFunc> LevelLoader::sComponentFactoryMap
{
    {"Component", &Component::create<Component>},
    {"MeshComponent", &Component::create<MeshComponent>},
    {"BoxComponent", &Component::create<BoxComponent>},
    {"AudioComponent", &Component::create<AudioComponent>},
    {"BallMove", &Component::create<BallMove>},
    {"CameraComponent", &Component::create<CameraComponent>},
    {"CircleComponent", &Component::create<CircleComponent>},
    {"FollowCamera", &Component::create<FollowCamera>},
    {"FPSCamera", &Component::create<FPSCamera>},
    {"InputComponent", &Component::create<InputComponent>},
    {"MirrorCamera", &Component::create<MirrorCamera>},
    {"OrbitCamera", &Component::create<OrbitCamera>},
    {"PointLightComponent", &Component::create<PointLightComponent>},
    {"SkeletalMeshComponent", &Component::create<SkeletalMeshComponent>},
    {"SplineCamera", &Component::create<SplineCamera>},
    {"SpriteComponent", &Component::create<SpriteComponent>},
    {"TargetComponent", &Component::create<TargetComponent>}
};

LevelLoader::LevelLoader()
{

}

LevelLoader::~LevelLoader()
{

}

bool LevelLoader::loadLevel(class Game* game, const std::string& fileName)
{
    rapidjson::Document doc; 
    if (!loadJSON(fileName, doc))
    {
        SDL_Log("Failed to load level %s", fileName.c_str());
        return false;
    }

    int version = 0;
    if (!JsonHelper::getInt(doc, "version", version))
    {
        SDL_Log("Incorrect level file version for %s", fileName.c_str());
        return false;
    }

    // 加载全局属性
    const rapidjson::Value& globals = doc["globalProperties"];
    if (globals.IsObject())
    {
        loadGlobalProperties(game, globals);
    }
    
    // 加载指定角色
    const rapidjson::Value& actors = doc["actors"];
    if (actors.IsArray())
    {
        loadActors(game, actors);
    }

    return true;
}

bool LevelLoader::loadJSON(const std::string& fileName, rapidjson::Document& outDoc)
{
    // 将文件从磁盘加载到输入文件流
    std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        SDL_Log("File %s is not found!", fileName.c_str());
        return false;
    }

    // 获取文件大小
    std::ifstream::pos_type fileSize = file.tellg();

    // 将文件流指向文件开头
    file.seekg(0, std::ios::beg);

    // 文件大小+空终止符
    std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
    file.read(bytes.data(), static_cast<size_t>(fileSize));

    // 解析文件
    // printf("gplevel: %s \n", bytes.data());
    outDoc.Parse(bytes.data());
    if (!outDoc.IsObject())
    {
        SDL_Log("File %s is not valid JSON!", fileName.c_str());
        return false;
    }
    return true;
}

void LevelLoader::loadGlobalProperties(class Game* game, const rapidjson::Value& inObject)
{
    // 环境光
    Vector3 ambient;
    if (JsonHelper::getVector3(inObject, "ambientLight", ambient))
    {
        game->getRenderer()->setAmbientLight(ambient);
    }

    // 定向光
    const rapidjson::Value& dirObj = inObject["directionalLight"];
    if (dirObj.IsObject())
    {
        DirectionalLight& dirLight = game->getRenderer()->getDirectionalLight();
        JsonHelper::getVector3(dirObj, "direction", dirLight.mDirection);
        JsonHelper::getVector3(dirObj, "diffuseColor", dirLight.mDiffuseColor);
        JsonHelper::getVector3(dirObj, "specColor", dirLight.mSpecColor);

        // std::cout << "specColor:" << dirLight.mSpecColor.x 
        // << " " << dirLight.mSpecColor.y 
        // << " " << dirLight.mSpecColor.z 
        // << std::endl;
    }
}

void LevelLoader::loadActors(class Game* game, const rapidjson::Value& inArray)
{
    for (rapidjson::SizeType i = 0; i < inArray.Size(); ++i)
    {
        const rapidjson::Value& actorObj = inArray[i];
        if (actorObj.IsObject())
        {
            std::string type;
            if (JsonHelper::getString(actorObj, "type", type))
            {
                auto iter = sActorFactoryMap.find(type);
                if (iter != sActorFactoryMap.end())
                {
                    Actor* a = iter->second(game, actorObj["properties"]);
                    if (actorObj.HasMember("components"))
                    {
                        const rapidjson::Value& components = actorObj["components"];
                        if (components.IsArray())
                        {
                            loadComponents(a, components);         
                        }
                    }

                    // int typeID = 0;
                    // for (; typeID < Actor::NUM_ACTOR_TYPES; ++typeID)
                    // {
                    //     if (!strcmp(type.c_str(), Actor::typeNames[typeID]))
                    //     {
                    //         break;
                    //     }
                    // }

                    // Actor* a = nullptr;
                    // Actor::EActorType actTypeID = static_cast<Actor::EActorType>(typeID);
                    // auto actors = game->getActors();
                    // for (auto act : actors)
                    // {
                    //     if (act->getType() == actTypeID)
                    //     {
                    //         a = act;
                    //     }
                    // }

                    // if (!a)
                    // {
                    //     a = iter->second(game, actorObj["properties"]);
                    // }
                    // else
                    // {
                    //     a->loadProperties(actorObj["properties"]);
                    // }

                    // if (actorObj.HasMember("components"))
                    // {
                    //     const rapidjson::Value& components = actorObj["components"];
                    //     if (components.IsArray())
                    //     {
                    //         loadComponents(a, components);         
                    //     }
                    // }
                }
            }
            else
            {
                SDL_Log("Unknown actor type: %s", type.c_str());
            }
        }
    }
}

void LevelLoader::loadComponents(class Actor* actor, const rapidjson::Value& inArray)
{
    for (rapidjson::SizeType i = 0; i < inArray.Size(); ++i)
    {
        const rapidjson::Value& compObj = inArray[i];
        if (compObj.IsObject())
        {
            std::string type;
            if (JsonHelper::getString(compObj, "type", type))
            {
                auto iter = sComponentFactoryMap.find(type);
                if (iter != sComponentFactoryMap.end())
                {
                    int typeID = 0;
                    for (; typeID < Component::NUM_COMPONENT_TYPES; ++typeID)
                    {
                        if (!strcmp(Component::TypeNames[typeID], type.c_str()))
                        {
                            break;
                        }
                    }
                    Component::EComponentType compTypeID = static_cast<Component::EComponentType>(typeID);
                    Component* cp = nullptr;
                    for (auto comp : actor->getComponents())
                    {
                        if (comp->getType() == compTypeID)
                        {
                            cp = comp;
                        }
                    }

                    if (!cp)
                    {
                        SDL_Log("[LevelLoader] Create Component: %s...", type.c_str());
                        cp = (*iter).second(actor, compObj["properties"]);
                    }
                    else
                    {
                        SDL_Log("[LevelLoader] Update Component: %s...", type.c_str());
                        cp->loadProperties(compObj["properties"]);
                    }
                }
            }
        }
    }
}

JsonHelper::JsonHelper(/* args */)
{

} 

JsonHelper::~JsonHelper()
{

}

bool JsonHelper::getInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt)
{
    auto iter = inObject.FindMember(inProperty);
    if (iter == inObject.MemberEnd())
    {
        return false;
    }

    auto& property = iter->value;
    if (!property.IsInt())
    {
        return false;
    }

    outInt = property.GetInt();
    return true;
}

bool JsonHelper::getVector3(const rapidjson::Value& inObject, const char* inProperty, Vector3& outVector3)
{
    auto iter = inObject.FindMember(inProperty);
    if (iter == inObject.MemberEnd())
    {
        return false;
    }

    auto& property = iter->value;
    if (property.Size() != 3 || !property.IsArray())
    {
        return false;
    }

    for (rapidjson::SizeType i = 0; i < property.Size(); ++i)
    {
        if (!property[i].IsDouble())
        {
            return false;
        }
    }

    outVector3.x = property[0].GetDouble();
    outVector3.y = property[1].GetDouble();
    outVector3.z = property[2].GetDouble();

    return true;
}

bool JsonHelper::getString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString)
{
    auto iter = inObject.FindMember(inProperty);
    if (iter == inObject.MemberEnd())
    {
        return false;
    }

    auto& property = iter->value;
    if (!property.IsString())
    {
        return false;
    }

    outString = property.GetString();
    return true;
}

bool JsonHelper::getRotation(const rapidjson::Value& inObject, const char* inProperty, Quaternion& outQuat)
{
    auto iter = inObject.FindMember(inProperty);
    if (iter == inObject.MemberEnd())
    {
        return false;
    }

    auto& property = iter->value;
    for (rapidjson::SizeType i = 0; i < property.Size(); ++i)
    {
        if (!property[i].IsDouble())
        {
            return false;
        }
    }

    outQuat.x = property[0].GetDouble();
    outQuat.y = property[1].GetDouble();
    outQuat.z = property[2].GetDouble();
    outQuat.w = property[3].GetDouble();
    return true;
}

bool JsonHelper::getScale(const rapidjson::Value& inObject, const char* inProperty, float& outScale)
{
    auto iter = inObject.FindMember(inProperty);
    if (iter == inObject.MemberEnd())
    {
        return false;
    }

    auto& property = iter->value;
    if (!property.IsDouble())
    {
        return false;
    }

    outScale = property.GetDouble();
    return true;
}

bool LevelLoader::saveLevel(class Game* game, const std::string& fileName)
{
    // 创建doc和根对象
    rapidjson::Document doc;
    doc.SetObject();

    // 写入版本
    JsonHelper::addInt(doc.GetAllocator(), doc, "version", 1);

    // 写入全局属性
    rapidjson::Value globals(rapidjson::kObjectType);
    saveGlobalProperties(doc.GetAllocator(), game, globals);
    doc.AddMember("globalProperties", globals, doc.GetAllocator());

    // 写入角色属性
    rapidjson::Value actors(rapidjson::kArrayType);
    saveActors(doc.GetAllocator(), game, actors);
    doc.AddMember("actors", actors, doc.GetAllocator());

    // 将JSON文本保存到字符串缓存区
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* output = buffer.GetString();

    // 写入文件
    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        outFile << output;
    }

    return true;
}

bool LevelLoader::saveGlobalProperties(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inObj)
{
    // 环境光
    JsonHelper::addVector3(alloc, inObj, "ambientLight", game->getRenderer()->getAmbientLight());

    // 定向光
    DirectionalLight& dirLight = game->getRenderer()->getDirectionalLight();
    rapidjson::Value dirObj(rapidjson::kObjectType);
    JsonHelper::addVector3(alloc, dirObj, "direction", dirLight.mDirection);
    JsonHelper::addVector3(alloc, dirObj, "diffuseColor", dirLight.mDiffuseColor);
    JsonHelper::addVector3(alloc, dirObj, "specColor", dirLight.mSpecColor);
    
    inObj.AddMember("directionalLight", dirObj, alloc);

    // std::cout << "specColor:" << dirLight.mSpecColor.x 
    //     << " " << dirLight.mSpecColor.y 
    //     << " " << dirLight.mSpecColor.z 
    //     << std::endl;
    return true;
}

bool LevelLoader::saveActors(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inArray)
{
    auto actors = game->getActors();
    std::cout << "[LevelLoader] Actors size:" << actors.size() << std::endl;

    for (auto act : actors)
    {
        // 定义json对象
        rapidjson::Value obj(rapidjson::kObjectType);
        JsonHelper::addString(alloc, obj, "type", Actor::typeNames[act->getType()]);

        // 定义属性对象
        rapidjson::Value props(rapidjson::kObjectType);
        act->saveProperties(alloc, props);
        obj.AddMember("properties", props, alloc);

        // 保存组件
        rapidjson::Value comps(rapidjson::kArrayType);
        saveComponents(alloc, act, comps);
        obj.AddMember("components", comps, alloc);

        inArray.PushBack(obj, alloc);
    }

    return true;
}

bool LevelLoader::saveComponents(rapidjson::Document::AllocatorType& alloc, class Actor* actor, rapidjson::Value& inArray)
{
    const auto& comps = actor->getComponents();
    for (auto comp : comps)
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        JsonHelper::addString(alloc, obj, "type", std::string(Component::TypeNames[comp->getType()]));

        rapidjson::Value props(rapidjson::kObjectType);
        comp->saveProperties(alloc, props);

        obj.AddMember("properties", props, alloc);

        inArray.PushBack(obj, alloc);
    }

    return true;
}   

bool JsonHelper::addInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, int value)
{
    rapidjson::Value v(value);
    inObj.AddMember(rapidjson::StringRef(name), v, alloc);

    return true;
}

bool JsonHelper::addVector3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, Vector3 value)
{
    rapidjson::Value v(rapidjson::kArrayType);
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);
    inObj.AddMember(rapidjson::StringRef(name), v, alloc);

    return true;
}

bool JsonHelper::addString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, std::string value)
{
    rapidjson::Value v(rapidjson::kObjectType);
    v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);
    inObj.AddMember(rapidjson::StringRef(name), v, alloc);

    return true;
}

bool JsonHelper::addRotation(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, Quaternion value)
{
    rapidjson::Value v(rapidjson::kArrayType);
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);
    v.PushBack(rapidjson::Value(value.z).Move(), alloc);
    v.PushBack(rapidjson::Value(value.w).Move(), alloc);
    inObj.AddMember(rapidjson::StringRef(name), v, alloc);

    return true;
}

bool JsonHelper::addScale(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, float value)
{
    rapidjson::Value v(rapidjson::kObjectType); 
    v.SetFloat(value);
    inObj.AddMember(rapidjson::StringRef(name), v, alloc);

    return true;
}