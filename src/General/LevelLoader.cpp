#include "../../include/General/LevelLoader.h"
#include "../../include/Game.h"

#include "../../include/Renderers/Renderer.h"
#include "../../include/Renderers/HUD.h"
#include "../../include/Renderers/Texture.h"
#include "../../include/Renderers/UIScreen.h"
#include "../../include/Renderers/Setting.h"
#include "../../include/Renderers/PauseMenu.h"
#include "../../include/Renderers/EmptySprite.h"
#include "../../include/Renderers/DialogBox.h"
#include "../../include/Renderers/Button.h"

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

#include "../../include/General/TreeStruct.h"

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

// 加载ui元素信息
bool LevelLoader::loadUIElements(class Game* game, const std::string& fileName)
{   
    // 加载json文件
    rapidjson::Document doc;
    if (!loadJSON(fileName, doc))
    {
        SDL_Log("Failed to load level %s", fileName.c_str());
        return false;
    }

    int version = 0;
    if (!JsonHelper::getInt(doc, "version", version))
    {
        SDL_Log("Incorrect ui element file version for %s", fileName.c_str());
        return false;
    }

    // 加载HUD信息
    const rapidjson::Value& hud = doc["HUD"];
    if (hud.IsObject())
    {
        SDL_Log("[LevelLoader] load hud elems...");
        loadHUD(game, hud, game->getHUD());
    }
    SDL_Log("[LevelLoader] Load HUD over...");

    // 加载PauseMenu信息
    const rapidjson::Value& pause_menu = doc["PauseMenu"];
    if (pause_menu.IsObject())
    {
        // loadPauseMenu(game, pause_menu);
        loadHUD(game, pause_menu, game->getPauseMenu());
    }
    SDL_Log("[LevelLoader] Load PauseMenu over...");

    // 加载DialogBox信息
    const rapidjson::Value& dialog_box = doc["DialogBox"];
    if (dialog_box.IsObject())
    {
        // loadDialogBox(game, dialog_box);
        loadHUD(game, dialog_box, game->getDialogBox());
    }
    SDL_Log("[LevelLoader] Load DialogBox over...");

    // 加载Setting信息
    const rapidjson::Value& setting = doc["Setting"];
    if (setting.IsObject())
    {
        // loadSetting(game, setting);
        loadHUD(game, setting, game->getSetting());
    }

    SDL_Log("[LevelLoader] Load ui element over...");

    return true;
}

bool LevelLoader::loadUITrees(class Game* game, const std::string& fileName)
{
    SDL_Log("[LevelLoader] Load ui tree...");

    // 加载json文件
    rapidjson::Document doc;
    if (!loadJSON(fileName, doc))
    {
        SDL_Log("Failed to load level %s", fileName.c_str());
        return false;
    }

    const rapidjson::Value& v = doc["version"];
    if (v.GetInt() != 1)
    {
        SDL_Log("[LevelLoader] Load ui tree: version is not matched...");
        return false;
    }

    // 加载ui树
    const rapidjson::Value& tree = doc["tree"];
    if (tree.IsArray())
    {
        loadUITree(game, tree, game->getHUD());
    }
    else
    {
        return false;
    }

    SDL_Log("[LevelLoader] Load ui tree over!");
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
                        cp = (*iter).second(actor, compObj["properties"]);

                        SDL_Log("[LevelLoader] Create Component: %s...", type.c_str());
                    }
                    else
                    {
                        cp->loadProperties(compObj["properties"]);

                        SDL_Log("[LevelLoader] Update Component: %s...", type.c_str());
                    }
                }
            }
        }
    }
}

bool LevelLoader::loadUITree(class Game* game, const rapidjson::Value& inArray, class UIScreen* ui)
{
    if (ui && inArray.IsArray())
    {
        auto hud = (HUD*)ui;
        auto tree = hud->getUIMenuTree();
        for (rapidjson::SizeType i = 0; i < inArray.Size(); ++i)
        {
            const rapidjson::Value& node = inArray[i];
            if (node.IsObject())
            {
                std::string node_name, parent_name;
                if (JsonHelper::getString(node, "node_name", node_name) &&
                    JsonHelper::getString(node, "parent_name", parent_name)
                )
                {
                    auto iter = node.FindMember("value");
                    if (iter != node.MemberEnd())
                    {
                        auto& v = iter->value;

                        std::string name, type, bind_name;
                        int updateOrder = 100;
                        Vector2 pos = Vector2{0.0f, 0.0f};
                        Vector2 scale = Vector2{0.0f, 0.0f};
                        if (JsonHelper::getString(v, "name", name) &&
                            JsonHelper::getString(v, "type", type) &&
                            v.HasMember("properties")
                        )
                        {
                            const rapidjson::Value& props = v["properties"];
                            if (JsonHelper::getInt(props, "updateOrder", updateOrder) &&
                                JsonHelper::getVector2(props, "position", pos) &&
                                JsonHelper::getVector2(props, "scale", scale)
                            )
                            {
                                std::string text;
                                std::map<std::string, std::string> texs_map;
                                if (!strcmp(type.c_str(), "button"))
                                {
                                    std::string buttonOn, buttonOff;
                                    int event_id = -1;
                                    // String2WString str2Wstr;
                                    const rapidjson::Value& texs = props["bindTexName"];
                                    if (texs.IsObject() &&
                                        JsonHelper::getString(texs, "off", buttonOff) &&
                                        JsonHelper::getString(texs, "on", buttonOn) &&
                                        JsonHelper::getInt(props, "bindEventID", event_id)
                                    )
                                    {
                                        texs_map.emplace("off", buttonOff);
                                        texs_map.emplace("on", buttonOn);
                                        
                                        Button* b = nullptr;
                                        if (JsonHelper::getString(props, "bindText", text))
                                        {
                                            // std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                                            // std::wstring w_str = converter.from_bytes(text);

                                            wchar_t pwchar[text.length()] = {'\0'};
                                            for (int i = 0; i < (int)text.length(); ++i)
                                            {
                                                memcpy(&pwchar[i], &text[i], sizeof(char));
                                            }  
                                            wstring w_str(pwchar, pwchar + (int)text.length());
                                            
                                            int font_size = 35;
                                            Vector3 text_color = Vector3{0.5, 0.25, 0.25};
                                            // 读取文本颜色和字体大小
                                            JsonHelper::getVector3(props, "textColor", text_color);
                                            JsonHelper::getInt(props, "fontSize", font_size);

                                            b = ui->addButton(
                                                name, type, 
                                                texs_map, 
                                                w_str, text_color, font_size,
                                                pos, (UIScreen::UIBindEvent)event_id, [ui, event_id](){
                                                    ui->bindEvent((UIScreen::UIBindEvent)event_id);
                                                }, false, scale
                                            );
                                        }
                                        else
                                        {
                                            b = ui->addButton(
                                                name, type,
                                                texs_map,
                                                pos, (UIScreen::UIBindEvent)event_id, [ui, event_id](){
                                                    ui->bindEvent((UIScreen::UIBindEvent)event_id);
                                                }, false, scale
                                            );
                                        }

                                        // 添加到ui树中
                                        if (b)
                                        {
                                            tree->addTreeNode(new TreeNode{
                                                node_name, parent_name,
                                                (EmptySprite*)b
                                            });
                                        }
                                    }
                                }
                                else
                                {
                                    JsonHelper::getString(props, "bindTexName", bind_name);
                                    texs_map.emplace("default", bind_name);

                                    EmptySprite* s = nullptr;
                                    if (JsonHelper::getString(props, "bindText", text))
                                    {
                                        wchar_t pwchar[text.length()] = {'\0'};
                                        for (int i = 0; i < (int)text.length(); ++i)
                                        {
                                            memcpy(&pwchar[i], &text[i], sizeof(char));
                                        }  
                                        wstring w_str(pwchar, pwchar + (int)text.length());

                                        int font_size = 35;
                                        Vector3 text_color = Vector3{0.5, 0.25, 0.25};
                                        
                                        // 读取文本颜色和字体大小
                                        JsonHelper::getVector3(props, "textColor", text_color);
                                        JsonHelper::getInt(props, "fontSize", font_size);

                                        s = new EmptySprite(
                                            ui, 
                                            name, 
                                            type,
                                            texs_map,  
                                            false,
                                            pos, 
                                            scale, 
                                            updateOrder,
                                            Vector2{0.0f, 0.0f},
                                            true, w_str, text_color, font_size 
                                        );
                                    }
                                    else
                                    {
                                        s = new EmptySprite(
                                            ui, 
                                            name, 
                                            type,
                                            texs_map,  
                                            false,
                                            pos, 
                                            scale, 
                                            updateOrder
                                        );
                                    }

                                    if (s)
                                    {
                                        // 添加到ui树中
                                        tree->addTreeNode(new TreeNode{
                                            node_name, 
                                            parent_name, 
                                            s
                                        });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // 将树的根节点入栈
        hud->nodeAddToStack(tree->findTreeNode("root"));
    }
    else
    {
        return false;
    }

    return true;
}

void LevelLoader::loadHUD(class Game* game, const rapidjson::Value& inObject, class UIScreen* ui)
{
    if (ui)
    {
        auto iter = inObject.FindMember("textures");
        if (iter != inObject.MemberEnd())
        {
            auto& textures = iter->value;
            for (rapidjson::SizeType i = 0; i < textures.Size(); ++i)
            {
                const rapidjson::Value& tex = textures[i];
                if (tex.IsObject())
                {
                    std::string name, path;
                    if (JsonHelper::getString(tex, "name", name) &&
                        JsonHelper::getString(tex, "texturePath", path)
                    )
                    {
                        auto t = game->getRenderer()->getTexture(path.c_str());
                        ui->addUITexture(name, t);
                    }
                }
            }
        }

        iter = inObject.FindMember("elements");
        if (iter != inObject.MemberEnd())
        {
            auto& elements = iter->value;
            for (rapidjson::SizeType i = 0; i < elements.Size(); ++i)
            {
                const rapidjson::Value& elem = elements[i];
                if (elem.IsObject())
                {
                    std::string name, type, bind_name;
                    int updateOrder = 100;
                    Vector2 pos = Vector2{0.0f, 0.0f};
                    Vector2 scale = Vector2{0.0f, 0.0f};

                    if (JsonHelper::getString(elem, "name", name) &&
                        JsonHelper::getString(elem, "type", type) &&
                        elem.HasMember("properties"))
                    {
                        const rapidjson::Value& props = elem["properties"];
                        if (JsonHelper::getInt(props, "updateOrder", updateOrder) &&
                            JsonHelper::getVector2(props, "position", pos) &&
                            JsonHelper::getVector2(props, "scale", scale)
                        )
                        {
                            std::string text;
                            std::map<std::string, std::string> texs_map;
                            if (!strcmp(type.c_str(), "button"))
                            {
                                SDL_Log("[LevelLoader] load button name: %s", name.c_str());

                                std::string buttonOn, buttonOff;
                                int event_id = -1;
                                // String2WString str2Wstr;
                                const rapidjson::Value& texs = props["bindTexName"];
                                if (texs.IsObject() &&
                                    JsonHelper::getString(texs, "off", buttonOff) &&
                                    JsonHelper::getString(texs, "on", buttonOn) &&
                                    JsonHelper::getInt(props, "bindEventID", event_id)
                                )
                                {
                                    texs_map.emplace("off", buttonOff);
                                    texs_map.emplace("on", buttonOn);

                                    if (JsonHelper::getString(props, "bindText", text))
                                    {
                                        // std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                                        // std::wstring w_str = converter.from_bytes(text);

                                        wchar_t pwchar[text.length()] = {'\0'};
                                        for (int i = 0; i < (int)text.length(); ++i)
                                        {
                                            memcpy(&pwchar[i], &text[i], sizeof(char));
                                        }  
                                        wstring w_str(pwchar, pwchar + (int)text.length());

                                        int font_size = 35;
                                        Vector3 text_color = Vector3{0.5, 0.25, 0.25};
                                        
                                        // 读取文本颜色和字体大小
                                        JsonHelper::getVector3(props, "textColor", text_color);
                                        JsonHelper::getInt(props, "fontSize", font_size);

                                        ui->addButton(
                                            name, type, 
                                            texs_map, 
                                            w_str, text_color, font_size,
                                            pos, (UIScreen::UIBindEvent)event_id, [ui, event_id](){
                                                ui->bindEvent((UIScreen::UIBindEvent)event_id);
                                            }, true, scale
                                        );
                                    }
                                    else
                                    {
                                        ui->addButton(
                                            name, type,
                                            texs_map,
                                            pos, (UIScreen::UIBindEvent)event_id, [ui, event_id](){
                                                ui->bindEvent((UIScreen::UIBindEvent)event_id);
                                            }, true, scale
                                        );
                                    }

                                    // SDL_Log("[LevelLoader] wstring: %d string: %d", w_str.length(), text.length());
                                    // SDL_Log("[LevelLoader] wstring size: %d string size: %d", sizeof(std::wstring), sizeof(std::string));
                                }
                            }
                            else
                            {
                                JsonHelper::getString(props, "bindTexName", bind_name);
                                texs_map.emplace("default", bind_name);

                                if (JsonHelper::getString(props, "bindText", text))
                                {
                                    wchar_t pwchar[text.length()] = {'\0'};
                                    for (int i = 0; i < (int)text.length(); ++i)
                                    {
                                        memcpy(&pwchar[i], &text[i], sizeof(char));
                                    }  
                                    wstring w_str(pwchar, pwchar + (int)text.length());

                                    int font_size = 35;
                                    Vector3 text_color = Vector3{0.5, 0.25, 0.25};
                                    
                                    // 读取文本颜色和字体大小
                                    JsonHelper::getVector3(props, "textColor", text_color);
                                    JsonHelper::getInt(props, "fontSize", font_size);

                                    new EmptySprite(
                                        ui, 
                                        name, 
                                        type,
                                        texs_map,  
                                        true,
                                        pos, 
                                        scale, 
                                        updateOrder,
                                        Vector2{0.0f, 0.0f},
                                        true, w_str, text_color, font_size 
                                    );
                                }
                                else
                                {
                                    new EmptySprite(
                                        ui, 
                                        name, 
                                        type,
                                        texs_map,  
                                        true,
                                        pos, 
                                        scale, 
                                        updateOrder
                                    );
                                }
                            }

                            SDL_Log("[LevelLoader] Load HUD element name: %s", name.c_str());
                        }
                    }
                }
            }
        }
    }
}

bool LevelLoader::saveUIElements(class Game* game, const std::string& fileName)
{
    // 保存ui元素信息

    // 创建doc和根对象
    rapidjson::Document doc;
    doc.SetObject();

    // 写入版本
    JsonHelper::addInt(doc.GetAllocator(), doc, "version", 1);

    // 保存hud信息
    rapidjson::Value hud(rapidjson::kObjectType);
    saveHUD(doc.GetAllocator(), game, hud, game->getHUD());
    doc.AddMember("HUD", hud, doc.GetAllocator());

    // 保存pausemenu信息
    rapidjson::Value pause_menu(rapidjson::kObjectType);
    saveHUD(doc.GetAllocator(), game, pause_menu, game->getPauseMenu());
    doc.AddMember("PauseMenu", pause_menu, doc.GetAllocator());

    // 保存dialogbox信息
    rapidjson::Value dialog_box(rapidjson::kObjectType);
    saveHUD(doc.GetAllocator(), game, dialog_box, game->getDialogBox());
    doc.AddMember("DialogBox", dialog_box, doc.GetAllocator());

    // 保存setting信息
    rapidjson::Value setting(rapidjson::kObjectType);
    saveHUD(doc.GetAllocator(), game, setting, game->getSetting());
    doc.AddMember("Setting", setting, doc.GetAllocator());


    // 将JSON文本保存到字符串缓存区
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* output = buffer.GetString();

    // 写入文件
    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        SDL_Log("[LevelLoader] Save hud elements...");

        outFile << output;
    }

    return true;
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

bool JsonHelper::getVector2(const rapidjson::Value& inObject, const char* inProperty, Vector2& outVector2)
{
    auto iter = inObject.FindMember(inProperty);
    if (iter == inObject.MemberEnd())
    {
        return false;
    }

    auto& property = (*iter).value;
    if (property.Size() != 2 || !property.IsArray())
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

    outVector2.x = property[0].GetDouble();
    outVector2.y = property[1].GetDouble();

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

        SDL_Log("[LevelLoader] Save global level elements...");
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

bool LevelLoader::saveHUD(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inObject, class UIScreen* ui)
{
    rapidjson::Value textures(rapidjson::kArrayType);
    for (auto iter : ui->getUITextures())
    {   
        rapidjson::Value obj(rapidjson::kObjectType);
        JsonHelper::addString(alloc, obj, "name", iter.first);
        JsonHelper::addString(alloc, obj, "texturePath", iter.second->getFileName());
        textures.PushBack(obj, alloc);
    }

    rapidjson::Value elements(rapidjson::kArrayType);
    for (auto elem : ui->getUIElements())
    {
        rapidjson::Value obj(rapidjson::kObjectType);
        if (JsonHelper::addString(alloc, obj, "name", elem->getSpriteName()) &&
            JsonHelper::addString(alloc, obj, "type", elem->getType()))
        {
            rapidjson::Value props(rapidjson::kObjectType);

            if (elem->containText())
            {
                wstring w_str = elem->getText();
                char* pschar = new char[w_str.length()];
                // memset(pschar, '\0', w_str.length());
                // memcpy(pschar, w_str.c_str(), sizeof(wchar_t) * w_str.length()); 
                for (auto i = 0; i < (int)w_str.length(); ++i)
                {
                    pschar[i] = w_str[i];
                }
                string str(pschar, pschar + w_str.length());

                Vector3 color = elem->getTextColor();
                int size = elem->getFontSize();

                JsonHelper::addString(alloc, props, "bindText", str);
                JsonHelper::addVector3(alloc, props, "textColor", color);
                JsonHelper::addInt(alloc, props, "fontSize", size);

                if (pschar)
                {
                    delete pschar;
                    pschar = nullptr;
                }

                // SDL_Log("[LevelLoader] bindText: wstr size: %d str size: %d", w_str.length(), str.length());
            }

            if (!strcmp(elem->getType().c_str(), "button"))
            {
                Button* b = (Button*)(elem);

                JsonHelper::addInt(alloc, props, "bindEventID", b->getBindEventID());

                rapidjson::Value texs(rapidjson::kObjectType);
                for (auto tex : b->getBindTexName())
                {
                    JsonHelper::addString(alloc, texs, tex.first, tex.second);
                }
                props.AddMember("bindTexName", texs, alloc);
            }
            else
            {
                JsonHelper::addString(alloc, props, "bindTexName", (*elem->getBindTexName().begin()).second);
            }

            JsonHelper::addInt(alloc, props, "updateOrder", elem->getUpdateOrder());
            JsonHelper::addVector2(alloc, props, "position", elem->getPosition());
            JsonHelper::addVector2(alloc, props, "scale", elem->getScale());
            obj.AddMember("properties", props, alloc);
            elements.PushBack(obj, alloc);
        }
    }

    inObject.AddMember("textures", textures, alloc);
    inObject.AddMember("elements", elements, alloc);
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

    // printf("pt: %s \n", name);
    inObj.AddMember(rapidjson::StringRef(name), v, alloc);

    return true;
}

bool JsonHelper::addString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const string& name, std::string value)
{
    rapidjson::Value n(rapidjson::kObjectType);
    n.SetString(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), alloc);

    rapidjson::Value v(rapidjson::kObjectType);
    v.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.length()), alloc);

    inObj.AddMember(n, v, alloc);
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

bool JsonHelper::addVector2(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, Vector2 value)
{
    rapidjson::Value v(rapidjson::kArrayType);
    v.PushBack(rapidjson::Value(value.x).Move(), alloc);
    v.PushBack(rapidjson::Value(value.y).Move(), alloc);

    inObj.AddMember(rapidjson::StringRef(name), v, alloc);

    return true;
}
